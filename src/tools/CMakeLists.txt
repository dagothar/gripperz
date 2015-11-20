if (BUILD_TOOLS)
        # Target: evaluate_gripper
	add_executable(evaluate_gripper evaluate_gripper.cpp)
	add_dependencies(evaluate_gripper gripperz)
	target_link_libraries(evaluate_gripper gripperz)
                
        # Target: optimize_gripper
	add_executable(optimize_gripper optimize_gripper.cpp)
	add_dependencies(optimize_gripper gripperz)
	target_link_libraries(optimize_gripper gripperz)
                
        # Target: generate_grippers
	add_executable(generate_grippers generate_grippers.cpp)
	add_dependencies(generate_grippers gripperz)
	target_link_libraries(generate_grippers gripperz)
endif ()
