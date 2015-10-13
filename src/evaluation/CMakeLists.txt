SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/Types.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OldGripperEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ConstrainedGripperEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperObjectiveFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AlignmentCalculator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StablePoseAlignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AllPosesAlignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VersorAlignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SimpleAlignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ClusteringAlignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperQualityExtractor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/QualityIndexCalculator.cpp
	CACHE INTERNAL ""
)

ADD_SUBDIRECTORY(calculators)