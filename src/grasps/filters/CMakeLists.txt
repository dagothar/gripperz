SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/GraspFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GraspFilterChain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/KDGraspFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GraspStatusFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GraspOffsetFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GraspGridFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RobustnessGraspFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ClearStatusFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SortingFilter.cpp
	CACHE INTERNAL ""
)
