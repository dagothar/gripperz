SET(SRC_FILES
	${SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/SuccessIndexCalculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CoverageIndexCalculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AlignmentIndexCalculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WrenchIndexCalculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/VolumeIndexCalculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/StressIndexCalculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RobustnessIndexCalculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SignificanceIndexCalculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/VarianceAlignmentCalculator.cpp
	CACHE INTERNAL ""
)
