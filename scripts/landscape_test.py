#!/usr/bin/python


import argparse
import numpy
import signal
import os
import sys
import xml.etree.ElementTree as ET


CMD = '/home/dagothar/gripperz-dev/bin/evaluate_gripper --dwc {dwc} --td {td} --gripper {gripper} -p {parameter} -v "{value}" -t {threads} -n {ngrasps} -r {result}'
DWC = "/home/dagothar/grippers/scenes/rotor/Scene.dwc.xml"
TD = "/home/dagothar/grippers/scenes/rotor/task3.td.xml"
GRIPPER = "/home/dagothar/gripperz-dev/data/test/trapezoid_cutout_gripper.xml"
GRASPS = "/home/dagothar/gripperz-dev/data/tasks/the_grasp.rwtask.xml"
PARAM = "cutdepth"
BOUNDS = (0.0, 0.025)
RES = 100
NGRASPS = 100
THREADS = 4


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
	parser.add_argument("--parameter", "-p", metavar='PARAMETER', required=True, help="which parameter to landscape")
	parser.add_argument("--res", metavar='RES', default=100, help="landscape resolution")
	parser.add_argument("--ngrasps", "-n", metavar='NGRASPS', default=100, help="# of grasps per evaluation")
	parser.add_argument("--threads", "-t", metavar='THREADS', default=4, help="# of threads to use")
	parser.add_argument("--bounds", "-b", metavar='BOUNDS', nargs=2, required=True, help="parameter bounds")
	args = parser.parse_args()
	
	DWC = args.dwc
	TD = args.td
	GRIPPER = args.gripper
	GRASPS = args.grasps
	PARAM = args.parameter
	RES = int(args.res)
	NGRASPS = int(args.ngrasps)
	BOUNDS = [float(b) for b in args.bounds]
		
	data = open(PARAM + '.csv', 'w')
	data.write(PARAM + ", success, alignment, coverage, wrench, stress, volume, qsum, qlog\n");
	
	i = 0
	for v in numpy.linspace(BOUNDS[0], BOUNDS[1], RES+1, endpoint=True):
		print "Evaluating " + PARAM + " = " + str(v)
		result = PARAM + '_' + str(i) + '.xml'
		
		# construct command
		cmd = CMD.format(
			dwc = DWC,
			td = TD,
			gripper = GRIPPER,
			parameter = PARAM,
			value = str(v),
			threads = THREADS,
			ngrasps = NGRASPS,
			result = result
		)
		
		if (TD):
			cmd += ' --grasps {grasps}'.format(grasps = GRASPS)
		
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
