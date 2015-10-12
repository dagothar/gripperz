/**
 * @file GripperXMLLoader.hpp
 * @author Adam Wolniakowski
 */

#pragma once

#include <string>
#include <models/OldGripper.hpp>

namespace gripperz {
namespace loaders {

/**
 * @class GripperXMLLoader
 * @brief Class for loading grippers from xml files.
 */
class GripperXMLLoader {
public:
	// static
	/**
	 * @brief Load gripper design from XML file.
	 */
	static models::OldGripper::Ptr load(const std::string& filename);

	/**
	 * @brief Save gripper data to XML file.
	 */
	static void save(models::OldGripper::Ptr gripper, const std::string& filename);

protected:
};

} /* loaders */
} /* gripperz */
