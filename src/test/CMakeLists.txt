if (BUILD_TESTS)
	FIND_PACKAGE(Boost COMPONENTS unit_test_framework REQUIRED)

	# Target: test-combine-objectives
	add_executable(test-combine-objectives testCombineObjectives.cpp)
	add_dependencies(test-combine-objectives gripperz)
	target_link_libraries(test-combine-objectives gripperz)
	
	# Target: test-basic-simulator
	add_executable(test-basic-simulator testBasicSimulator.cpp)
	add_dependencies(test-basic-simulator gripperz)
	target_link_libraries(test-basic-simulator gripperz)
	
	# Target: test-dlib-rosenbrock
	add_executable(test-dlib-rosenbrock testDlibRosenbrock.cpp)
	add_dependencies(test-dlib-rosenbrock gripperz)
	target_link_libraries(test-dlib-rosenbrock gripperz)
	
	# Target: test_bobyqa
	add_executable(test_bobyqa testBOBYQA.cpp)
	add_dependencies(test_bobyqa gripperz)
	target_link_libraries(test_bobyqa gripperz)
	
	# Target: test-optimization
	add_executable(test-optimization testOptimization.cpp)
	target_link_libraries(test-optimization gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
	add_dependencies(test-optimization gripperz)
	
	# Target: test-gripper-objective
	add_executable(test-gripper-objective testGripperObjective.cpp)
	add_dependencies(test-gripper-objective gripperz)
	target_link_libraries(test-gripper-objective gripperz)
	
	# Target: test-gripper-evaluator
	add_executable(test-gripper-evaluator testGripperEvaluator.cpp)
	add_dependencies(test-gripper-evaluator gripperz)
	target_link_libraries(test-gripper-evaluator gripperz)
endif ()
