if (BUILD_SANDBOX)
    
    SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/GripperEvaluationProcessManager.cpp
    )

    #Target: try_contour
    add_executable(try_contour try_contour.cpp)
    add_dependencies(try_contour gripperz)
    target_link_libraries(try_contour gripperz)

    #Target: try_slice
    add_executable(try_slice try_slice.cpp)
    add_dependencies(try_slice gripperz)
    target_link_libraries(try_slice gripperz)

    #Target: try_finger
    add_executable(try_finger try_finger.cpp)
    add_dependencies(try_finger gripperz)
    target_link_libraries(try_finger gripperz)

    #Target: try_process
    add_executable(try_process try_process.cpp)
    add_dependencies(try_process gripperz)
    target_link_libraries(try_process gripperz)

    #Target: try_parametrized_geometry
    add_executable(try_parametrized_geometry try_parametrized_geometry.cpp)
    add_dependencies(try_parametrized_geometry gripperz)
    target_link_libraries(try_parametrized_geometry gripperz)

    #Target: try_openscad
    add_executable(try_openscad try_openscad.cpp)
    add_dependencies(try_openscad gripperz)
    target_link_libraries(try_openscad gripperz)

    #Target: try_pointers
    #add_executable(try_pointers try_pointers.cpp)
    #add_dependencies(try_pointers gripperz)
    #target_link_libraries(try_pointers gripperz)

    #Target: try_shards
    add_executable(try_shards try_shards.cpp)
    add_dependencies(try_shards gripperz)
    target_link_libraries(try_shards gripperz)

    #Target: try_rbf
    add_executable(try_rbf try_rbf.cpp)
    add_dependencies(try_rbf gripperz)
    target_link_libraries(try_rbf gripperz)

    #Target: rwtask_to_csv
    add_executable(rwtask_to_csv rwtask_to_csv.cpp)
    target_link_libraries(rwtask_to_csv ${ROBWORK_LIBRARIES})

    #Target: random_csv
    add_executable(random_csv random_csv.cpp)
    target_link_libraries(random_csv ${ROBWORK_LIBRARIES})
    
endif()
