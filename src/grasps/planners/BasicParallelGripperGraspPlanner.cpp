/* 
 * File:   BasicParallelGripperGraspPlanner.cpp
 * Author: dagothar
 * 
 * Created on October 8, 2015, 4:27 PM
 */

#include "BasicParallelGripperGraspPlanner.hpp"
#include "grasps/SurfaceSample.hpp"
#include <rw/math/RPY.hpp>
#include <rw/geometry/TriMeshSurfaceSampler.hpp>
#include <rwlibs/proximitystrategies/ProximityStrategyFactory.hpp>

using namespace std;
using namespace gripperz::context;
using namespace gripperz::grasps;
using namespace gripperz::grasps::planners;
using namespace rw::common;
using namespace rw::math;
using namespace rw::models;
using namespace rw::kinematics;
using namespace rw::geometry;
using namespace rw::proximity;
using namespace rwlibs::proximitystrategies;
using namespace rwlibs::task;

BasicParallelGripperGraspPlanner::BasicParallelGripperGraspPlanner(unsigned nGrasps, const rw::kinematics::State& state, context::TaskDescription::Ptr td) :
GraspPlanner(nGrasps),
_state(state),
_td(td) {
}

BasicParallelGripperGraspPlanner::~BasicParallelGripperGraspPlanner() {
}

void moveFrameW(const Transform3D<>& wTtcp, Frame* tcp, MovableFrame* base, State& state) {
    Transform3D<> tcpTbase = Kinematics::frameTframe(tcp, base, state);
    Transform3D<> wTbase_target = wTtcp * tcpTbase;

    base->moveTo(wTbase_target, state);
}

