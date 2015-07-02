/**
 * @file GripperXMLLoader.hpp
 * @author Adam Wolniakowski
 */

#pragma once

#include <string>
#include "Gripper.hpp"

namespace grippers {

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
	static Gripper::Ptr load(const std::string& filename);

	/**
	 * @brief Save gripper data to XML file.
	 */
	static void save(Gripper::Ptr gripper, const std::string& filename);

protected:
};

} // namespace
