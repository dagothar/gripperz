#!/usr/bin/python

import os
import sys


CMD = 'qsub ../scripts/evaluate_gripper.sh -V -v DWC=/home/awolniakowski/grippers/scenes/rotor/Scene.dwc.xml,TD=/home/awolniakowski/grippers/scenes/rotor/task3.td.xml,GRIPPER=/home/awolniakowski/gripperz/data/test/standard_gripper.xml,RESULT={result},GRASPS=/home/awolniakowski/grippers/real_experiments/grasp_database/the_grasp.rwtask.xml,NGRASPS=100,VALUE={value}'


def main():

	for i in range(1, 100):
		print "# Job " + str(i)
		result = 'result_{i}.grp.xml'.format(i = i)
		
		# construct command
		cmd = CMD.format(
			result = result,
			value = 0.0
		)
		
		print cmd
		
		# evaluate
		ret = os.system(cmd)


if __name__ == "__main__":
	main()
