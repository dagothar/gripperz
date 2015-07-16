#include <iostream>
#include <string>
#include <fstream>
#include <rw/rw.hpp>
#include <rw/RobWork.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <rwlibs/algorithms/PointModel.hpp>
#include <rwlibs/algorithms/LineModel.hpp>
#include <rwlibs/algorithms/StructuredLineModel.hpp>
#include <rwlibs/algorithms/PlaneModel.hpp>
#include <rwlibs/algorithms/StablePose1DModel.hpp>
#include <rwlibs/algorithms/StablePose0DModel.hpp>



using namespace std;
USE_ROBWORK_NAMESPACE;
using namespace rw;
using namespace robwork;
using namespace rwlibs::algorithms;
using namespace boost::program_options;
namespace po = boost::program_options;



vector<Transform3D<> > readData(istream& stream) {
	string line;
	vector<Transform3D<> > data;
	
	while (getline(stream, line)) {
		replace(line.begin(), line.end(), ',', ' ');
		stringstream sstr(line);
		
		// read position
		double* m = new double[3];
		sstr >> m[0] >> m[1] >> m[2];
		Vector3D<> pos(m[0], m[1], m[2]);
		
		// read rpy
		sstr >> m[0] >> m[1] >> m[2];
		RPY<> rpy(m[0], m[1], m[2]);
		
		Transform3D<> t(pos, rpy.toRotation3D());
		data.push_back(t);
	}
	
	return data;
}



void savePoints(ostream& stream, const vector<PointModel>& models) {
	BOOST_FOREACH (const PointModel& m, models) {
		Vector3D<> p = m.p();
		stream << p[0] << ", " << p[1] << ", " << p[2] << ", "
			<< m.getQuality() << ", " << m.getNumberOfInliers() << endl;
	}
}



void saveLines(ostream& stream, const vector<LineModel>& models) {
	BOOST_FOREACH (const LineModel& m, models) {
		Line line = m.line();
		stream << line.p1()[0] << ", " << line.p1()[1] << ", " << line.p1()[2] << ", "
			<< line.p2()[0] << ", " << line.p2()[1] << ", " << line.p2()[2] << ", "
			<< m.getQuality() << ", " << m.getNumberOfInliers() << endl;
	}
}



void saveSLines(ostream& stream, const vector<StructuredLineModel>& models) {
	BOOST_FOREACH (const StructuredLineModel& m, models) {
		Line line = m.line();
		stream << line.p1()[0] << ", " << line.p1()[1] << ", " << line.p1()[2] << ", "
			<< line.p2()[0] << ", " << line.p2()[1] << ", " << line.p2()[2] << ", "
			<< m.start()[0] << ", " << m.start()[1] << ", " << m.start()[2] << ", "
			<< m.getInterval() << ", "
			<< m.getQuality() << ", " << m.getNumberOfInliers() << endl;
	}
}



void savePlanes(ostream& stream, const vector<PlaneModel>& models) {
	BOOST_FOREACH (const PlaneModel& m, models) {
		stream << m.normal()[0] << ", " << m.normal()[1] << ", " << m.normal()[2] << ", "
			<< m.d() << ", " 
			<< m.getQuality() << ", " << m.getNumberOfInliers() << endl;
	}
}



void savePoses(ostream& stream, const vector<PlaneModel>& models) {
	BOOST_FOREACH (const PlaneModel& m, models) {
		stream << m.normal()[0] << ", " << m.normal()[1] << ", " << m.normal()[2] << ", "
			<< m.d() << ", " 
			<< m.getQuality() << ", " << m.getNumberOfInliers() << endl;
	}
}



