if (BUILD_TOOLS)
	# Target: generate-surface-samples
#	add_executable(generate-surface-samples generateSurfaceSamples.cpp)
#	add_dependencies(generate-surface-samples gripperz)
#	target_link_libraries(generate-surface-samples gripperz)
	
	# Target: gripper_landscape
	add_executable(gripper_landscape gripper_landscape.cpp)
	add_dependencies(gripper_landscape gripperz)
	target_link_libraries(gripper_landscape gripperz)
	
	# Target: generate_grippers
	add_executable(generate_grippers generate_grippers.cpp)
	add_dependencies(generate_grippers gripperz)
	target_link_libraries(generate_grippers gripperz)
	
	# Target: gripper_optimizer
	add_executable(gripper_optimizer gripper_optimizer.cpp)
	add_dependencies(gripper_optimizer gripperz)
	target_link_libraries(gripper_optimizer gripperz)
	
	# Target: gripper_evaluate
	add_executable(gripper_evaluate gripper_evaluate.cpp)
	add_dependencies(gripper_evaluate gripperz)
	target_link_libraries(gripper_evaluate gripperz)
endif ()
