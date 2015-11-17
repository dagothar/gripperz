"""Utility functions.

This module contains a number of subroutines that are used by the
other modules.

Licensed under Revised BSD license, see LICENSE.
(C) Copyright Singapore University of Technology and Design 2014.
Research partially supported by SUTD-MIT International Design Center.
"""

from __future__ import print_function
from __future__ import unicode_literals
from __future__ import division
from __future__ import absolute_import

import sys
import math
import random
import itertools
import numpy as np
import rbfopt_config as config
import rbfopt_aux_problems as aux
import pyDOE
from rbfopt_settings import RbfSettings

def get_rbf_function(settings):
    """Return a radial basis function.

    Return the radial basis function appropriate function as indicated
    by the settings.
    
    Parameters
    ----------
    
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.
    
    Returns
    ---
    Callable[float]
        A callable radial basis function.
    """
    assert(isinstance(settings, RbfSettings))
    if (settings.rbf == 'cubic'):
        return _cubic
    elif (settings.rbf == 'thin_plate_spline'):
        return _thin_plate_spline
    elif (settings.rbf == 'linear'):
        return _linear
    elif (settings.rbf == 'multiquadric'):
        return _multiquadric

# -- List of radial basis functions
def _cubic(r):
    """Cubic RBF: :math: `f(x) = x^3`"""
    assert(r >= 0)
    return r*r*r

def _thin_plate_spline(r):
    """Thin plate spline RBF: :math: `f(x) = x^2 \log x`"""
    assert(r >= 0)
    if (r == 0.0):
        return 0.0
    return math.log(r)*r*r

def _linear(r):
    """Linear RBF: :math: `f(x) = x`"""
    assert(r >= 0)
    return r

def _multiquadric(r):
    """Multiquadric RBF: :math: `f(x) = \sqrt{x^2 + \gamma^2}`"""
    assert(r >= 0)
    return math.sqrt(r*r + config.GAMMA*config.GAMMA)
# -- end list of radial basis functions

def get_degree_polynomial(settings):
    """Compute the degree of the polynomial for the interpolant.

    Return the degree of the polynomial that should be used in the RBF
    expression to ensure unisolvence and convergence of the
    optimization method.

    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.

    Returns
    -------
    int
        Degree of the polynomial
    """
    assert(isinstance(settings, RbfSettings))
    if (settings.rbf == 'cubic' or settings.rbf == 'thin_plate_spline'):
        return 1
    elif (settings.rbf == 'linear' or settings.rbf == 'multiquadric'):
        return 0
    else:
        return -1

# -- end function

def get_size_P_matrix(settings, n):
    """Compute size of the P part of the RBF matrix.

    Return the number of columns in the P part of the matrix [\Phi P;
    P^T 0] that is used through the algorithm.

    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.

    n : int
        Dimension of the problem, i.e. number of variables.

    Returns
    -------
    int
        Number of columns in the matrix
    """
    assert(isinstance(settings, RbfSettings))
    if (settings.rbf == 'cubic' or settings.rbf == 'thin_plate_spline'):
        return n+1
    elif (settings.rbf == 'linear' or settings.rbf == 'multiquadric'):
        return 1
    else:
        return 0

# -- end function

def get_all_corners(var_lower, var_upper):
    """Compute all corner points of a box.

    Compute and return all the corner points of the given box. Note
    that this number is exponential in the dimension of the problem.

    Parameters
    ----------
    var_lower : List[float]
        List of lower bounds of the variables.
    var_upper : List[float]
        List of upper bounds of the variables.

    Returns
    -------
    List[List[float]]
        All the corner points.
    """
    assert(len(var_lower)==len(var_upper))

    node_pos = list()
    # Generate all corners
    for corner in itertools.product('lu', repeat=len(var_lower)):
        point = list()
        for (i, bound) in enumerate(corner):
            if (bound == 'l'):
                point.append(var_lower[i])
            else:
                point.append(var_upper[i])
        node_pos.append(point)

    return node_pos

# -- end function

