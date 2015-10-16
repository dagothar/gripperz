SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/Types.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Gripper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/WorkcellGripper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParallelFingerGripper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperQuality.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperQualityImpl.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperQualityFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OldGripper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OldGripperQuality.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GripperBuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MapGripperBuilder.cpp
	CACHE INTERNAL ""
)
