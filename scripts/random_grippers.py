#!/usr/bin/python


import argparse
import os
import random


# parameter limits
PARAM_LIMITS = {
	'length': (0, 0.2),
	'width': (0, 0.05),
	'depth': (0, 0.05),
	'chfdepth': (0, 1),
	'chfangle': (0, 90),
	#'cutpos': (0, length),
	#'cutdepth': (0, width),
	'cutangle': (0, 180),
	#'cutoffset': (-depth/2, depth/2),
	'cuttilt': (-90, 90),
	'cutdiameter': (0, 0.05)
}


def make_parameters():
	parameters = {
		'length': random.uniform(*PARAM_LIMITS['length']),
		'width': random.uniform(*PARAM_LIMITS['width']),
		'depth': random.uniform(*PARAM_LIMITS['depth']),
		'chfdepth': random.uniform(*PARAM_LIMITS['chfdepth']),
		'chfangle': random.uniform(*PARAM_LIMITS['chfangle']),
		'cutangle': random.uniform(*PARAM_LIMITS['cutangle']),
		'cuttilt': random.uniform(*PARAM_LIMITS['cuttilt']),
		'cutdiameter': random.uniform(*PARAM_LIMITS['cutdiameter'])
	}
	
	parameters['cutpos'] = random.uniform(0, parameters['length'])
	parameters['cutdepth'] = random.uniform(0, parameters['width'])
	parameters['cutoffset'] = random.uniform(-parameters['depth']/2, parameters['depth']/2)
	
	return parameters


def make_finger(name, parameters, script):
	cmd = 'openscad {} -o {}'.format(script, name)
	
	for k, v in parameters.items():
		cmd = cmd + " -D '{}={};'".format(k, v)
	
	os.system(cmd)


def main():
	parser = argparse.ArgumentParser(description="Generates random finger STL files")
	parser.add_argument("--name", metavar='NAME', required=True, help="base finger name")
	parser.add_argument("-n", "--number", metavar='NUMBER', required=True, help="number of designs to generate")
	parser.add_argument("-t", "--template", metavar='TEMPLATE', required=True, help="scad script")
	
	args = parser.parse_args()
	
	name = args.name
	number = int(args.number)
	template = args.template
	
	for i in range(0, number):
		finger_name = '{}_{}.stl'.format(name, i)
		params = make_parameters()
		
		print 'Generating finger ' + finger_name + '...'
		make_finger(finger_name, params, template)
		

if __name__ == '__main__':
	main()