def get_lower_corners(var_lower, var_upper):
    """Compute the lower corner points of a box.

    Compute a list of (n+1) corner points of the given box, where n is
    the dimension of the space. The selected points are the bottom
    left (i.e. corresponding to the origin in the 0-1 hypercube) and
    the n adjacent ones.

    Parameters
    ----------
    var_lower : List[float]
        List of lower bounds of the variables.
    var_upper : List[float]
        List of upper bounds of the variables.

    Returns
    -------
    List[List[float]]
        The lower corner points.
    """
    assert(len(var_lower)==len(var_upper))

    # Make sure we copy the lists instead of copying just a reference
    node_pos = [ list(var_lower) ]
    # Generate adjacent corners
    for i in range(len(var_lower)):
        corner = list(var_lower)
        corner[i] = var_upper[i]
        node_pos.append(corner)

    return node_pos

# -- end function

def get_random_corners(var_lower, var_upper):
    """Compute some randomly selected corner points of the box.

    Compute a list of (n+1) corner points of the given box, where n is
    the dimension of the space. The selected points are picked
    randomly.

    Parameters
    ----------
    var_lower : List[float]
        List of lower bounds of the variables.
    var_upper : List[float]
        List of upper bounds of the variables.

    Returns
    -------
    List[List[float]]
        A list of random corner points.
    """
    assert(len(var_lower)==len(var_upper))

    n = len(var_lower)
    node_pos = list()
    while (len(node_pos) < n+1):
        point = [var_lower[i] if (random.random() <= 0.5) else var_upper[i]
                 for i in range(n)]
        if (get_min_distance(point, node_pos) > 0):
            node_pos.append(point)

    return node_pos
    
# -- end function

def get_lhd_maximin_points(var_lower, var_upper):
    """Compute a latin hypercube design with maximin distance.

    Compute a list of (n+1) points in the given box, where n is the
    dimension of the space. The selected points are picked according
    to a random latin hypercube design with maximin distance
    criterion. This function relies on the library pyDOE.

    Parameters
    ----------
    var_lower : List[float]
        List of lower bounds of the variables.
    var_upper : List[float]
        List of upper bounds of the variables.

    Returns
    -------
    List[List[float]]
        List of points in the latin hypercube design.
    """
    assert(len(var_lower)==len(var_upper))

    n = len(var_lower)
    if (n == 1):
        # For unidimensional problems, simply take the two endpoints
        # of the interval as starting points
        return [var_lower, var_upper]
    # Otherwise, generate the LHD
    lhd = pyDOE.lhs(n, n+1, 'maximin')
    node_pos = [[var_lower[i] + (var_upper[i] - var_lower[i])*lhd_point[i] 
                 for i in range(n)] for lhd_point in lhd]
    return node_pos

# -- end function

def get_lhd_corr_points(var_lower, var_upper):
    """Compute a latin hypercube design with min correlation.

    Compute a list of (n+1) points in the given box, where n is the
    dimension of the space. The selected points are picked according
    to a random latin hypercube design with minimum correlation
    criterion. This function relies on the library pyDOE.

    Parameters
    ----------
    var_lower : List[float]
        List of lower bounds of the variables.
    var_upper : List[float]
        List of upper bounds of the variables.

    Returns
    -------
    List[List[float]]
        List of points in the latin hypercube design.
    """
    assert(len(var_lower)==len(var_upper))

    n = len(var_lower)
    if (n == 1):
        # For unidimensional problems, simply take the two endpoints
        # of the interval as starting points
        return [var_lower, var_upper]
    # Otherwise, generate the LHD
    lhd = pyDOE.lhs(n, n+1, 'correlation')
    node_pos = [[var_lower[i] + (var_upper[i] - var_lower[i])*lhd_point[i] 
                 for i in range(n)] for lhd_point in lhd]
    return node_pos

# -- end function

