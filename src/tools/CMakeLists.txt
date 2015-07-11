if (BUILD_TOOLS)
	# Target: evaluate-gripper
	add_executable(evaluate-gripper evaluateGripper.cpp)
	add_dependencies(evaluate-gripper gripperz)
	target_link_libraries(evaluate-gripper ode_plugin.rwplugin gripperz ${ROBWORK_LIBRARIES} ${ROBWORKSIM_LIBRARIES})

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
endif ()