int main(int argc, char* argv[]) {
	/* initialization */
	Math::seed();
	RobWork::getInstance()->initialize();
	
	string inFileName;
	string outFileName;
	
	bool findPoints = false;
	bool findLines = false;
	bool findPlanes = false;
	bool findPoses = false;
	bool findPose0 = false;
	bool findSLines = false;
	
	vector<double> pointsParams;
	vector<double> linesParams;
	vector<double> planesParams;
	vector<double> posesParams;
	vector<double> pose0Params;
	vector<double> slinesParams;
	
	
	
	/* parse command line */
	string usage = "This is a script used for constraints code testing.";
	
	// switch options
	options_description opt("Options");
	opt.add_options()
		("help,h", "help message")
		("infile,", value<string>(&inFileName)->required(), "CSV file with Transform3Ds")
		//("outfile,", value<string>(&outFileName)->required(), "CSV file with models found")
		("points,", value<vector<double> >()->multitoken(), "find models for points; iterations, mindata, datathr and modelthr should follow")
		("lines,", value<vector<double> >()->multitoken(), "find models for lines; iterations, mindata, datathr and modelthr should follow")
		("planes,", value<vector<double> >()->multitoken(), "find models for planes; iterations, mindata, datathr and modelthr should follow")
		("poses,", value<vector<double> >()->multitoken(), "find models for poses; iterations, mindata, datathr and modelthr should follow")
		("pose0,", value<vector<double> >()->multitoken(), "find models for 0 dof pose; iterations, mindata, datathr and modelthr should follow")
		("slines,", value<vector<double> >()->multitoken(), "find models for structured lines; iterations, mindata, datathr and modelthr should follow")
	;
	
	// positional parameters
	positional_options_description pos_opt;
	pos_opt.add("infile", 1);
	//pos_opt.add("outfile", 1);
	
	variables_map vm;
	store(command_line_parser(argc, argv).options(opt).positional(pos_opt).run(), vm);
	notify(vm);
		
	if (vm.count("help")) {
		cout << usage << endl;
		cout << opt << endl;
		return 0;
	}
		
	if (vm.count("points")) {
		findPoints = true;
		pointsParams = vm["points"].as<vector<double> >();
		if (pointsParams.size() < 4) RW_THROW("RANSAC parameters missing");
	}
	
	if (vm.count("lines")) {
		findLines = true;
		linesParams = vm["lines"].as<vector<double> >();
		if (linesParams.size() < 4) RW_THROW("RANSAC parameters missing");
	}
	
	if (vm.count("planes")) {
		findPlanes = true;
		planesParams = vm["planes"].as<vector<double> >();
		if (planesParams.size() < 4) RW_THROW("RANSAC parameters missing");
	}
	
	if (vm.count("poses")) {
		findPoses = true;
		posesParams = vm["poses"].as<vector<double> >();
		if (posesParams.size() < 4) RW_THROW("RANSAC parameters missing");
	}
	
	if (vm.count("pose0")) {
		findPose0 = true;
		pose0Params = vm["pose0"].as<vector<double> >();
		if (pose0Params.size() < 4) RW_THROW("RANSAC parameters missing");
	}
	
	if (vm.count("slines")) {
		findSLines = true;
		slinesParams = vm["slines"].as<vector<double> >();
		if (slinesParams.size() < 4) RW_THROW("RANSAC parameters missing");
	}
	
	
	
	/* read data in */
	ifstream inFile(inFileName.c_str());
	vector<Transform3D<> > data = readData(inFile);
	inFile.close();
	
	// extract positions
	vector<Vector3D<> > pos;
	BOOST_FOREACH (const Transform3D<>& t, data) {
		pos.push_back(t.P());
		
		//cout << t.P() << endl;
	}
	
	// extract rotations
	vector<Rotation3D<> > rot;
	BOOST_FOREACH (const Transform3D<>& t, data) {
		rot.push_back(t.R());
	}
	
	//return 0;
	
	/* find models */
	// POINTS
	if (findPoints) {
		cout << "--- EXTRACTING POINT MODELS ---" << endl;
		vector<PointModel> models = PointModel::findModels(pos, (int)pointsParams[0], (int)pointsParams[1], pointsParams[2], pointsParams[3]);
		
		cout << "Models found (" << models.size() << "):" << endl;
		BOOST_FOREACH (const PointModel& m, models) {
			cout << " - Point: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
		}
		
		ofstream f("points.csv");
		savePoints(f, models);
		f.close();
		
		cout << endl;
	}
	
	// LINES
	if (findLines) {
		cout << "--- EXTRACTING LINE MODELS ---" << endl;
		vector<LineModel> models = LineModel::findModels(pos, (int)linesParams[0], (int)linesParams[1], linesParams[2], linesParams[3]);
		
		cout << "Models found (" << models.size() << "):" << endl;
		BOOST_FOREACH (const LineModel& m, models) {
			cout << " - Line: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
		}
		
		ofstream f("lines.csv");
		saveLines(f, models);
		f.close();
		
		cout << endl;
	}
	
	// PLANES
	if (findPlanes) {
		cout << "--- EXTRACTING PLANE MODELS ---" << endl;
		vector<PlaneModel> models = PlaneModel::findModels(pos, (int)planesParams[0], (int)planesParams[1], planesParams[2], planesParams[3]);
		
		cout << "Models found (" << models.size() << "):" << endl;
		BOOST_FOREACH (const PlaneModel& m, models) {
			cout << " - Plane: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
		}
		
		ofstream f("planes.csv");
		savePlanes(f, models);
		f.close();
		
		cout << endl;
	}
	
	
	
	// POSES
	if (findPoses) {
		cout << "--- EXTRACTING STABLE POSE MODELS (1 DOF) ---" << endl;
		vector<StablePose1DModel> models = StablePose1DModel::findModels(rot, (int)posesParams[0], (int)posesParams[1], posesParams[2], posesParams[3]);
		
		cout << "Models found (" << models.size() << "):" << endl;
		BOOST_FOREACH (const StablePose1DModel& m, models) {
			cout << " - StablePose1D: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
		}
		
		//ofstream f("planes.csv");
		//savePlanes(f, models);
		//f.close();
		
		// perform pose classification
		typedef std::pair<Transform3D<>, int> Classified;
		vector<Classified> classified;
		
		int idx = 0; // model index
		BOOST_FOREACH (const StablePose1DModel& m, models) {
			
			/*
			BOOST_FOREACH (const Transform3D<>& s, data) {
				if (m.belongsTo(s.R(), posesParams[2])) {
					classified.push_back(Classified(s, idx));
				}
			}
			/*/
			vector<size_t> indices = m.getInlierIndices();
			BOOST_FOREACH (size_t i, indices) {
				classified.push_back(Classified(data[i], idx));
			}
			//*/
			
			++idx;
		}
		
		// save classified poses
		ofstream f("poses.csv");
		BOOST_FOREACH (const Classified& s, classified) {
			Vector3D<> p = s.first.P();
			f << p[0] << ", " << p[1] << ", " << p[2] << ", ";
			
			RPY<> r(s.first.R());
			f << r[0] << ", " << r[1] << ", " << r[2] << ", ";
			
			f << s.second << endl;
		}
		f.close();
		
		cout << endl;
	}
	
	
	
	// POSE0
	if (findPose0) {
		cout << "--- EXTRACTING STABLE POSE MODELS (0 DOF) ---" << endl;
		vector<StablePose0DModel> models = StablePose0DModel::findModels(rot, (int)pose0Params[0], (int)pose0Params[1], pose0Params[2], pose0Params[3]);
		
		cout << "Models found (" << models.size() << "):" << endl;
		BOOST_FOREACH (const StablePose0DModel& m, models) {
			cout << " - StablePose0D: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
			cout << RPY<>(m.orientation()) << endl;
			
			StablePose0DModel refModel(RPY<>(0.1, 0.45, 0.0).toRotation3D());
			cout << refModel.same(m, pose0Params[3]) << endl;
		}
		
		//ofstream f("planes.csv");
		//savePlanes(f, models);
		//f.close();
		
		// perform pose classification
		typedef std::pair<Transform3D<>, int> Classified;
		vector<Classified> classified;
		
		int idx = 0; // model index
		BOOST_FOREACH (const StablePose0DModel& m, models) {
			
			/*
			BOOST_FOREACH (const Transform3D<>& s, data) {
				if (m.belongsTo(s.R(), posesParams[2])) {
					classified.push_back(Classified(s, idx));
				}
			}
			/*/
			vector<size_t> indices = m.getInlierIndices();
			BOOST_FOREACH (size_t i, indices) {
				classified.push_back(Classified(data[i], idx));
			}
			//*/
			
			++idx;
		}
		
		// save classified poses
		ofstream f("poses.csv");
		BOOST_FOREACH (const Classified& s, classified) {
			Vector3D<> p = s.first.P();
			f << p[0] << ", " << p[1] << ", " << p[2] << ", ";
			
			RPY<> r(s.first.R());
			f << r[0] << ", " << r[1] << ", " << r[2] << ", ";
			
			f << s.second << endl;
		}
		f.close();
		
		cout << endl;
	}
	
	
	
	// SLINES
	if (findSLines) {
		cout << "--- EXTRACTING STRUCTURED LINE MODELS ---" << endl;
		vector<StructuredLineModel> models = StructuredLineModel::findModels(pos, (int)slinesParams[0], (int)slinesParams[1], slinesParams[2], slinesParams[3]);
		
		cout << "Models found (" << models.size() << "):" << endl;
		BOOST_FOREACH (const StructuredLineModel& m, models) {
			cout << " - Structured line: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
		}
		
		ofstream f("slines.csv");
		saveSLines(f, models);
		f.close();
		
		cout << endl;
	}
	
	
	
	return 0;
}
