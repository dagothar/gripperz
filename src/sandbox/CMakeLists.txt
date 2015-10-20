if (BUILD_SANDBOX)

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
    
endif()
