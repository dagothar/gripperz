#include "TaskGenerator.hpp"

#include <vector>
#include <iostream>
#include <rw/math.hpp>
#include <rwlibs/algorithms/kdtree/KDTree.hpp>
#include <rwlibs/algorithms/kdtree/KDTreeQ.hpp>
#include <rw/geometry/TriMeshSurfaceSampler.hpp>
#include <rwlibs/proximitystrategies/ProximityStrategyFactory.hpp>


#define DEBUG rw::common::Log::debugLog()
#define INFO rw::common::Log::infoLog()


using namespace std;

USE_ROBWORK_NAMESPACE;
using namespace robwork;
using namespace rw::math;
using namespace rwsim;
using namespace gripperz::context;
using namespace gripperz::grasps;



TaskGenerator::TaskGenerator(TaskDescription::Ptr td) :
	_td(td),
	_tasks(NULL),
	_samples(NULL)
{
	_openQ = Q(1, td->getGripperDevice()->getBounds().second[0]);
	_closeQ = Q(1, td->getGripperDevice()->getBounds().first[0]);
}



void TaskGenerator::moveFrameW(const Transform3D<>& wTtcp, Frame* tcp,
			MovableFrame* base, State& state)
{
    Transform3D<> tcpTbase = Kinematics::frameTframe(tcp, base, state);
    Transform3D<> wTbase_target = wTtcp * tcpTbase;
   
    base->moveTo(wTbase_target, state);
}



SurfaceSample TaskGenerator::sample(TriMeshSurfaceSampler& sampler, ProximityModel::Ptr object, ProximityModel::Ptr ray, CollisionStrategy::Ptr cstrategy)
{
	Transform3D<> wTobj = Kinematics::worldTframe(_td->getTargetObject()->getBase(), _td->getInitState());

    // choose a random number in the total area
    TriMesh::Ptr mesh = sampler.getMesh();
    ProximityStrategyData data;
    data.setCollisionQueryType( CollisionStrategy::AllContacts );
    
    bool targetFound = false;
    int tries = 0;
    Transform3D<> target;
    double graspW = 0.0;
    do { 
        target = sampler.sample();
        Vector3D<> pos = target.P();
        
        // z-axis is aligned with tri normal
        Vector3D<> negFaceNormal = -(target.R() * Vector3D<>::z());
        Vector3D<> faceNormal = -negFaceNormal;
        Vector3D<> xaxis = target.R() * Vector3D<>::x();

        Rotation3D<> rot(normalize(cross(xaxis, -faceNormal)), xaxis, -faceNormal);
        
        // create ray transform and add some noise to the "shooting direction"
        Transform3D<> rayTrans(pos-faceNormal*0.001, RPY<>(Math::ran(-0.1, 0.1), Math::ran(-0.1, 0.1), Math::ran(-0.1, 0.1)).toRotation3D()*rot);

        // now we want to find any triangles that collide with the ray and which are parallel with the sampled
        // this should look for all the collisions, so should detect a proper grasp regardless of presence of folds in the model
        cstrategy->inCollision(object, Transform3D<>::identity(), ray, rayTrans, data);
        typedef std::pair<int,int> PrimID;
        
        BOOST_FOREACH(PrimID pid, data.getCollisionData()._geomPrimIds) {
			
			// search for a triangle that has a normal
			Triangle<> tri = mesh->getTriangle(pid.first);
			Vector3D<> normal = tri.calcFaceNormal();
			
			bool closeAngle = angle(-faceNormal,normal)<50*Deg2Rad; // are the normals aprox. parallel?
			
			// calculate the distance between primitives on the opposite sides of an object
			double d1 = dot(tri[0], -faceNormal); // cast a triangle point on the origin
			double d2 = dot(pos, -faceNormal); // cast a pos point on the origin
			double dist = d1 - d2;

			if (closeAngle) {
				
				// calculate target
				Vector3D<> avgNormal = normalize((-faceNormal + normal)/2.0);
				Vector3D<> xcol = normalize(cross(xaxis, -avgNormal));
				Vector3D<> ycol = normalize(cross(-avgNormal, xcol));
				Rotation3D<> rot2(xcol, ycol, -avgNormal);
				Rotation3D<> trot = rot2*RPY<>(Math::ran(0.0,Pi*2.0), 0, 0).toRotation3D();
				
				// next we rotate z-axis into place
				trot = trot * RPY<>(0, 90*Deg2Rad, 0).toRotation3D();
				target.R() = trot; 

				graspW = dist;

				target.P() = pos-faceNormal*(dist/2.0);
				
				if (_td->hasHints()) {
					targetFound = false;
				} else {
					targetFound = true;
				}
				// test if the target belongs in the area around hinted grasps
				Transform3D<> targetW = wTobj * target; //inverse(wTobj) * target;
				
				BOOST_FOREACH (Transform3D<> hint, _td->getHints()) {
					// calculate distance
					Q teachDist = _td->getTeachDistance();
					
					bool distOk = std::fabs(targetW.P()[0] - hint.P()[0]) <= teachDist[0] &&
						std::fabs(targetW.P()[1] - hint.P()[1]) <= teachDist[1] &&
						std::fabs(targetW.P()[2] - hint.P()[2]) <= teachDist[2];
						
					
					
					/* CRAZY STUFF */
					Vector3D<> targetZ = targetW.R() * Vector3D<>::z();
					Vector3D<> hintZ = hint.R() * Vector3D<>::z();
					Vector3D<> targetY = targetW.R() * Vector3D<>::y();
					Vector3D<> hintY = hint.R() * Vector3D<>::y();
					
					bool angleOk = (std::fabs(angle(targetZ, hintZ)) <= _td->getTeachDistance()[3]) && 
						(std::fabs(angle(targetY, hintY)) <= _td->getTeachDistance()[4]);
						
					if (distOk && angleOk) {
						targetFound = true;
						break;
					}
				}
				
				if (targetFound) break;
			}
        }
        
        tries++;
        if (tries > 25000) {
			RW_THROW("Cannot find target that's OK! Tries: " << tries);
		}
    } while (!targetFound);
    
    return SurfaceSample(target, graspW);
}



