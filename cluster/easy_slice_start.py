#!/usr/bin/python

# Example:
# ../scripts/easy_landscape_start.py --dir /home/awolniakowski/test/ --dwc /home/awolniakowski/grippers_acat/scenes/rotorcap/Scene.dwc.xml --td /home/awolniakowski/grippers_acat/scenes/rotorcap/task.td.xml --gripper /home/awolniakowski/grippers_acat/grippers/standard_gripper.grp.xml --grasps /home/awolniakowski/grippers_acat/grasps/rotorcap100_p10_a15.rwtask.xml -p cutdepth -b 0 0.025 --res 100 --threads 2 --alir 0.001 


import argparse
import numpy
import signal
import os
import sys
import xml.etree.ElementTree as ET


CMD = 'qsub /home/awolniakowski/scripts/evaluate_slice.sh -V -v DIRECTORY={dir},DWC={dwc},TD={td},GRIPPER={gripper},GRASPS={grasps},SAVE={save},PARAM1={param1},PARAM2={param2},VALUE={value},THREADS={threads},NGRASPS={ngrasps},RESULT={result},ALIR={alir},SEED={seed},FILTERS={filters}'


def signal_handler(signal, frame):
	print('Stopping')
	sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


def main():
	
	parser = argparse.ArgumentParser(description="Generates gripper slice data")
	parser.add_argument("--dir", metavar='DIR', required=True, help="landscape directory")
	parser.add_argument("--dwc", metavar='DWC', required=True, help="scene dwc file")
	parser.add_argument("--td", metavar='TD', required=True, help="scene td file")
	parser.add_argument("--gripper", metavar='GRP', required=True, help="gripper xml file")
	parser.add_argument("--grasps", metavar='RWTASK', default='', help="grasps in RW xml format")
	parser.add_argument("--filters", metavar='FILTERS', default='zzz', help="grasp filters to apply")
	#parser.add_argument("--save_grasps", dest='save_grasps', action='store_true', help="save grasps in RW xml format?")
	parser.add_argument("--p1", metavar='PARAMETER1', required=True, help="which parameter to slice (1)")
	parser.add_argument("--p2", metavar='PARAMETER2', required=True, help="which parameter to slice (2)")
	parser.add_argument("--res1", metavar='RES1', default=10, help="slice resolution (1)")
	parser.add_argument("--res2", metavar='RES2', default=10, help="slice resolution (2)")
	parser.add_argument("--ngrasps", "-n", metavar='NGRASPS', default=100, help="# of grasps per evaluation")
	parser.add_argument("--threads", "-t", metavar='THREADS', default=4, help="# of threads to use")
	parser.add_argument("--b1", metavar='BOUNDS1', nargs=2, required=True, help="parameter bounds (1)")
	parser.add_argument("--b2", metavar='BOUNDS2', nargs=2, required=True, help="parameter bounds (2)")
	parser.add_argument("--alir", metavar='ALIR', default=0.01, help="alignment filtering radius")
	parser.set_defaults(save_grasps=False)
	args = parser.parse_args()
	
	DIR = args.dir
	DWC = args.dwc
	TD = args.td
	GRIPPER = args.gripper
	GRASPS = args.grasps
	PARAM1 = args.p1
	PARAM2 = args.p2
	RES1 = int(args.res1)
	RES2 = int(args.res2)
	NGRASPS = int(args.ngrasps)
	THREADS = int(args.threads)
	BOUNDS1 = [float(b) for b in args.b1]
	BOUNDS2 = [float(b) for b in args.b2]
	ALIR = float(args.alir)
	#SAVE = args.save_grasps
	FILTERS = args.filters
	
	i = 0
	k = 0
	for v1 in numpy.linspace(BOUNDS1[0], BOUNDS1[1], RES1+1, endpoint=True):
		
		j = 0
		for v2 in numpy.linspace(BOUNDS2[0], BOUNDS2[1], RES2+1, endpoint=True):
			print "Evaluating " + PARAM1 + "=" + str(v1) + " " + PARAM2 + "=" + str(v2)
			result = PARAM1 + '_' + str(i) + '_' + PARAM2 + '_' + str(j) + '.grp.xml'
			tasks = PARAM1 + '_' + str(i) + '_' + PARAM2 + '_' + str(j) + '.rwtask.xml'
			
			# construct command
			cmd = CMD.format(
				dir = DIR,
				dwc = DWC,
				td = TD,
				gripper = GRIPPER,
				param1 = PARAM1,
				param2 = PARAM2,
				value = '"{} {}"'.format(v1, v2),
				threads = THREADS,
				grasps = GRASPS,
				save = tasks,
				ngrasps = NGRASPS,
				result = result,
				alir = ALIR,
				seed = k,
				filters = FILTERS
			)
			
			print cmd
			
			# evaluate
			ret = os.system(cmd)
			
			if (ret == 2): # ctrl - c
				sys.exit(0)
			
			j += 1
			k += 1
		
		i += 1


if __name__ == "__main__":
	main()
