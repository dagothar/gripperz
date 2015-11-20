if (BUILD_OLD)
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
	
	# Target: gripper_space
	add_executable(gripper_space gripper_space.cpp)
	add_dependencies(gripper_space gripperz)
	target_link_libraries(gripper_space gripperz)
	
	# Target: gripper_slice
	add_executable(gripper_slice gripper_slice.cpp)
	add_dependencies(gripper_slice gripperz)
	target_link_libraries(gripper_slice gripperz)
	
	# Target: filter_tasks
	add_executable(filter_tasks filter_tasks.cpp)
	add_dependencies(filter_tasks gripperz)
	target_link_libraries(filter_tasks gripperz)
                
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
                
        # Target: generate-surface-samples
#	add_executable(generate-surface-samples generateSurfaceSamples.cpp)
#	add_dependencies(generate-surface-samples gripperz)
#	target_link_libraries(generate-surface-samples gripperz)
	
	# Target: gripper_landscape
	#add_executable(gripper_landscape gripper_landscape.cpp)
	#add_dependencies(gripper_landscape gripperz)
	#target_link_libraries(gripper_landscape gripperz)
	
	# Target: generate_grippers
	#add_executable(generate_grippers generate_grippers.cpp)
	#add_dependencies(generate_grippers gripperz)
	#target_link_libraries(generate_grippers gripperz)
	
	# Target: gripper_optimizer
	#add_executable(gripper_optimizer gripper_optimizer.cpp)
	#add_dependencies(gripper_optimizer gripperz)
	#target_link_libraries(gripper_optimizer gripperz)
	
	# Target: gripper_evaluate
	#add_executable(gripper_evaluate gripper_evaluate.cpp)
	#add_dependencies(gripper_evaluate gripperz)
	#target_link_libraries(gripper_evaluate gripperz)
endif ()
