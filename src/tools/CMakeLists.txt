if (BUILD_TOOLS)
        # Target: evaluate_gripper
	add_executable(evaluate_gripper evaluate_gripper.cpp)
	add_dependencies(evaluate_gripper gripperz)
	target_link_libraries(evaluate_gripper gripperz)
                
        # Target: optimize_gripper
	add_executable(optimize_gripper optimize_gripper.cpp)
	add_dependencies(optimize_gripper gripperz)
	target_link_libraries(optimize_gripper gripperz)
endif ()
