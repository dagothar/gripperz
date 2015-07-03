if (BUILD_TESTS)
	# Target: test-combine-objectives
	add_executable(test-combine-objectives testCombineObjectives.cpp)
	add_dependencies(test-combine-objectives gripperz)
	target_link_libraries(test-combine-objectives gripperz)
endif ()
