SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/Parametrization.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParametrizationTranslator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VectorParametrizationTranslator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MultiParametrizationTranslator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Parametrized.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParametrizedObjective.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParametrizedObjectiveFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParametrizedMultiObjectiveFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Types.cpp
	CACHE INTERNAL ""
)

ADD_SUBDIRECTORY(loaders)