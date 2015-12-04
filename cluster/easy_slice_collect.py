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
	parser.add_argument("--p1", metavar='PARAMETER1', required=True, help="which parameter to collect (1)")
	parser.add_argument("--p2", metavar='PARAMETER2', required=True, help="which parameter to collect (2)")
	parser.add_argument("--res1", metavar='RES1', default=10, help="slice resolution (1)")
	parser.add_argument("--res2", metavar='RES2', default=10, help="slice resolution (2)")
	args = parser.parse_args()
	
	PARAM1 = args.p1
	PARAM2 = args.p2
	RES1 = int(args.res1)
	RES2 = int(args.res2)
	
	data = open(PARAM1 + '_' + PARAM2 + '.csv', 'w')
	data.write("x, y, success, alignment, coverage, wrench, stress, volume, qsum, qlog\n");
	
	for i in range(0, RES1+1):
		for j in range(0, RES2+1):
			print "Collecting " + PARAM1 + "=" + str(i) + ' ' + PARAM2 + '=' + str(j)
			result = PARAM1 + '_' + str(i) + '_' + PARAM2 + '_' + str(j) + '.grp.xml'
			
			try:
				tree = ET.parse(result)
				root = tree.getroot()
				
				v1 = float(root.find('.//parameter[@name="{name}"]'.format(name = PARAM1)).text)
				v2 = float(root.find('.//parameter[@name="{name}"]'.format(name = PARAM2)).text)
				success = float(root.find('.//index[@name="success"]').text)
				alignment = float(root.find('.//index[@name="alignment"]').text)
				coverage = float(root.find('.//index[@name="coverage"]').text)
				wrench = float(root.find('.//index[@name="wrench"]').text)
				stress = float(root.find('.//index[@name="stress"]').text)
				volume = float(root.find('.//index[@name="volume"]').text)
				qlog = float(root.find('.//index[@name="Q"]').text)
				qsum = (success + alignment + coverage + wrench + stress + volume) / 6.0
				
				record = [v1, v2, success, alignment, coverage, wrench, stress, volume, qsum, qlog]
				print record
				
				data.write(", ".join(str(x) for x in record) + "\n")
				data.flush()
			except (RuntimeError, IOError):
				continue
		
	data.close()


if __name__ == "__main__":
	main()
