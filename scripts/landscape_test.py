#!/usr/bin/python


import numpy
import signal
import os
import sys
import xml.etree.ElementTree as ET


CMD = '/home/dagothar/gripperz-dev/bin/evaluate_gripper --dwc {dwc} --td {td} --gripper {gripper} -p {parameter} -v "{value}" -t {threads} -n {grasps} -r {result}'
DWC = "/home/dagothar/grippers/scenes/rotor/Scene.dwc.xml"
TD = "/home/dagothar/grippers/scenes/rotor/task1.td.xml"
GRIPPER = "/home/dagothar/gripperz-dev/data/test/standard_gripper.xml"
PARAM = "length"
BOUNDS = (0.0, 0.1)
RESOLUTION = 100
GRASPS = 100
THREADS = 4


def signal_handler(signal, frame):
	print('Stopping')
	sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


def main():
	
	data = open(PARAM + '.csv', 'w')
	data.write(PARAM + ", success, alignment, coverage, wrench, stress, volume, qsum, qlog\n");
	
	i = 0
	for v in numpy.linspace(BOUNDS[0], BOUNDS[1], RESOLUTION+1, endpoint=True):
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
			grasps = GRASPS,
			result = result
		)
		
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
