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


bool sortf(const PointModel& m1, const PointModel& m2) {
	//return m1.getQuality() > m2.getQuality();
	return m1.getNumberOfInliers() < m2.getNumberOfInliers();
}



int main(int argc, char* argv[]) {
	/* initialization */
	Math::seed();
	RobWork::getInstance()->initialize();
	
	string inFileName;
	string outFileName;
	
	bool findPoints = false;

	vector<double> pointsParams;

	/* parse command line */
	string usage = "This is a script used for constraints code testing.";
	
	// switch options
	options_description opt("Options");
	opt.add_options()
		("help,h", "help message")
		("infile,", value<string>(&inFileName)->required(), "CSV file with Transform3Ds")
		("points,", value<vector<double> >()->multitoken(), "find models for points; iterations, mindata, datathr and modelthr should follow")
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
	
	
	/* read data in */
	ifstream inFile(inFileName.c_str());
	vector<Transform3D<> > data = readData(inFile);
	inFile.close();
	
	// extract positions
	vector<Vector3D<> > pos;
	BOOST_FOREACH (const Transform3D<>& t, data) {
		pos.push_back(t.P());
	}
	
	// extract rotations
	vector<Rotation3D<> > rot;
	BOOST_FOREACH (const Transform3D<>& t, data) {
		rot.push_back(t.R());
	}
	
	// extract versors
	vector<Vector3D<> > xs, ys, zs;
	BOOST_FOREACH (const Transform3D<>& t, data) {
		xs.push_back(t.R() * Vector3D<>::x());
		ys.push_back(t.R() * Vector3D<>::y());
		zs.push_back(t.R() * Vector3D<>::z());
	}
	
	/* find models */
	cout << "--- EXTRACTING POINT MODELS ---" << endl;
	vector<PointModel> xmodels = PointModel::findModels(xs, (int)pointsParams[0], (int)pointsParams[1], pointsParams[2], pointsParams[3]);
	vector<PointModel> ymodels = PointModel::findModels(ys, (int)pointsParams[0], (int)pointsParams[1], pointsParams[2], pointsParams[3]);
	vector<PointModel> zmodels = PointModel::findModels(zs, (int)pointsParams[0], (int)pointsParams[1], pointsParams[2], pointsParams[3]);
		
	sort(xmodels.begin(), xmodels.end(), sortf);
	reverse(xmodels.begin(), xmodels.end());
	sort(ymodels.begin(), ymodels.end(), sortf);
	reverse(ymodels.begin(), ymodels.end());
	sort(zmodels.begin(), zmodels.end(), sortf);
	reverse(zmodels.begin(), zmodels.end());
	
	cout << "Found " << xmodels.size() << " X models." << endl;
	cout << "Found " << ymodels.size() << " Y models." << endl;
	cout << "Found " << zmodels.size() << " Z models." << endl;
	
	BOOST_FOREACH (const PointModel& m, xmodels) {
		cout << " X: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
	}
	
	BOOST_FOREACH (const PointModel& m, ymodels) {
		cout << " Y: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
	}
	
	BOOST_FOREACH (const PointModel& m, zmodels) {
		cout << " Z: " << m << ", QUALITY: " << m.getQuality() << ", INLIERS: " << m.getNumberOfInliers() << endl;
	}
	
	ofstream fx("xpoints.csv");
	ofstream fy("ypoints.csv");
	ofstream fz("zpoints.csv");
	savePoints(fx, xmodels);
	savePoints(fy, ymodels);
	savePoints(fz, zmodels);
	fx.close();
	fy.close();
	fz.close();
	
	cout << endl;
	
	
	return 0;
}
