#!/usr/bin/python

# Example:
# ../scripts/easy_landscape_start.py --dir /home/awolniakowski/test/ --dwc /home/awolniakowski/grippers_acat/scenes/rotorcap/Scene.dwc.xml --td /home/awolniakowski/grippers_acat/scenes/rotorcap/task.td.xml --gripper /home/awolniakowski/grippers_acat/grippers/standard_gripper.grp.xml --grasps /home/awolniakowski/grippers_acat/grasps/rotorcap100_p10_a15.rwtask.xml -p cutdepth -b 0 0.025 --res 100 --threads 2 --alir 0.001 


import argparse
import numpy
import signal
import os
import sys
import xml.etree.ElementTree as ET


CMD = 'qsub /home/awolniakowski/scripts/evaluate_landscape.sh -V -v DIRECTORY={dir},DWC={dwc},TD={td},GRIPPER={gripper},GRASPS={grasps},SAVE={save},PARAM={param},VALUE={value},THREADS={threads},NGRASPS={ngrasps},RESULT={result},SEED={seed},FILTERS={filters}'


def signal_handler(signal, frame):
	print('Stopping')
	sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


def main():
	
	parser = argparse.ArgumentParser(description="Generates gripper landscape data")
	parser.add_argument("--dir", metavar='DIR', required=True, help="landscape directory")
	parser.add_argument("--dwc", metavar='DWC', required=True, help="scene dwc file")
	parser.add_argument("--td", metavar='TD', required=True, help="scene td file")
	parser.add_argument("--gripper", metavar='GRP', required=True, help="gripper xml file")
	parser.add_argument("--grasps", metavar='RWTASK', default='', help="grasps in RW xml format")
	parser.add_argument("--filters", metavar='FILTERS', default='zzz', help="grasp filters to apply")
	#parser.add_argument("--save_grasps", dest='save_grasps', action='store_true', help="save grasps in RW xml format?")
	parser.add_argument("--parameter", "-p", metavar='PARAMETER', required=True, help="which parameter to landscape")
	parser.add_argument("--res", metavar='RES', default=100, help="landscape resolution")
	parser.add_argument("--ngrasps", "-n", metavar='NGRASPS', default=100, help="# of grasps per evaluation")
	parser.add_argument("--threads", "-t", metavar='THREADS', default=4, help="# of threads to use")
	parser.add_argument("--bounds", "-b", metavar='BOUNDS', nargs=2, required=True, help="parameter bounds")
	#parser.add_argument("--alir", metavar='ALIR', default=0.01, help="alignment filtering radius")
	parser.set_defaults(save_grasps=False)
	args = parser.parse_args()
	
	DIR = args.dir
	DWC = args.dwc
	TD = args.td
	GRIPPER = args.gripper
	GRASPS = args.grasps
	PARAM = args.parameter
	RES = int(args.res)
	NGRASPS = int(args.ngrasps)
	THREADS = int(args.threads)
	BOUNDS = [float(b) for b in args.bounds]
	#ALIR = float(args.alir)
	#SAVE = args.save_grasps
	FILTERS = args.filters
	
	i = 0
	for v in numpy.linspace(BOUNDS[0], BOUNDS[1], RES+1, endpoint=True):
		print "Evaluating " + PARAM + " = " + str(v)
		result = PARAM + '_' + str(i) + '.grp.xml'
		tasks = PARAM + '_' + str(i) + '.rwtask.xml'
		
		# construct command
		cmd = CMD.format(
			dir = DIR,
			dwc = DWC,
			td = TD,
			gripper = GRIPPER,
			param = PARAM,
			value = str(v),
			threads = THREADS,
			grasps = GRASPS,
			save = tasks,
			ngrasps = NGRASPS,
			result = result,
			#alir = ALIR,
			seed = i,
			filters = FILTERS
		)
		
		print cmd
		
		# evaluate
		ret = os.system(cmd)
		
		if (ret == 2): # ctrl - c
			sys.exit(0)
		
		i += 1


if __name__ == "__main__":
	main()
