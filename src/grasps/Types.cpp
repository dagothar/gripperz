#include "Types.hpp"

using namespace gripperz::grasps;
using namespace rwlibs::task;

Grasps copyGrasps(const Grasps tasks, bool onlySuccesses = false) {
    Grasps tasks_copy = tasks->clone();

    // clone subtasks

    BOOST_FOREACH(GraspSubTask& subtask, tasks->getSubTasks()) {
        GraspSubTask subtask_copy = subtask.clone();

        BOOST_FOREACH(GraspTarget& target, subtask.getTargets()) {
            if (!onlySuccesses || target.getResult()->testStatus == GraspResult::Success) {
                subtask_copy.addTarget(target);
            }
        }

        tasks_copy->addSubTask(subtask_copy);
    }

    return tasks_copy;
}