rwlibs::task::GraspTask::Ptr TaskGenerator::filterTasks(const rwlibs::task::GraspTask::Ptr tasks, rw::math::Q diff)
{	
	if (!tasks) {
		return NULL;
	}
	
	GraspTask::Ptr tasks1 = copyTasks(tasks);
	
	// create nodes for succesful grasps
	typedef GraspResult::Ptr ValueType;
	typedef KDTreeQ<ValueType> NNSearch;
	vector<NNSearch::KDNode> nodes;

	int nTasks = 0;
	
	typedef std::pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, tasks1->getAllTargets()) {
		
		if (p.second->getResult()->testStatus == GraspResult::Success ||
			p.second->getResult()->testStatus == GraspResult::Interference ||
			p.second->getResult()->testStatus == GraspResult::ObjectSlipped ||
			p.second->getResult()->testStatus == GraspResult::UnInitialized) {
				
			Q key(7);
            key[0] = p.second->pose.P()[0];
            key[1] = p.second->pose.P()[1];
            key[2] = p.second->pose.P()[2];
            EAA<> eaa(p.second->pose.R());
            key[3] = eaa.axis()(0);
            key[4] = eaa.axis()(1);
            key[5] = eaa.axis()(2);
            key[6] = eaa.angle();
            
			nodes.push_back(NNSearch::KDNode(key, p.second->getResult()));
			
			++nTasks;
		}
	}
	
	NNSearch *nntree = NNSearch::buildTree(nodes);
    std::list<const NNSearch::KDNode*> result;
    
    int nRemoved = 0;
    BOOST_FOREACH (NNSearch::KDNode& node, nodes) {
		if (node.value->testStatus != GraspResult::Filtered) {
			result.clear();
			Q key = node.key;
			nntree->nnSearchRect(key-diff, key+diff, result);

			int removed = 0;
			BOOST_FOREACH (const NNSearch::KDNode* n, result) {
				if (n->key == node.key) continue;
				
				if (n->value->testStatus != GraspResult::Filtered) ++removed;
				const_cast<NNSearch::KDNode*>(n)->value->testStatus = GraspResult::Filtered; // this is a hack
			}
			nRemoved += removed;
		}
	}
	
	return tasks1;
}



int TaskGenerator::countTasks(const rwlibs::task::GraspTask::Ptr tasks, const rwlibs::task::GraspResult::TestStatus status)
{
	int n = 0;
	
	typedef std::pair<class GraspSubTask*, class GraspTarget*> TaskTarget;
	BOOST_FOREACH (TaskTarget p, tasks->getAllTargets()) {
		if (p.second->getResult()->testStatus == status) {
			++n;
		}
	}
	
	return n;
}