SurfaceSample sample(TaskDescription::Ptr td, const rw::kinematics::State& state, TriMeshSurfaceSampler& sampler, ProximityModel::Ptr object, ProximityModel::Ptr ray, CollisionStrategy::Ptr cstrategy) {
    Transform3D<> wTobj = Kinematics::worldTframe(td->getTargetObject()->getBase(), state);

    // choose a random number in the total area
    TriMesh::Ptr mesh = sampler.getMesh();
    ProximityStrategyData data;
    data.setCollisionQueryType(CollisionStrategy::AllContacts);

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
        Transform3D<> rayTrans(pos - faceNormal * 0.001, RPY<>(Math::ran(-0.1, 0.1), Math::ran(-0.1, 0.1), Math::ran(-0.1, 0.1)).toRotation3D() * rot);

        // now we want to find any triangles that collide with the ray and which are parallel with the sampled
        // this should look for all the collisions, so should detect a proper grasp regardless of presence of folds in the model
        cstrategy->inCollision(object, Transform3D<>::identity(), ray, rayTrans, data);
        typedef std::pair<int, int> PrimID;

        BOOST_FOREACH(PrimID pid, data.getCollisionData()._geomPrimIds) {

            // search for a triangle that has a normal
            Triangle<> tri = mesh->getTriangle(pid.first);
            Vector3D<> normal = tri.calcFaceNormal();

            bool closeAngle = angle(-faceNormal, normal) < 50 * Deg2Rad; // are the normals aprox. parallel?

            // calculate the distance between primitives on the opposite sides of an object
            double d1 = dot(tri[0], -faceNormal); // cast a triangle point on the origin
            double d2 = dot(pos, -faceNormal); // cast a pos point on the origin
            double dist = d1 - d2;

            if (closeAngle) {

                // calculate target
                Vector3D<> avgNormal = normalize((-faceNormal + normal) / 2.0);
                Vector3D<> xcol = normalize(cross(xaxis, -avgNormal));
                Vector3D<> ycol = normalize(cross(-avgNormal, xcol));
                Rotation3D<> rot2(xcol, ycol, -avgNormal);
                Rotation3D<> trot = rot2 * RPY<>(Math::ran(0.0, Pi * 2.0), 0, 0).toRotation3D();

                // next we rotate z-axis into place
                trot = trot * RPY<>(0, 90 * Deg2Rad, 0).toRotation3D();
                target.R() = trot;

                graspW = dist;

                target.P() = pos - faceNormal * (dist / 2.0);

                if (td->hasHints()) {
                    targetFound = false;
                } else {
                    targetFound = true;
                }
                // test if the target belongs in the area around hinted grasps
                Transform3D<> targetW = wTobj * target; //inverse(wTobj) * target;

                BOOST_FOREACH(Transform3D<> hint, td->getHints()) {
                    // calculate distance
                    Q teachDist = td->getTeachDistance();

                    bool distOk = std::fabs(targetW.P()[0] - hint.P()[0]) <= teachDist[0] &&
                            std::fabs(targetW.P()[1] - hint.P()[1]) <= teachDist[1] &&
                            std::fabs(targetW.P()[2] - hint.P()[2]) <= teachDist[2];



                    /* CRAZY STUFF */
                    Vector3D<> targetZ = targetW.R() * Vector3D<>::z();
                    Vector3D<> hintZ = hint.R() * Vector3D<>::z();
                    Vector3D<> targetY = targetW.R() * Vector3D<>::y();
                    Vector3D<> hintY = hint.R() * Vector3D<>::y();

                    bool angleOk = (std::fabs(angle(targetZ, hintZ)) <= td->getTeachDistance()[3]) &&
                            (std::fabs(angle(targetY, hintY)) <= td->getTeachDistance()[4]);

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

Grasps BasicParallelGripperGraspPlanner::planGrasps(unsigned nGrasps) {
    vector<SurfaceSample> ssamples = _ssamples;

    Transform3D<> wTobj = Kinematics::worldTframe(_targetObject->getBase(), _state);

    // setup task
    Grasps grasps = ownedPtr(new GraspTask);
    grasps->getSubTasks().resize(1);
    GraspSubTask& stask = grasps->getSubTasks()[0];
    grasps->setGripperID(_td->getGripperID());
    stask.offset = wTobj;
    stask.approach = Transform3D<>(Vector3D<>(0, 0, 0.1));
    stask.retract = Transform3D<>(Vector3D<>(0, 0, 0.1));
    stask.openQ = Q(1, _td->getGripperDevice()->getBounds().second[0]);
    stask.closeQ = Q(1, _td->getGripperDevice()->getBounds().first[0]);
    grasps->setTCPID(_td->getGripperTCP()->getName());
    grasps->setGraspControllerID(_td->getControllerID());

    // prepare
    TriMeshSurfaceSampler sampler(_targetObject->getGeometry()[0]);
    sampler.setRandomPositionEnabled(false);
    sampler.setRandomRotationEnabled(false);

    CollisionStrategy::Ptr cstrategy = ProximityStrategyFactory::makeDefaultCollisionStrategy();

    PlainTriMeshF *rayMesh = new PlainTriMeshF(1);
    (*rayMesh)[0] = Triangle<float>(Vector3D<float>(0, (float) - 0.001, 0), Vector3D<float>(0, (float) 0.001, 0), Vector3D<float>(0, 0, (float) 10));
    ProximityModel::Ptr ray = cstrategy->createModel();
    Geometry geom(rayMesh);
    geom.setId("Ray");
    ray->addGeometry(geom);

    ProximityModel::Ptr object = cstrategy->createModel();
    cstrategy->addGeometry(object.get(), _targetObject->getGeometry()[0]);

    CollisionDetector::Ptr cdetect = new CollisionDetector(_td->getWorkCell(), ProximityStrategyFactory::makeDefaultCollisionStrategy());

    BOOST_FOREACH(Object::Ptr obj, _td->getInterferenceObjects()) {
        cdetect->addRule(ProximitySetupRule::makeExclude("gripper.Base", obj->getBase()->getName()));
        cdetect->addRule(ProximitySetupRule::makeExclude("gripper.LeftFinger", obj->getBase()->getName()));
        cdetect->addRule(ProximitySetupRule::makeExclude("gripper.RightFinger", obj->getBase()->getName()));
    }

    unsigned failures_in_row = 0;
    unsigned successes = 0;
    while (successes < nGrasps) {
        SurfaceSample ssample;

        if (ssamples.size() > 0) {
            ssample = ssamples.back();
            ssamples.pop_back();

        } else {
            ssample = sample(_td, _state, sampler, object, ray, cstrategy);
        }

        Transform3D<>& target = ssample.transform;

        Q oq = Q(1, _td->getGripperDevice()->getBounds().second[0]);
        _td->getGripperDevice()->setQ(oq, _state);

        // then check for collision
        moveFrameW(wTobj * target, _td->getGripperTCP().get(), _td->getGripperMovable(), _state);

        CollisionDetector::QueryResult result;
        if (!cdetect->inCollision(_state, &result, true)) {
            ++successes;
            failures_in_row = 0;

            // make new subtask (for tasks)
            GraspSubTask subtask;
            subtask.offset = wTobj;
            subtask.approach = Transform3D<>(Vector3D<>(0, 0, 0.3));
            subtask.retract = Transform3D<>(Vector3D<>(0, 0, 0.1));
            subtask.openQ = oq;
            subtask.closeQ = Q(1, _td->getGripperDevice()->getBounds().first[0]);

            GraspTarget gtarget(target);
            gtarget.result = ownedPtr(new GraspResult());
            gtarget.result->testStatus = GraspResult::UnInitialized;
            gtarget.result->objectTtcpTarget = target;
            subtask.addTarget(gtarget);
            grasps->addSubTask(subtask);
            
        } else {
            ++failures_in_row;
            if (failures_in_row > 10000) {
                RW_WARN("Something is rotten in the state of RobWork: " << successes << "/" << failures_in_row);
                break;
            }

            //DEBUG << "Adding sample" << endl;

            GraspTarget gtarget(target);
            gtarget.result = ownedPtr(new GraspResult());
            gtarget.result->testStatus = GraspResult::Skip;
            gtarget.result->objectTtcpTarget = target;
            gtarget.result->gripperConfigurationGrasp = oq;

            GraspSubTask asubtask;
            asubtask.addTarget(gtarget);
            grasps->addSubTask(asubtask);
        }
    }


    return grasps;
}
