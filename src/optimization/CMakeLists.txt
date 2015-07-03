SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/ObjectiveFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MultiObjectiveFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CombinedFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CombineObjectives.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CombineObjectivesFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CombineSum.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CombineProduct.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CombineLog.cpp
	CACHE INTERNAL ""
)
