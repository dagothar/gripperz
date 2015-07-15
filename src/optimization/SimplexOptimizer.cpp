/**
 * @file SimplexOptimizer.cpp
 * @author Adam Wolniakowski
 * @date 2015-07-15
 */

#include "SimplexOptimizer.hpp"
#include <boost/foreach.hpp>
#include <rw/math/Q.hpp>
#include <rw/math/Math.hpp>


using namespace gripperz::optimization;
using namespace gripperz::math;
using namespace std;
using namespace rw::math;


typedef std::pair<Q, double> Vertex;


SimplexOptimizer::SimplexOptimizer(Configuration config) :
	_config(config)
{
}


SimplexOptimizer::~SimplexOptimizer() {
}


vector<Vertex> buildSimplex(const Q& start, double size) {
	vector<Vertex> vertices;

	// place a vertex at the origin
	vertices.push_back(std::make_pair(start, 0.0));

	// place remaining vertices
	unsigned n = start.size();
	for (unsigned i = 0; i < n; ++i) {
		Vertex v = std::make_pair(start, 0.0);
		v.first[i] += size;
		vertices.push_back(v);
	}

	return vertices;
}


bool vertexComp(const Vertex& i, const Vertex& j) {
	return i.second < j.second;
}


Q getCentroid(const vector<Vertex>& vertices, const Vertex& ve) {
	unsigned n = vertices.size();
	Q centroid(n, 0.0);

	BOOST_FOREACH (const Vertex& v, vertices) {
		if (&v != &ve) {
			centroid += v.first;
		}
	}

	return centroid / (n - 1);
}


Vector SimplexOptimizer::minimize(ObjectiveFunction::Ptr function, const Vector& initialGuess) {
	/*
	 * Build simplex.
	 */
	unsigned dim = initialGuess.size();
	Q init(dim), resultQ(dim);
	
	init = Math::fromStdVector(initialGuess, init);
	vector<Vertex> vertices = buildSimplex(init, _config.initialSize);
	
	/*
	 * Evaluate function at vertices.
	 */
	unsigned fev = 0;
	BOOST_FOREACH (Vertex& v, vertices) {
		v.second = function->evaluate(Math::toStdVector(v.first, dim));
		++fev;
	}
	std::sort(vertices.begin(), vertices.end(), vertexComp);
	
	/*
	 * Optimization loop.
	 */
	double rho;
	do {
		/*
		 * 1. Pick worst vertex, and try reflecting it about centroid.
		 */
		Vertex& v = vertices.back();
		Q centroid = getCentroid(vertices, v);
		Vertex reflected = std::make_pair(centroid + kReflection * (centroid - v.first), 0.0);
		reflected.second = function->evaluate(Math::toStdVector(reflected.first, dim)); ++fev;
		
		/*
		 * 2. If the reflected point is the best point among all vertices,
		 * try expansion step.
		 */
		if (reflected.second < vertices.front().second) {
			Vertex expanded = std::make_pair(centroid + kExpansion * (centroid - v.first), 0.0);
			expanded.second = function->evaluate(Math::toStdVector(expanded.first, dim)); ++fev;

			// if expanded point is better than reflected point, then assign v := expanded
			if (expanded.second < reflected.second) {
				v = expanded;
			} else {
				v = reflected;
			}
		} else {
			/*
			 * 3. If reflected point is better than second worst, but not better than
			 * the best point, assign v := reflected
			 */
			Vertex& w = *(vertices.rbegin() + 1);
			if (reflected.second < w.second) {
				v = reflected;
			} else {
				/*
				 * 4. Try contraction on the v vertex
				 */
				Vertex contracted = std::make_pair(centroid + kContraction * (centroid - v.first), 0.0);
				contracted.second = function->evaluate(Math::toStdVector(contracted.first, dim)); ++fev;
				
				// if contracted point is better than v, assign v := contracted
				if (contracted.second < v.second) {
					v = contracted;
				} else {
					// perform reduction of all the points except best!
					Vertex best = vertices.front();
					for (std::vector<Vertex>::iterator i = vertices.begin() + 1; i != vertices.end(); ++i) {
						(*i) = std::make_pair(best.first + kReduction * (best.first - i->first), 0.0);
						i->second = function->evaluate(Math::toStdVector(i->first, dim)); ++fev;
					}
				}
			}
		}
		
		/* sort vertices */
		std::sort(vertices.begin(), vertices.end(), vertexComp);
		
		/*
		 * Store result and get RHO
		 */
		resultQ = vertices.front().first;

		/*
		 * Error is calculated as the difference in values of best & worst points divided by their distance
		 */
		rho = (vertices.front().first - vertices.back().first).norm2();
		
	} while (rho > _config.finalSize && fev < _config.maxNOfEvaluations);
	
	/* Return results */
	return Math::toStdVector(resultQ, dim);
}