def initialize_nodes(settings, var_lower, var_upper, integer_vars):
    """Compute the initial sample points.

    Compute an initial list of nodes using the initialization strategy
    indicated in the algorithmic settings.
    
    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.

    var_lower : List[float]
        List of lower bounds of the variables.

    var_upper : List[float]
        List of upper bounds of the variables.

    integer_vars : List[int] or None
        A list containing the indices of the integrality constrained
        variables. If None or empty list, all variables are assumed to
        be continuous.

    Returns
    -------
    List[List[float]]
        List of at least n+1 corner points, where n is the dimension
        of the space. The number and position of points depends on the
        chosen strategy.

    Raises
    ------
    RuntimeError
        If a set of feasible and linearly independent sample points
        cannot be computed within the prescribed number of iterations.
    """
    assert(len(var_lower)==len(var_upper))
    assert(isinstance(settings, RbfSettings))

    # We must make sure points are linearly independent; if they are
    # not, we perform a given number of iterations
    dependent = True
    itercount = 0
    while (dependent and itercount < config.MAX_RANDOM_INIT):
        itercount += 1
        if (settings.init_strategy == 'all_corners'):
            nodes = get_all_corners(var_lower, var_upper)
        elif (settings.init_strategy == 'lower_corners'):
            nodes = get_lower_corners(var_lower, var_upper)
        elif (settings.init_strategy == 'rand_corners'):
            nodes = get_random_corners(var_lower, var_upper)
        elif (settings.init_strategy == 'lhd_maximin'):
            nodes = get_lhd_maximin_points(var_lower, var_upper)
        elif (settings.init_strategy == 'lhd_corr'):
            nodes = get_lhd_corr_points(var_lower, var_upper)

        if (integer_vars is not None and len(integer_vars) > 0):
            for i in range(len(nodes)):
                for j in integer_vars:
                    nodes[i][j] = round(nodes[i][j])

        Amat = np.row_stack(nodes)
        U, s, V = np.linalg.svd(Amat)
        if (min(s) > settings.eps_zero):
            dependent = False

    if (itercount == config.MAX_RANDOM_INIT):
        raise RuntimeError('Exceeded number of random initializations')

    return nodes

# -- end function

def round_integer_vars(point, integer_vars):
    """Round a point to the closest integer.

    Round the values of the integer-constrained variables to the
    closest integer value.

    Parameters
    ----------
    point : List[float]
        The point to be rounded.
    integer_vars : List[int] or None
        A list of indices of integer variables, or None.
    """
    if (integer_vars is not None):
        assert(max(integer_vars)<len(point))
        for i in integer_vars:
            point[i] = round(point[i])

# -- end function

def round_integer_bounds(var_lower, var_upper, integer_vars):
    """Round the variable bounds to integer values.

    Round the values of the integer-constrained variable bounds, in
    the usual way: lower bounds are rounded up, upper bounds are
    rounded down.

    Parameters
    ----------
    var_lower : List[float]
        List of lower bounds of the variables.

    var_upper : List[float]
        List of upper bounds of the variables.

    integer_vars : List[int] or None
        A list containing the indices of the integrality constrained
        variables. If None or empty list, all variables are assumed to
        be continuous.
    """
    if (integer_vars is not None):
        assert(len(var_lower)==len(var_upper))
        assert(max(integer_vars)<len(var_lower))
        for i in integer_vars:
            var_lower[i] = math.floor(var_lower[i])
            var_upper[i] = math.ceil(var_upper[i])
            if (var_upper[i] < var_lower[i]):
                # Swap the two bounds
                var_lower[i], var_upper[i] = var_upper[i], var_lower[i]

# -- end function

def norm(p):
    """Compute the L2-norm of a vector

    Compute the L2 (Euclidean) norm.

    Parameters
    ----------
    p : List[float]
        The point whose norm should be computed.

    Returns
    -------
    float
        The norm of the point.
    """
    norm = math.fsum(val*val for val in p)
    return math.sqrt(norm)

# -- end function

def distance(p1, p2):
    """Compute Euclidean distance between two points.

    Compute Euclidean distance between two points.

    Parameters
    ----------
    p1 : List[float]
        First point.
    p2 : List[float]
        Second point.

    Returns
    -------
    float
        Euclidean distance.
    """
    assert(len(p1) == len(p2))
    dist = math.fsum((p1[i]-p2[i])*(p1[i]-p2[i]) for i in range(len(p1)))
    return math.sqrt(dist)

# -- end function

def get_min_distance(point, other_points):
    """Compute minimum distance from a set of points.

    Compute the minimum Euclidean distance between a given point and a
    list of points.

    Parameters
    ----------
    point : List[float]
        The point we compute the distances from.

    other_points : List[List[float]]
        The list of points we want to compute the distances to.

    Returns
    -------
    float
        Minimum distance between point and the other_points.
    """
    assert(point is not None)
    assert(other_points is not None)

    min_dist = float('inf')
    for other_point in other_points:
        min_dist = min(min_dist, distance(point, other_point))
    return min_dist

