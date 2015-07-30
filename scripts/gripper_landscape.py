#!/usr/bin/python


import argparse
import os
import subprocess
import xml.etree.ElementTree as ET
import numpy
import signal
import sys


# CONFIGURATION
EVALUATION_COMMAND = '${{GRIPPERZ_ROOT}}/bin/gripper_evaluate -c {cores} -g {simulations} -r {robustness} -v "{values}" --dwc {dwc} --td {td} --samples {samples} --name {name} 1>/dev/null 2>/dev/null'
P_NAMES = ['length', 'width', 'depth', 'chfdepth', 'chfangle', 'cutdepth', 'cutangle', 'tilt', 'tcp', 'jawdist', 'stroke', 'force']
P_RANGES = [
	(0, 0.2),
	(0, 0.05),
	(0, 0.05),
	(0, 1),
	(0, 90),
	(0, 0.05),
	(0, 180),
	(-180, 180),
	(0, 0.2),
	(0, 0.05),
	(0, 0.05),
	(0, 50)
]


def signal_handler(signal, frame):
	print('Stopping')
	sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)


def init():
	"""
	Parses command line and creates configuration data
	"""
	parser = argparse.ArgumentParser(description="Generates gripper landscape data")
	parser.add_argument("--scene", metavar='SCENE', required=True, help="which scene to use")
	parser.add_argument("--task", metavar='TASK', required=True, help="which task to use")
	parser.add_argument("-p", "--parameters", nargs='+', metavar='PARAMETERS', default=[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11], help="parameters to landscape")
	parser.add_argument("-i", "--seed", nargs='+', metavar='INITIAL', default=[0.1, 0.025, 0.02, 0.5, 45, 0.01, 90, 0, 0.025, 0, 0.05, 25], help="initial gripper configuration (12 values)")
	parser.add_argument("--res", metavar='RES', default=20, help="landscape resolution")
	parser.add_argument("-n", "--targets", metavar='TARGETS', default=100, help="number of simulations per evaluation")
	parser.add_argument("-r", "--robust", metavar='ROBUST', default=100, help="number of robustness simulations per evaluation")
	parser.add_argument("-c", "--cores", metavar='CORES', default=1, help="number of cores to use")

	args = parser.parse_args()
	
	return {
		'scene': args.scene,
		'task': args.task,
		'parameters': [int(p) for p in args.parameters],
		'seed': [float(v) for v in args.seed],
		'res': int(args.res),
		'targets': int(args.targets),
		'robust': int(args.robust),
		'cores': int(args.cores)
	}


def extract_quality(filename):
	"""
	Extracts quality values from evaluated gripper file
	"""
	
	tree = ET.parse(filename)
	root = tree.getroot()
	
	success = float(root.iter('success').next().text)
	robustness = float(root.iter('robustness').next().text)
	alignment = float(root.iter('alignment').next().text)
	coverage = float(root.iter('coverage').next().text)
	wrench = float(root.iter('wrench').next().text)
	stress = float(root.iter('stress').next().text)
	volume = float(root.iter('volume').next().text)
	qlog = float(root.iter('quality').next().text)
	
	qsum = (success + robustness + alignment + coverage + wrench + stress + volume) / 7.0
	
	return [success, robustness, alignment, coverage, wrench, stress, volume, qsum, qlog]


def evaluate(values, scene, task, targets, robust, cores, n=10):
	"""
	Evaluates gripper design given by provided parameters.
	Tries a number of times in case a segmentation fault occurs.
	"""
	
	cmd = EVALUATION_COMMAND.format(
		cores = cores,
		simulations = targets,
		robustness = robust,
		values = " ".join(str(v) for v in values),
		dwc = "${GRIPPERS_ROOT}/scenes/" + scene + "/Scene.dwc.xml",
		td = "${GRIPPERS_ROOT}/scenes/" + scene + "/task" + task + ".td.xml",
		samples = "${GRIPPERS_ROOT}/scenes/" + scene + "/samples" + task + ".xml",
		name = ".gripper"
	)
	
	quality = [0, 0, 0, 0, 0, 0, 0, 0]
	
	tries = 0
	while tries < n:
		ret = os.system(cmd)
		
		if (ret == 2): # ctrl - c
			sys.exit(0)
		
		if (ret == 0):
			quality = extract_quality(".gripper.grp.xml")
			break
		else:
			print "FAILED"
			tries += 1
	
	return quality
	

def main():
	args = init()
	
	for p in args['parameters']:
		name = P_NAMES[p]
		print "* Evaluating landscape for " + name
		
		data = open(name + '.csv', 'w')
		data.write("# " + name + ", success, robustness, alignment, coverage, wrench, stress, volume, qsum, qlog\n");
		
		bounds = P_RANGES[p]
		for v in numpy.linspace(bounds[0], bounds[1], args['res']+1, endpoint=True):
			print "Evaluating " + name + "= " + str(v)
			
			params = args['seed']
			params[p] = v
			
			quality = evaluate(params, args['scene'], args['task'], args['targets'], args['robust'], args['cores'])
			print str(v) + ", " + ", ".join(str(x) for x in quality)
			data.write(str(v) + ", " + ", ".join(str(x) for x in quality) + "\n")
			data.flush()
		
		data.close()
		


if __name__ == '__main__':
	main()
