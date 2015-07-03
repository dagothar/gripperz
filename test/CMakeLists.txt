if (BUILD_TESTS)
	# Target: test-combine-objectives
	add_executable(test-combine-objectives testCombineObjectives.cpp)
	add_dependencies(test-combine-objectives gripperz)
	target_link_libraries(test-combine-objectives gripperz)
	
	# Target: test-basic-simulator
	add_executable(test-basic-simulator testBasicSimulator.cpp)
	add_dependencies(test-basic-simulator gripperz)
	target_link_libraries(test-basic-simulator gripperz)
endif ()
