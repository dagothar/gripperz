/**
 * @file extract_poses.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-16
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <rw/rw.hpp>
#include <rw/RobWork.hpp>
#include <rwlibs/task/GraspTask.hpp>
#include <evaluation/VersorAlignment.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>


using namespace std;
USE_ROBWORK_NAMESPACE
using namespace robwork;
using namespace rwlibs::task;
using namespace gripperz::evaluation;
using namespace boost::program_options;
namespace po = boost::program_options;


int main(int argc, char* argv[]) {
	Math::seed();
	RobWork::getInstance()->initialize();
	Log::log().setLevel(Log::Debug);
	
	string inFileName;
	vector<double> params;
	
	/* parse command line */
	string usage = "This is a script used for constraints code testing.";
	
	// switch options
	options_description opt("Options");
	opt.add_options()
		("help,h", "help message")
		("infile,", value<string>(&inFileName)->required(), "tasks file")
		("params,", value<vector<double> >(&params)->multitoken()->required(), "RANSAC parameters")
	;
	
	// positional parameters
	positional_options_description pos_opt;
	pos_opt.add("infile", 1);
	
	variables_map vm;
	store(command_line_parser(argc, argv).options(opt).positional(pos_opt).run(), vm);
	notify(vm);
		
	if (vm.count("help")) {
		cout << usage << endl;
		cout << opt << endl;
		return 0;
	}
		
	if (params.size() < 4) RW_THROW("RANSAC parameters missing");
	
	/*
	 * Load tasks.
	 */
	cout << "Loading tasks from: " << inFileName << endl;
	GraspTask::Ptr tasks = GraspTask::load(inFileName);
	
	/*
	 * Test alignment calculator.
	 */
	VersorAlignment::Configuration conf;
	conf.iterations = (int) params[0];
	conf.minInliers = (int) params[1];
	conf.dataThreshold = params[2];
	conf.modelThreshold = params[3];
	AlignmentCalculator::Ptr calc = ownedPtr(new VersorAlignment(conf));
	
	double alignment = calc->calculateAlignment(tasks);
	
	cout << "ALIGNMENT = " << alignment << endl;
	
	return 0;
}
