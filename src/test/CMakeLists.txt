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

# Target: test_ParametrizationTranslator
add_executable(test_ParametrizationTranslator test_ParametrizationTranslator.cpp)
target_link_libraries(test_ParametrizationTranslator gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_ParametrizationTranslator gripperz)

# Target: test_BasicParametrizedGripper
add_executable(test_BasicParametrizedGripper test_BasicParametrizedGripper.cpp)
target_link_libraries(test_BasicParametrizedGripper gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_BasicParametrizedGripper gripperz)
        
endif ()
