#!/usr/bin/python

# Example:
# ../scripts/easy_landscape_start.py --dir /home/awolniakowski/test/ --dwc /home/awolniakowski/grippers_acat/scenes/rotorcap/Scene.dwc.xml --td /home/awolniakowski/grippers_acat/scenes/rotorcap/task.td.xml --gripper /home/awolniakowski/grippers_acat/grippers/standard_gripper.grp.xml --grasps /home/awolniakowski/grippers_acat/grasps/rotorcap100_p10_a15.rwtask.xml -p cutdepth -b 0 0.025 --res 100 --threads 2 --alir 0.001 


import argparse
import numpy
import signal
import os
import sys
import xml.etree.ElementTree as ET


CMD = 'qsub /home/awolniakowski/scripts/evaluate_landscape.sh -V -v DIRECTORY={dir},DWC={dwc},TD={td},GRIPPER={gripper},GRASPS={grasps},SAVE={save},PARAM={param},VALUE={value},THREADS={threads},NGRASPS={ngrasps},RESULT={result},ALIR={alir},SEED={seed},FILTERS={filters}'


def signal_handler(signal, frame):
	print('Stopping')
	sys.exit(0)


signal.signal(signal.SIGINT, signal_handler)


def main():
	
	parser = argparse.ArgumentParser(description="Generates gripper landscape data")
	parser.add_argument("--parameter", "-p", metavar='PARAMETER', required=True, help="which parameter to landscape")
	parser.add_argument("--res", metavar='RES', default=100, help="landscape resolution")
	parser.set_defaults(save_grasps=False)
	args = parser.parse_args()
	
	PARAM = args.parameter
	RES = int(args.res)
	
	data = open(PARAM + '.csv', 'w')
	data.write(PARAM + ", success, alignment, coverage, wrench, stress, volume, qsum, qlog\n");
	
	for i in range(0, RES+1):
		print "Collecting " + PARAM + " = " + str(i)
		result = PARAM + '_' + str(i) + '.grp.xml'
		
		try:
			tree = ET.parse(result)
			root = tree.getroot()
			
			v = float(root.find('.//parameter[@name="{name}"]'.format(name = PARAM)).text)
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
		except (RuntimeError, IOError):
			continue
		
	data.close()


if __name__ == "__main__":
	main()