# -- end function

def get_min_distance_index(point, other_points):
    """Compute the index of the point with minimum distance.

    Compute the index of the point in a list that achieves minimum
    Euclidean distance to a given point.

    Parameters
    ----------
    point : List[float]
        The point we compute the distances from.

    other_points : List[List[float]]
        The list of points we want to compute the distances to.

    Returns
    -------
    int
        The index of the point in other_points that achieved minimum
        distance from point.
    """
    assert(point is not None)
    assert(other_points is not None)

    min_dist = float('inf')
    index = len(other_points)
    for (i, other_point) in enumerate(other_points):
        dist = distance(point, other_point)
        if (dist < min_dist):
            min_dist = dist
            index = i
    return index

# -- end function
        
def get_rbf_matrix(settings, n, k, node_pos):
    """Compute the matrix for the RBF system.

    Compute the matrix A = [Phi P; P^T 0] of equation (3) in the paper
    by Costa and Nannicini.
    
    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings.
        Global and algorithmic settings.

    n : int
        Dimension of the problem, i.e. the size of the space.

    k : int
        Number of interpolation nodes.

    Returns
    -------
    numpy.matrix
        The matrix A = [Phi P; P^T 0].

    Raises
    ------
    ValueError
        If the type of RBF function is not supported.
    """
    assert(len(node_pos)==k)
    assert(isinstance(settings, RbfSettings))

    rbf = get_rbf_function(settings)
    p = get_size_P_matrix(settings, n)
    # Create matrix P.
    if (p == n + 1):
        # Keep the node coordinates and append a 1.
        # P is ((k) x (n+1)), PTr is its transpose
        P = [ point + [1.0] for point in node_pos ]
        PTr = [ [point[i] for point in node_pos] 
                for i in range(n) ] + [ [1.0 for i in range(k)] ]
    elif (p == 1):
        # P is an all-one vector of size ((k) x (1))
        P = [ [1.0] for i in range(k) ]
        PTr = [ [ 1.0 for i in range(k) ] ]
    else:
        raise ValueError('Rbf "' + settings.rbf + '" not implemented yet')

    # Now create matrix Phi. Phi is ((k) x (k))
    Phi = [ [rbf(distance(p1, p2)) for p2 in node_pos]
            for p1 in node_pos ]
    # Put together to obtain [Phi P; P^T 0].
    A = ([ Phi[i] + P[i] for i in range(k) ] + 
         [ PTr[i] + [0 for j in range(p)] for i in range(p)])

    Amat = np.matrix(A)
    # Zero out tiny elements
    Amat[np.abs(Amat) < settings.eps_zero] = 0

    return Amat

# -- end function

def get_matrix_inverse(settings, Amat):
    """Compute the inverse of a matrix.

    Compute the inverse of a given matrix, zeroing out small
    coefficients to improve sparsity.
    
    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.
    Amat : numpy.matrix
        The matrix to invert.

    Returns
    -------
    numpy.matrix
        The matrix Amat^{-1}.

    Raises
    ------
    numpy.linalg.LinAlgError
        If the matrix cannot be inverted for numerical reasons.
    """
    assert(isinstance(settings, RbfSettings))
    assert(isinstance(Amat, np.matrix))

    try:
        Amatinv = Amat.getI()
    except np.linalg.LinAlgError as e:
        print('Exception raised trying to invert the RBF matrix',
              file = sys.stderr)
        print(e, file = sys.stderr)
        raise e

    # Zero out tiny elements of the inverse -- this is potentially
    # dangerous as the product between Amat and Amatinv may not be the
    # identity, but if the zero tolerance is chosen not too large,
    # this should help the optimization process
    Amatinv[np.abs(Amatinv) < settings.eps_zero] = 0

    return Amatinv

# -- end function
    