rwlibs::task::GraspTask::Ptr  TaskGenerator::copyTasks(const rwlibs::task::GraspTask::Ptr tasks, bool onlySuccesses)
{
	GraspTask::Ptr tasks_copy = tasks->clone();
	
	// clone subtasks
	BOOST_FOREACH (GraspSubTask& subtask, tasks->getSubTasks()) {
		GraspSubTask subtask_copy = subtask.clone();
		
		// copy targets
		BOOST_FOREACH (GraspTarget& target, subtask.getTargets()) {
			if (!onlySuccesses || target.getResult()->testStatus == GraspResult::Success) {
				subtask_copy.addTarget(target);
			}
		}
		
		tasks_copy->addSubTask(subtask_copy);
	}
	
	return tasks_copy;
}



rwlibs::task::GraspTask::Ptr TaskGenerator::addPerturbations(rwlibs::task::GraspTask::Ptr tasks, double sigma_p, double sigma_a, int perturbations)
{
	GraspTask::Ptr perturbed = tasks->clone();
	int perturbationsPerTarget = perturbations / tasks->getAllTargets().size() + 1;
	
	int generated = 0;
	bool stop = false;
	BOOST_FOREACH (GraspSubTask &stask, tasks->getSubTasks()) {
		GraspSubTask stask_copy = stask.clone();
		
        BOOST_FOREACH (GraspTarget &target, stask.getTargets()) {
            
            for (int i=0; i < perturbationsPerTarget; i++) {
                Vector3D<> pos(Math::ranNormalDist(0, sigma_p), Math::ranNormalDist(0, sigma_p), Math::ranNormalDist(0, sigma_p));
                
                // we can do this only for small sigmas (approximation)
                EAA<> rot(Math::ranNormalDist(0, sigma_a), Math::ranNormalDist(0, sigma_a), Math::ranNormalDist(0, sigma_a));

                Transform3D<> ntarget = target.pose * Transform3D<>(pos, rot);
                stask_copy.addTarget(ntarget);
                
                ++generated;
                if (generated >= perturbations) {
					stop = true;
					break;
				}
            }
            
            if (stop) break;
        }
        
        perturbed->addSubTask(stask_copy);
        
        if (stop) break;
    }
    
    return perturbed;
}


rwlibs::task::GraspTask::Ptr TaskGenerator::generateTasks(
	int nTargets,
	rw::kinematics::State state
) {
	return generateTask(nTargets, state, &_ssamples, 0);
}



