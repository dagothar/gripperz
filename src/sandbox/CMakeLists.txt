if (BUILD_SANDBOX)
	# Target: param_landscape
	add_executable(param_landscape paramLandscape.cpp)
	add_dependencies(param_landscape gripperz)
	target_link_libraries(param_landscape gripperz)
endif ()
