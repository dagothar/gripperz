SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/GripperEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ConstrainedGripperEvaluator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperObjectiveFunction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperEvaluationManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StandardEvaluationManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperEvaluationManagerFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperEvaluationManagerBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AlignmentCalculator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StablePoseAlignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AllPosesAlignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/VersorAlignment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperQualityExtractor.cpp
	CACHE INTERNAL ""
)
