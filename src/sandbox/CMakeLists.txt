if (BUILD_SANDBOX)
	# Target: param_landscape
	#add_executable(param_landscape paramLandscape.cpp)
	#add_dependencies(param_landscape gripperz)
	#target_link_libraries(param_landscape gripperz)
	
	# Target: gripper_opt
	#add_executable(gripper_opt gripper_opt.cpp)
	#add_dependencies(gripper_opt gripperz)
	#target_link_libraries(gripper_opt gripperz)
	
	# Target: trying_opt
	add_executable(trying_opt trying_opt.cpp)
	add_dependencies(trying_opt gripperz)
	target_link_libraries(trying_opt gripperz)
	
	# Target: extract_constraints
	add_executable(extract_constraints extract_constraints.cpp)
	add_dependencies(extract_constraints gripperz)
	target_link_libraries(extract_constraints gripperz)
	
	# Target: extract_poses
	add_executable(extract_poses extract_poses.cpp)
	add_dependencies(extract_poses gripperz)
	target_link_libraries(extract_poses gripperz)
	
	# Target: alignment_test
	add_executable(alignment_test alignment_test.cpp)
	add_dependencies(alignment_test gripperz)
	target_link_libraries(alignment_test gripperz)
	
	# Target: extract_versors
	add_executable(extract_versors extract_versors.cpp)
	add_dependencies(extract_versors gripperz)
	target_link_libraries(extract_versors gripperz)
	
	# Target: versor_test
	add_executable(versor_test versor_test.cpp)
	add_dependencies(versor_test gripperz)
	target_link_libraries(versor_test gripperz)
	
	# Target: objective_example
	add_executable(objective_example objective_example.cpp)
	add_dependencies(objective_example gripperz)
	target_link_libraries(objective_example gripperz)
endif ()
