/**
 * @file MapGripperBuilder.hpp
 * @author Adam Wolniakowski
 * @date 2015-07-06
 */

#pragma once

#include "GripperBuilder.hpp"

namespace gripperz {
namespace models {

/**
 * @class MapGripperBuilder
 * @brief Uses a gripper prototype and a list of parameters to apply.
 */
class MapGripperBuilder : public GripperBuilder {
public:
	//! Smart pointer.
	typedef rw::common::Ptr<MapGripperBuilder> Ptr;
	
	//! Names of the parameters.
	typedef enum {
		Length=0,
		Width,
		Depth,
		ChfDepth,
		ChfAngle,
		CutDepth,
		CutAngle,
		CutTilt,
		TcpOffset,
		Jawdist,
		Stroke,
		Force
	} ParameterName;

public:
	MapGripperBuilder(
		Gripper::Ptr prototype,
		const std::vector<ParameterName>& parameters=std::vector<ParameterName>{
			Length,
			Width,
			Depth,
			ChfDepth,
			ChfAngle,
			CutDepth,
			CutAngle,
			CutTilt,
			TcpOffset,
			Jawdist,
			Stroke,
			Force
		}
	);
	
	virtual ~MapGripperBuilder();
	
	virtual Gripper::Ptr parametersToGripper(const math::Vector& params);

	virtual math::Vector gripperToParameters(Gripper::Ptr gripper);
	
	Gripper::Ptr getPrototype() { return _prototype; }
	void setPrototype(Gripper::Ptr prototype) { _prototype = prototype; }
	
	const std::vector<ParameterName>& getParameterList() const { return _parameters; }
	void setParameterList(const std::vector<ParameterName>& parameters) { _parameters = parameters; }
	
	static std::string parameterNameToString(ParameterName parameter);
	
protected:
	void applyParameter(Gripper::Ptr gripper, ParameterName name, double value);
	
	double fetchParameter(Gripper::Ptr gripper, ParameterName name);
	
private:
	Gripper::Ptr _prototype;
	std::vector<ParameterName> _parameters;
};

} /* models */
} /* gripperz */
