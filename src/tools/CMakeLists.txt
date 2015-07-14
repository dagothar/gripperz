if (BUILD_TOOLS)
	# Target: evaluate_gripper
	add_executable(evaluate_gripper evaluateGripper.cpp)
	add_dependencies(evaluate_gripper gripperz)
	target_link_libraries(evaluate_gripper ode_plugin.rwplugin gripperz ${ROBWORK_LIBRARIES} ${ROBWORKSIM_LIBRARIES})

	# Target: create-gripper-xml
	#add_executable(create-gripper-xml createGripperXML.cpp)
	#add_dependencies(create-gripper-xml gripperz)
	#target_link_libraries(create-gripper-xml gripperz)

	# Target: generate-surface-samples
	add_executable(generate-surface-samples generateSurfaceSamples.cpp)
	add_dependencies(generate-surface-samples gripperz)
	target_link_libraries(generate-surface-samples gripperz)
	
	# Target: gripper_landscape
	add_executable(gripper_landscape gripper_landscape.cpp)
	add_dependencies(gripper_landscape gripperz)
	target_link_libraries(gripper_landscape gripperz)
	
	# Target: generate_grippers
	add_executable(generate_grippers generate_grippers.cpp)
	add_dependencies(generate_grippers gripperz)
	target_link_libraries(generate_grippers gripperz)
	
	# Target: optimize_gripper
	add_executable(optimize_gripper gripper_optimizer.cpp)
	add_dependencies(optimize_gripper gripperz)
	target_link_libraries(optimize_gripper gripperz)
endif ()
