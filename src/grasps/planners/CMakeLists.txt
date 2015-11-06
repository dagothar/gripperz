SET(SRC_FILES
	${SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/GraspPlanner.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BasicParallelGripperGraspPlanner.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParallelGripperGraspPlanner.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GraspSampler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SurfaceSampleGraspSampler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RayGraspSampler.cpp
	CACHE INTERNAL ""
)