rwlibs::task::GraspTask::Ptr TaskGenerator::generateTask(int nTargets, rw::kinematics::State state, std::vector<SurfaceSample>* ssamples, int nSamples)
{
	Transform3D<> wTobj = Kinematics::worldTframe(_td->getTargetObject()->getBase(), state);
	
	// setup task
	GraspTask::Ptr gtask = new GraspTask;
	gtask->getSubTasks().resize(1);
	GraspSubTask& stask = gtask->getSubTasks()[0];
	gtask->setGripperID(_td->getGripperID());
    stask.offset = wTobj;
    stask.approach = Transform3D<>(Vector3D<>(0, 0, 0.1));
    stask.retract = Transform3D<>(Vector3D<>(0, 0, 0.1));
    stask.openQ = _openQ;
    stask.closeQ = _closeQ;
    gtask->setTCPID(_td->getGripperTCP()->getName());
    gtask->setGraspControllerID(_td->getControllerID());
    
    // setup all samples
	GraspTask::Ptr atask = new GraspTask;
	atask->getSubTasks().resize(1);
    
    // prepare
    TriMeshSurfaceSampler sampler(_td->getTargetObject()->getGeometry()[0]);
	sampler.setRandomPositionEnabled(false);
	sampler.setRandomRotationEnabled(false);
	
	CollisionStrategy::Ptr cstrategy = ProximityStrategyFactory::makeDefaultCollisionStrategy();
    
    PlainTriMeshF *rayMesh = new PlainTriMeshF(1);
    (*rayMesh)[0] = Triangle<float>( Vector3D<float>(0,(float)-0.001,0),Vector3D<float>(0,(float)0.001,0),Vector3D<float>(0,0,(float)10) );
    ProximityModel::Ptr ray = cstrategy->createModel();
    Geometry geom(rayMesh); // we have to wrap the trimesh in an geom object
    geom.setId("Ray");
    ray->addGeometry(geom);
	
	ProximityModel::Ptr object = cstrategy->createModel();
    cstrategy->addGeometry(object.get(), _td->getTargetObject()->getGeometry()[0]);
    
    // make CD for WC & add rules excluding interference objects
    CollisionDetector::Ptr cdetect = new CollisionDetector(_td->getWorkCell(), ProximityStrategyFactory::makeDefaultCollisionStrategy());
    BOOST_FOREACH (Object::Ptr obj, _td->getInterferenceObjects()) {
		cdetect->addRule(ProximitySetupRule::makeExclude("gripper.Base", obj->getBase()->getName()));
		cdetect->addRule(ProximitySetupRule::makeExclude("gripper.LeftFinger", obj->getBase()->getName()));
		cdetect->addRule(ProximitySetupRule::makeExclude("gripper.RightFinger", obj->getBase()->getName()));
	}
    
    int failures_in_row = 0;
    int successes = 0, samples = 0;
	while ((nTargets > 0 && successes < nTargets) || (nSamples > 0 && samples < nSamples)) {
		SurfaceSample ssample;
		
		if (ssamples && ssamples->size() > 0) {
			ssample = ssamples->back();
			ssamples->pop_back();
		} else {
			ssample = sample(sampler, object, ray, cstrategy);
		}
		
		++samples;
		
		double& graspW = ssample.graspW;
		Transform3D<>& target = ssample.transform;

        // distance between grasping points is graspW
        
        Q oq = _openQ;
        oq(0) = std::max(_closeQ(0), _closeQ(0)+(graspW+0.01)/2.0);
        oq(0) = std::min(_openQ(0), oq(0) );
        _td->getGripperDevice()->setQ(oq, state);
        
        // then check for collision
        moveFrameW(wTobj * target, _td->getGripperTCP(), _td->getGripperMovable(), state);
        
        CollisionDetector::QueryResult result;
        if (!cdetect->inCollision(state, &result, true)) {
            ++successes;
            failures_in_row = 0;
            
            DEBUG << "Adding target and sample" << endl;
            
            // make new subtask (for tasks)
            GraspSubTask subtask;
            subtask.offset = wTobj;
			subtask.approach = Transform3D<>(Vector3D<>(0, 0, 0.3));
			subtask.retract = Transform3D<>(Vector3D<>(0, 0, 0.1));
			subtask.openQ = oq;
			subtask.closeQ = _closeQ;
            
            GraspTarget gtarget(target);
            gtarget.result = ownedPtr(new GraspResult());
            gtarget.result->testStatus = GraspResult::UnInitialized;
            gtarget.result->objectTtcpTarget = target;
            subtask.addTarget(gtarget);
            gtask->addSubTask(subtask);
            
            // make new subtask (for samples only)
            GraspSubTask asubtask;
			GraspTarget gtarget1(target);
            gtarget1.result = ownedPtr(new GraspResult());
            gtarget1.result->testStatus = GraspResult::UnInitialized;
            gtarget1.result->objectTtcpTarget = target;
			asubtask.addTarget(gtarget1);
            atask->addSubTask(asubtask);
        } else {			
			++failures_in_row;
			if (failures_in_row > 10000) {
				RW_THROW("Something is rotten in the state of RobWork: " << successes << "/" << failures_in_row);
				break;
			}
			
			DEBUG << "Adding sample" << endl;

			GraspTarget gtarget(target);
            gtarget.result = ownedPtr(new GraspResult());
            gtarget.result->testStatus = GraspResult::UnInitialized;
            gtarget.result->objectTtcpTarget = target;
            gtarget.result->gripperConfigurationGrasp = oq;
            
            GraspSubTask asubtask;
			asubtask.addTarget(gtarget);
            atask->addSubTask(asubtask);
		}
    }
    
    _tasks = gtask;
    _samples = atask;
    
    int ntargets = _tasks->getAllTargets().size();
    int nsamples = _samples->getAllTargets().size();
    
    // preliminary filtering
    DEBUG << "Preliminary filtering" << endl;
    Q preDist = _td->getPrefilteringDistance();
	double R = 2.0 * sin(0.25 * preDist(1));
	Q diff(7, preDist(0), preDist(0), preDist(0), R, R, R, preDist(2));
	
	DEBUG << " - filtering targets... ";
    _tasks = filterTasks(_tasks, diff);
    int nftargets = countTasks(_tasks, GraspResult::UnInitialized);
    DEBUG << nftargets << " out of " << ntargets << endl;
    
    DEBUG << " - filtering samples... ";
    _samples = filterTasks(_samples, diff);
    int nfsamples = countTasks(_samples, GraspResult::UnInitialized);
    DEBUG << nfsamples << " out of " << nsamples << endl;
    
    INFO << "Generated " << ntargets << " tasks & "	<< nsamples << " samples." << endl;
    
	return _tasks;
}

