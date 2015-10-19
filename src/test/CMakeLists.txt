if (BUILD_TESTS)

FIND_PACKAGE(Boost COMPONENTS unit_test_framework REQUIRED)
                
# Target: test_beam
add_executable(test_beam testBeam.cpp)
target_link_libraries(test_beam gripperz ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_dependencies(test_beam gripperz)
        
endif ()
