SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/Optimizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OptimizerFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BOBYQAOptimizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SimplexOptimizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CoordinateDescentOptimizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SimulatedAnnealingOptimizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PowellOptimizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OptimizationManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperOptimizationManager.cpp
	CACHE INTERNAL ""
)