def get_rbf_coefficients(settings, n, k, Amat, node_val):
    """Compute the coefficients of the RBF interpolant.

    Solve a linear system to compute the coefficients of the RBF
    interpolant.

    Parameters
    ---
    settings : rbfopt_settings.RbfSettings.
        Global and algorithmic settings.

    n : int
        Dimension of the problem, i.e. the size of the space.

    k : int
        Number of interpolation nodes.

    Amat : numpy.matrix
        Matrix [Phi P; P^T 0] defining the linear system. Must be a
        square matrix of appropriate size.

    node_val : List[float]
        List of values of the function at the nodes.

    Returns
    -------
    (List[float], List[float])
        Lambda coefficients (for the radial basis functions), and h
        coefficients (for the polynomial).
    """    
    assert(len(node_val)==(k))
    assert(isinstance(settings, RbfSettings))
    assert(isinstance(Amat, np.matrix))
    p = get_size_P_matrix(settings, n)
    assert(Amat.shape==(k+p,k+p))
    rhs = node_val + [0 for i in range(p)]
    try:
        solution = np.linalg.solve(Amat, rhs)
    except np.linalg.LinAlgError as e:
        print('Exception raised in the solution of the RBF linear system',
              file = sys.stderr)
        print('Exception details:', file = sys.stderr)
        print(e, file = sys.stderr)
        exit()
        
    return (solution[0:k], solution[k:])

# -- end function

def evaluate_rbf(settings, point, n, k, node_pos, rbf_lambda, rbf_h):
    """Evaluate the RBF interpolant at a given point.

    Evaluate the RBF interpolant at a given point.

    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings.
        Global and algorithmic settings.

    point : List[float]
        The point in R^n where we want to evaluate the interpolant.
    
    n : int
        Dimension of the problem, i.e. the size of the space.

    k : int
        Number of interpolation nodes.

    node_pos : List[List[float]]
        List of coordinates of the interpolation points. 

    rbf_lambda : List[float]
        The lambda coefficients of the RBF interpolant, corresponding
        to the radial basis functions. List of dimension k.

    rbf_h : List[float]
        The h coefficients of the RBF interpolant, corresponding to he
        polynomial. List of dimension given by get_size_P_matrix().

    Returns
    -------
    float
        Value of the RBF interpolant at the given point.
    """
    assert(len(point)==n)
    assert(len(rbf_lambda)==k)
    assert(len(node_pos)==k)
    assert(isinstance(settings, RbfSettings))
    p = get_size_P_matrix(settings, n)
    assert(len(rbf_h)==p)

    rbf_function = get_rbf_function(settings)
    
    # Formula:
    # \sum_{i=1}^k \lambda_i \phi(\|x - x_i\|) + h^T (x 1)
    part1 = math.fsum(rbf_lambda[i] *
                      rbf_function(distance(point, node_pos[i]))
                      for i in range(k))
    part2 = math.fsum(rbf_h[i]*point[i] for i in range(p-1))
    return math.fsum([part1, part2, rbf_h[-1] if (p > 0) else 0.0])

# -- end function

def get_fast_error_bounds(settings, value):
    """Compute error bounds for fast interpolation nodes.

    Compute the interval that contains the true value of a fast
    function evaluation, according to the specified relative and
    absolute error tolerances.

    Parameters
    ----------

    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.
    value : float
        The value for which the error interval should be computed.

    Returns
    -------
    (float, float)
        A tuple (lower_variation, upper_variation) indicating the
        possible deviation to the left (given as a negative number)
        and to the right of the current value.
    """
    return (-abs(value)*settings.fast_objfun_rel_error - 
            settings.fast_objfun_abs_error,
            abs(value)*settings.fast_objfun_rel_error +
            settings.fast_objfun_abs_error)

# -- end function

