/**
 * @file TaskDescriptionLoader.hpp
 * @author Adam Wolniakowski
 * @date 3-07-2015
 */

#pragma once

#include <context/TaskDescription.hpp>
#include <util/XMLHelpers.hpp>

namespace gripperz {
namespace loaders {

/**
 * @class TaskDescriptionLoader
 * @brief Loads & saves task description from/to XML file.
 */
class TaskDescriptionLoader {
public:
	//! Load task description from XML file.
	static context::TaskDescription::Ptr load(const std::string& filename,
			rwsim::dynamics::DynamicWorkCell::Ptr dwc);

	//! Save task description to XML file.
	static void save(const context::TaskDescription::Ptr td,
			const std::string& filename);
			
protected:
	static context::TaskDescription::Ptr readTaskDescription(gripperz::util::PTree& tree, rwsim::dynamics::DynamicWorkCell::Ptr dwc);
	static void readTarget(gripperz::util::PTree& tree, context::TaskDescription::Ptr task);
	static void readGripper(gripperz::util::PTree& tree, context::TaskDescription::Ptr task);
	static void readInterferenceObjects(gripperz::util::PTree& tree, context::TaskDescription::Ptr task);
	static void readLimits(gripperz::util::PTree& tree, context::TaskDescription::Ptr task);
	static void readQualities(gripperz::util::PTree& tree, context::TaskDescription::Qualities& q);
	static void readHints(gripperz::util::PTree& tree, context::TaskDescription::Ptr task);
	static void readGrasp(gripperz::util::PTree& tree, context::TaskDescription::Ptr task);
	static void readAlignment(gripperz::util::PTree& tree, context::TaskDescription::Ptr task);
};

} /* loaders */
} /* gripperz */
