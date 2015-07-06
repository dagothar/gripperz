if (BUILD_SANDBOX)
	# Target: evaluate-gripper
	#add_executable(evaluate-gripper evaluateGripper.cpp)
	#add_dependencies(evaluate-gripper gripperz)
	#target_link_libraries(evaluate-gripper ode_plugin.rwplugin gripperz ${ROBWORK_LIBRARIES} ${ROBWORKSIM_LIBRARIES})
endif ()
