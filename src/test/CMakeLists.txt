if (BUILD_TESTS)
	FIND_PACKAGE(Boost COMPONENTS unit_test_framework REQUIRED)

	# Target: test_combine_objectives
	add_executable(test_combine_objectives testCombineObjectives.cpp)
	add_dependencies(test_combine_objectives gripperz)
	target_link_libraries(test_combine_objectives gripperz)
	
	# Target: test_basic_simulator
	add_executable(test_basic_simulator testBasicSimulator.cpp)
	add_dependencies(test_basic_simulator gripperz)
	target_link_libraries(test_basic_simulator gripperz)
	
	# Target: test_dlib_rosenbrock
	add_executable(test_dlib_rosenbrock testDlibRosenbrock.cpp)
	add_dependencies(test_dlib_rosenbrock gripperz)
	target_link_libraries(test_dlib_rosenbrock gripperz)
	
	# Target: test_bobyqa
	add_executable(test_bobyqa testBOBYQA.cpp)
	add_dependencies(test_bobyqa gripperz)
	target_link_libraries(test_bobyqa gripperz)
	
	# Target: test_math
	add_executable(test_math testMath.cpp)
	target_link_libraries(test_math gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
	add_dependencies(test_math gripperz)
	
	# Target: test_gripper_objective
	add_executable(test_gripper_objective testGripperObjective.cpp)
	add_dependencies(test_gripper_objective gripperz)
	target_link_libraries(test_gripper_objective gripperz)
	
	# Target: test_gripper_evaluator
	add_executable(test_gripper_evaluator testGripperEvaluator.cpp)
	add_dependencies(test_gripper_evaluator gripperz)
	target_link_libraries(test_gripper_evaluator gripperz)
	
	# Target: test_robust_evaluation
	add_executable(test_robust_evaluation testRobustEvaluation.cpp)
	add_dependencies(test_robust_evaluation gripperz)
	target_link_libraries(test_robust_evaluation gripperz)
endif ()
