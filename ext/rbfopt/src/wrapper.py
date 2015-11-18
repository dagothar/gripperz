#!/usr/bin/python


import rbfopt
from rbfopt_settings import RbfSettings


class Proxy:
	"""Provides the evaluation function for the RBF optimizer
	that is not related to the ObjectiveFuntion class"""
	
	def __init__(self, objective, callback):
		self.objective = objective
		self.callback = callback
	
	def evaluate(self, x):
		return self.callback(self.objective, x)


def optimize(objective, callback, dimensions, max_iter, max_fev):
	"""Wrapper function for RBF optimization"""
	
	#print "RBF dimensions= ", dimensions
	#print "RBF max_iter= ", max_iter
	#print "RBF max_fev= ", max_fev
	
	proxy = Proxy(objective, callback)
	
	settings = RbfSettings()
	settings.max_iterations = max_iter
	settings.max_evaluations = max_fev

	return rbfopt.rbf_optimize(settings, dimensions, [0] * dimensions, [1] * dimensions, proxy.evaluate)
