/**
 * @file ParameterMapping.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <vector>
#include <rw/common/Ptr.hpp>

namespace gripperz {
namespace optimization {

/**
 * @class ParameterMapping
 * @brief Transforms the parameter space by mapping from normalized, or un-mapping into normalized.
 */
class ParameterMapping {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<ParameterMapping> Ptr;
	
	//! Defines range on a single parameter (lower : upper).
	typedef std::pair<double, double> Range;
	
	//! Defines a mapping on a single parameter (from : to)
	typedef std::pair<Range, Range> Transform;
	
	//! Defines the parameter map.
	typedef std::vector<Transform> Map;
	
public:
	ParameterMapping(const Map& map);
	virtual ~ParameterMapping();
	
	const Map& getMap() const { return _map; }
	void setMap(const Map& map) { _map = map; }

	/**
	 * @brief Maps the parameter vector into normalized vector (using bounds).
	 */
	virtual std::vector<double> pack(const std::vector<double> params) const;
	
	/**
	 * @brief Un-maps normalized parameters into a real space (using bounds).
	 */
	virtual std::vector<double> unpack(const std::vector<double> params) const;
	
private:
	Map _map;
};

} /* optimization */
} /* gripperz */
