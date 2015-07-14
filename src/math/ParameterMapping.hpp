/**
 * @file ParameterMapping.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include "Types.hpp"
#include <rw/common/Ptr.hpp>

namespace gripperz {
namespace math {

/**
 * @class ParameterMapping
 * @brief Transforms the parameter space by mapping from normalized, or un-mapping into normalized.
 */
class ParameterMapping {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<ParameterMapping> Ptr;
	
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
	 * @brief Maps the parameter vector.
	 */
	virtual Vector map(const Vector& params) const;
	
	/**
	 * @brief Un-maps normalized parameters.
	 */
	virtual Vector unmap(const Vector& params) const;
	
private:
	Map _map;
};

} /* math */
} /* gripperz */