def transform_function_values(settings, node_val, fmin, fmax,
                              fast_node_index = list()):
    """Rescale function values.
    
    Rescale and adjust function values according to the chosen
    strategy and to the occurrence of large fluctuations (high
    dynamism). May not rescale at all if rescaling is off.

    Parameters
    ----------

    settings : rbfopt_settings.RbfSettings
       Global and algorithmic settings.

    node_val : List[float]
       List of function values at the interpolation nodes.

    fmin : float
       Minimum function value found so far.

    fmax : float
       Maximum function value found so far.
    
    fast_node_index : List[int] or None
       Index of function evaluations in 'fast' mode. If this is empty,
       then error bounds will not be returned.

    Returns
    -------
    (List[float], float, float, List[(float, float)])
        A quadruple (scaled_function_values, scaled_fmin, scaled_fmax,
        fast_error_bounds) containing a list of rescaled function
        values, the rescaled minimum, the rescaled maximum, the
        rescaled error bounds for function evaluations in 'fast' mode.

    Raises
    ------
    ValueError
        If the function scaling strategy requested is not implemented.
    """
    assert(isinstance(settings, RbfSettings))
    # Check dynamism: if too high, replace large function values with
    # the median or clip at maximum dynamism
    if (settings.dynamism_clipping != 'off' and
        ((abs(fmin) > settings.eps_zero and
          abs(fmax)/abs(fmin) > settings.dynamism_threshold) or
         (abs(fmin) <= settings.eps_zero and
          abs(fmax) > settings.dynamism_threshold))):
        if (settings.dynamism_clipping == 'median'):
            sorted_values = sorted(node_val)
            median = sorted_values[len(sorted_values)//2]
            clip_val = [min(val, median) for val in node_val]
            fmax = median
        elif (settings.dynamism_clipping == 'clip_at_dyn'):
            clip_val = [min(val, settings.dynamism_threshold*abs(fmin))
                               for val in node_val]
            fmax = settings.dynamism_threshold*abs(fmin)
    else:
        clip_val = node_val

    if (settings.function_scaling == 'off'):
        # We make a copy because the caller may assume that
        return (list(clip_val), fmin, fmax, 
                [get_fast_error_bounds(settings, clip_val[i])
                 for i in fast_node_index])
    elif (settings.function_scaling == 'affine'):
        # Compute denominator separately to make sure that it is not
        # zero. This may happen if the surface is "flat" after median
        # clipping.
        denom = (fmax - fmin) if (fmax - fmin > settings.eps_zero) else 1.0
        return ([(val - fmin)/denom for val in clip_val], 0.0, 1.0,
                [tuple([val/denom for val in 
                        get_fast_error_bounds(settings, clip_val[i])])
                 for i in fast_node_index])
    elif (settings.function_scaling == 'log'):
        # Compute by how much we should translate to make all points >= 1
        shift = (max(0.0, 1.0 - fmin) if not fast_node_index
                 else max(0.0, 1.0 - fmin -
                          get_fast_error_bounds(settings, fmin)[0]))
        return ([math.log(val + shift) for val in clip_val], 
                math.log(fmin + shift), math.log(fmax + shift),
                [tuple([math.log((clip_val[i] + shift + val) / 
                                 (clip_val[i] + shift))
                        for val in get_fast_error_bounds(settings,
                                                         clip_val[i])])
                 for i in fast_node_index])
    else:
        raise ValueError('Function scaling "' + settings.function_scaling + 
                         '" not implemented')

# -- end function

def transform_domain(settings, var_lower, var_upper, point, reverse = False):
    """Rescale the domain.

    Rescale the function domain according to the chosen strategy.

    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.

    var_lower : List[float]
        List of lower bounds of the variables.

    var_upper : List[float]
        List of upper bounds of the variables.

    point : List[float]
        Point in the domain to be rescaled.

    reverse : bool
        False if we transform from the original domain to the
        transformed space, True if we want to apply the reverse.

    Returns
    -------
    List[float]
        Rescaled point.
    
    Raises
    ------
    ValueError
        If the requested rescaling strategy is not implemented.
    """
    assert(isinstance(settings, RbfSettings))
    assert(len(var_lower)==len(var_upper))
    assert(len(var_lower)==len(point))

    if (settings.domain_scaling == 'off'):
        # Make a copy because the caller may assume so
        return [val for val in point]
    elif (settings.domain_scaling == 'affine'):
        # Make an affine transformation to the unit hypercube
        if (reverse):
            return [point[i]*(var_upper[i] - var_lower[i]) + var_lower[i]
                    for i in range(len(point))]
        else:
            return [(point[i] - var_lower[i])/(var_upper[i] - var_lower[i])
                    for i in range(len(point))]
    else:
        raise ValueError('Domain scaling "' + settings.domain_scaling + 
                         '" not implemented')
    
# -- end function

def transform_domain_bounds(settings, var_lower, var_upper):
    """Rescale the variable bounds.

    Rescale the bounds of the function domain according to the chosen
    strategy.

    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.
    var_lower : List[float]
        List of lower bounds of the variables.
    var_upper : List[float]
        List of upper bounds of the variables.

    Returns
    -------
    (List[float], List[float])
        Rescaled bounds as (lower, upper).
    
    Raises
    ------
    ValueError
        If the requested rescaling strategy is not implemented.
    """
    assert(isinstance(settings, RbfSettings))
    assert(len(var_lower)==len(var_upper))

    if (settings.domain_scaling == 'off'):
        # Make a copy because the caller may assume so
        return ([val for val in var_lower], [val for val in var_upper])
    elif (settings.domain_scaling == 'affine'):
        # Make an affine transformation to the unit hypercube
        return ([0 for val in var_lower], [1 for val in var_upper])
    else:
        raise ValueError('Domain scaling "' + settings.domain_scaling + 
                         '" not implemented')
    
# -- end function

def get_sigma_n(k, current_step, num_global_searches, num_initial_points):
    """Compute sigma_n.
    
    Compute the index :math: `sigma_n`, where :math: `sigma_n` is a
    function described in the paper by Gutmann (2001). The same
    function is called :math: `alpha_n` in a paper of Regis &
    Shoemaker (2007).

    Parameters
    ----------
    k : int
        Number of nodes, i.e. interpolation points.    
    current_step : int
        The current step in the cyclic search strategy.
    num_global_searches : int
        The number of global searches in a cycle.
    num_initial_points : int
        Number of points for the initialization phase.

    Returns
    -------
    int
        The value of sigma_n.
    """
    assert(current_step >= 1)
    assert(num_global_searches >= 0)
    if (current_step == 1):
        return k - 1
    return (get_sigma_n(k, current_step - 1, num_global_searches,
                        num_initial_points) -
            int(math.floor((k - num_initial_points)/num_global_searches)))

# -- end function    

def get_fmax_current_iter(settings, n, k, current_step, node_val):
    """Compute the largest function value for target value computation.
    
    Compute the largest function value used to determine the target
    value. This is given by the sorted value in position :math:
    `sigma_n`.

    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.
    n : int
        Dimension of the problem, i.e. the space where the point lives.
    k : int
        Number of nodes, i.e. interpolation points.
    current_step : int
        The current step in the cyclic search strategy.
    node_val : List[float]
        List of function values.

    Returns
    -------
    float
        The value that should be used to determine the range of the
        function values when computing the target value.

    See also
    --------
    get_sigma_n
    """
    assert(isinstance(settings, RbfSettings))
    assert(k == len(node_val))
    assert(current_step >= 1)
    sorted_node_val = sorted(node_val)
    num_initial_points = (2**n if settings.init_strategy == 'all_corners'
                           else n + 1)
    s_n = get_sigma_n(k, current_step, settings.num_global_searches,
                          num_initial_points)
    return sorted_node_val[s_n]

# -- end function

def get_min_bump_node(settings, n, k, Amat, node_val,
                      fast_node_index, fast_node_err_bounds, 
                      target_val):
    """Compute the bumpiness obtained by moving an interpolation point.

    Compute the bumpiness of the interpolant obtained by moving a
    single node (the one that yields minimum bumpiness, which is
    determined by this function) within target_val plus or minus
    error, to target_val.

    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.

    n : int
        Dimension of the problem, i.e. the space where the point lives.

    k : int
        Number of nodes, i.e. interpolation points.

    Amat : numpy.matrix
        The matrix A = [Phi P; P^T 0] of equation (3) in the paper by
        Costa and Nannicini.

    node_val : List[float]
        List of values of the function at the nodes.

    fast_node_index : List[int]
        List of indices of nodes whose function value should be
        considered variable withing the allowed range.
    
    fast_node_err_bounds : List[int]
        Allowed deviation from node values for nodes affected by
        error. This is a list of tuples (lower, upper) of the same
        length as fast_node_index.

    target_val : float
        Target function value at which we want to move the node.

    Returns
    -------
    (int, float)
        The index of the node and corresponding bumpiness value
        indicating the sought node in the list node_pos.
    """
    assert(isinstance(settings, RbfSettings))
    assert(len(node_val)==k)
    assert(isinstance(Amat, np.matrix))
    assert(len(fast_node_index)==len(fast_node_err_bounds))

    # Extract the matrices Phi and P from
    Phimat = Amat[:k, :k]
    Pmat = Amat[:k, k:]
    
    min_bump_index, min_bump = None, float('Inf')
    for (pos, i) in enumerate(fast_node_index):
        # Check if we are within the allowed range
        if (node_val[i] + fast_node_err_bounds[pos][0] <= target_val and
            node_val[i] + fast_node_err_bounds[pos][1] >= target_val):
            # If so, compute bumpiness. Save original data.
            orig_node_val = node_val[i]
            orig_node_err_bounds = fast_node_err_bounds[pos]
            # Fix this node at the target value.
            node_val[i] = target_val
            fast_node_err_bounds[pos] = (0.0, 0.0)
            # Compute RBF interpolant.
            # Get coefficients for the exact RBF first
            (rbf_l, rbf_h) = get_rbf_coefficients(settings, n, k, 
                                                  Amat, node_val)
            # And now the noisy version
            (rbf_l,
             rbf_h) = aux.get_noisy_rbf_coefficients(settings, n, k, Phimat,
                                                     Pmat, node_val,
                                                     fast_node_index,
                                                     fast_node_err_bounds,
                                                     rbf_l, rbf_h)
            # Restore original values
            node_val[i] = orig_node_val
            fast_node_err_bounds[pos] = orig_node_err_bounds
            # Compute bumpiness using the formula \lambda^T \Phi \lambda
            bump = math.fsum(rbf_l[h]*Phimat[h,j]*rbf_l[j]
                             for h in range(k) for j in range(k))
            if (bump < min_bump):
                min_bump_index, min_bump = i, bump

    return (min_bump_index, min_bump)

# -- end function

def get_bump_new_node(settings, n, k, node_pos, node_val, new_node,
                      fast_node_index, fast_node_err_bounds, target_val):
    """Compute the bumpiness with a new interpolation point.

    Computes the bumpiness of the interpolant obtained by setting a
    new node in a specified location, at value target_val.

    Parameters
    ----------
    settings : rbfopt_settings.RbfSettings
        Global and algorithmic settings.

    n : int
        Dimension of the problem, i.e. the space where the point lives.

    k : int
        Number of nodes, i.e. interpolation points.

    node_pos : List[List[float]]
        Location of current interpolation nodes.

    node_val : List[float]
        List of values of the function at the nodes.

    new_node : List[float]
        Location of new interpolation node.

    fast_node_index : List[int]
        List of indices of nodes whose function value should be
        considered variable withing the allowed range.
    
    fast_node_err_bounds : List[int]
        Allowed deviation from node values for nodes affected by
        error. This is a list of tuples (lower, upper) of the same
        length as fast_node_index.

    target_val : float
        Target function value at which we want to move the node.
    
    Returns
    -------
    float
        The bumpiness of the interpolant having a new node at the
        specified location, with value target_val.
    """
    assert(isinstance(settings, RbfSettings))
    assert(len(node_val)==k)
    assert(len(node_pos)==k)
    assert(len(fast_node_index)==len(fast_node_err_bounds))
    assert(new_node is not None)

    # Add the new node to existing ones
    n_node_pos = node_pos + [new_node]
    n_node_val = node_val + [target_val]

    # Compute the matrices necessary for the algorithm
    Amat = get_rbf_matrix(settings, n, k + 1, n_node_pos)

    # Get coefficients for the exact RBF
    (rbf_l, rbf_h) = get_rbf_coefficients(settings, n, k + 1, Amat,
                                          n_node_val)

    # Get RBF coefficients for noisy interpolant
    (rbf_l, rbf_h) = aux.get_noisy_rbf_coefficients(settings, n, k + 1, 
                                                    Amat[:(k+1), :(k+1)],
                                                    Amat[:(k+1), (k+1):],
                                                    n_node_val,
                                                    fast_node_index,
                                                    fast_node_err_bounds,
                                                    rbf_l, rbf_h)

    bumpiness = math.fsum(rbf_l[h]*Amat[h,j]*rbf_l[j]
                          for h in range(k+1) for j in range(k+1))

    return bumpiness

# -- end function
