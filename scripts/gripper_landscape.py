#!/usr/bin/python

import argparse

# CONFIGURATION
N_SIMULATIONS = 100
N_ROBUSTNESS = 100
N_CORES = 4


def init():
	"""
	Parses command line and creates configuration data
	"""
	parser = argparse.ArgumentParser(description="Generates gripper landscape data")
	parser.add_argument("-s", "--scene", metavar='SCENE', help="which scene to use")
	parser.add_argument("-t", "--task", metavar='TASK', help="which task to use")

	args = parser.parse_args()
	
	return {
		
	}


def main():
	init()


if __name__ == '__main__':
	main()
