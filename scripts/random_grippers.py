#!/usr/bin/python


import argparse
import os
import random


# parameter limits
PARAM_LIMITS = {
	'length': (0.075, 0.125),
	'width': (0.02, 0.03),
	'depth': (0.02, 0.03),
	'chfdepth': (0, 1),
	'chfangle': (0, 90),
	#'cutpos': (0.5 length, length),
	#'cutdepth': (0, width),
	'cutwidth': (0, 0.025),
	'cutangle': (0, 180),
	'cutangle1': (0, 90),
	'cutangle2': (0, 90),
	#'cutoffset': (-depth/2, depth/2),
	'cuttilt': (-90, 90),
	'cutdiameter': (0.01, 0.04)
}


def make_parameters():
	parameters = {
		's': 100,
		'length': random.uniform(*PARAM_LIMITS['length']),
		'width': random.uniform(*PARAM_LIMITS['width']),
		'depth': random.uniform(*PARAM_LIMITS['depth']),
		'chfdepth': random.uniform(*PARAM_LIMITS['chfdepth']),
		'chfangle': random.uniform(*PARAM_LIMITS['chfangle']),
		'cutwidth': random.uniform(*PARAM_LIMITS['cutwidth']),
		'cutangle': random.uniform(*PARAM_LIMITS['cutangle']),
		'cutangle1': random.uniform(*PARAM_LIMITS['cutangle1']),
		'cutangle2': random.uniform(*PARAM_LIMITS['cutangle2']),
		'cuttilt': random.uniform(*PARAM_LIMITS['cuttilt']),
		'cutdiameter': random.uniform(*PARAM_LIMITS['cutdiameter'])
	}
	
	parameters['cutpos'] = random.uniform(0.5*parameters['length'], parameters['length'])
	parameters['cutdepth'] = random.uniform(0, parameters['width']-0.005)
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
