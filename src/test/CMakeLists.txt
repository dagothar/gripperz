if (BUILD_TESTS)

FIND_PACKAGE(Boost COMPONENTS unit_test_framework REQUIRED)
                
# Target: test_Beam
add_executable(test_Beam test_Beam.cpp)
target_link_libraries(test_Beam gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_Beam gripperz)

# Target: test_Parametrization
add_executable(test_Parametrization test_Parametrization.cpp)
target_link_libraries(test_Parametrization gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_Parametrization gripperz)

# Target: test_ParametrizationLoader
add_executable(test_ParametrizationLoader test_ParametrizationLoader.cpp)
target_link_libraries(test_ParametrizationLoader gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_ParametrizationLoader gripperz)

# Target: test_GripperQualityLoader
add_executable(test_GripperQualityLoader test_GripperQualityLoader.cpp)
target_link_libraries(test_GripperQualityLoader gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_GripperQualityLoader gripperz)

# Target: test_GripperLoader
add_executable(test_GripperLoader test_GripperLoader.cpp)
target_link_libraries(test_GripperLoader gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_GripperLoader gripperz)

# Target: test_ParametrizationTranslator
add_executable(test_ParametrizationTranslator test_ParametrizationTranslator.cpp)
target_link_libraries(test_ParametrizationTranslator gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_ParametrizationTranslator gripperz)

# Target: test_BasicParametrizedGripper
add_executable(test_BasicParametrizedGripper test_BasicParametrizedGripper.cpp)
target_link_libraries(test_BasicParametrizedGripper gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_BasicParametrizedGripper gripperz)

# Target: test_Shard
add_executable(test_Shard test_Shard.cpp)
target_link_libraries(test_Shard gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_Shard gripperz)
        
# Target: test_ShardContainer
add_executable(test_ShardContainer test_ShardContainer.cpp)
target_link_libraries(test_ShardContainer gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_ShardContainer gripperz)
        
endif ()
