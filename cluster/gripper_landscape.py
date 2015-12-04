#!/usr/bin/python

# Example:
#	./gripper_landscape.py --dwc /home/dothar/grippers/scenes/rotor/Scene.dwc.xml --td /home/dagothar/grippers/scenes/rotor/task3.td.xml --gripper ~/gripperz-dev/data/test/standard_gripper.xml -p cutdepth --grasps ~/gripperz-dev/data/tasks/the_grasp.rwtask.xml --res 100 --ngrasps 100 --bounds 0.0 0.025 --alir 0.001 --seed 0


import argparse
import numpy
import signal
import os
import sys
import xml.etree.ElementTree as ET


CMD = '/home/dagothar/gripperz-dev/bin/evaluate_gripper --dwc {dwc} --td {td} --gripper {gripper} -p {parameter} -v "{value}" -t {threads} -n {ngrasps} -r {result} --aliR {alir} --seed {seed}'


def signal_handler(signal, frame):
	print('Stopping')
	sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


def main():
	
	parser = argparse.ArgumentParser(description="Generates gripper landscape data")
	parser.add_argument("--dwc", metavar='DWC', required=True, help="scene dwc file")
	parser.add_argument("--td", metavar='TD', required=True, help="scene td file")
	parser.add_argument("--gripper", metavar='GRP', required=True, help="gripper xml file")
	parser.add_argument("--grasps", metavar='RWTASK', default='', help="grasps in RW xml format")
	parser.add_argument("--save_grasps", dest='save_grasps', action='store_true', help="save grasps in RW xml format?")
	parser.add_argument("--parameter", "-p", metavar='PARAMETER', required=True, help="which parameter to landscape")
	parser.add_argument("--res", metavar='RES', default=100, help="landscape resolution")
	parser.add_argument("--ngrasps", "-n", metavar='NGRASPS', default=100, help="# of grasps per evaluation")
	parser.add_argument("--threads", "-t", metavar='THREADS', default=4, help="# of threads to use")
	parser.add_argument("--bounds", "-b", metavar='BOUNDS', nargs=2, required=True, help="parameter bounds")
	parser.add_argument("--alir", metavar='ALIR', default=0.01, help="alignment filtering radius")
	parser.add_argument("--seed", metavar='SEED', default=0, help="RNG seed")
	parser.set_defaults(save_grasps=False)
	args = parser.parse_args()
	
	DWC = args.dwc
	TD = args.td
	GRIPPER = args.gripper
	GRASPS = args.grasps
	PARAM = args.parameter
	RES = int(args.res)
	NGRASPS = int(args.ngrasps)
	THREADS = int(args.threads)
	BOUNDS = [float(b) for b in args.bounds]
	ALIR = float(args.alir)
	SEED = int(args.seed)
	SAVE = args.save_grasps
		
	data = open(PARAM + '.csv', 'w')
	data.write(PARAM + ", success, alignment, coverage, wrench, stress, volume, qsum, qlog\n");
	
	i = 0
	for v in numpy.linspace(BOUNDS[0], BOUNDS[1], RES+1, endpoint=True):
		print "Evaluating " + PARAM + " = " + str(v)
		result = PARAM + '_' + str(i) + '.grp.xml'
		tasks = PARAM + '_' + str(i) + '.rwtask.xml'
		
		# construct command
		cmd = CMD.format(
			dwc = DWC,
			td = TD,
			gripper = GRIPPER,
			parameter = PARAM,
			value = str(v),
			threads = THREADS,
			ngrasps = NGRASPS,
			result = result,
			alir = ALIR,
			seed = SEED
		)
		
		if (TD):
			cmd += ' --grasps {grasps}'.format(grasps = GRASPS)
			
		if (SAVE):
			cmd += ' --save_grasps {tasks}'.format(tasks = tasks)
		
		# evaluate
		ret = os.system(cmd)
		
		if (ret == 2): # ctrl - c
			sys.exit(0)
			
		if (ret != 0):
			continue
		
		# parse quality
		tree = ET.parse(result)
		root = tree.getroot()
		
		success = float(root.find('.//index[@name="success"]').text)
		alignment = float(root.find('.//index[@name="alignment"]').text)
		coverage = float(root.find('.//index[@name="coverage"]').text)
		wrench = float(root.find('.//index[@name="wrench"]').text)
		stress = float(root.find('.//index[@name="stress"]').text)
		volume = float(root.find('.//index[@name="volume"]').text)
		qlog = float(root.find('.//index[@name="Q"]').text)
		qsum = (success + alignment + coverage + wrench + stress + volume) / 6.0
		
		record = [v, success, alignment, coverage, wrench, stress, volume, qsum, qlog]
		print record
		
		data.write(", ".join(str(x) for x in record) + "\n")
		data.flush()
		
		i += 1
		
	data.close()


if __name__ == "__main__":
	main()
