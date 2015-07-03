CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

PROJECT(gripperz)

## Options
OPTION(BUILD_TESTS "Build tests" ON)
OPTION(BUILD_TOOLS "Build tools" ON)
OPTION(BUILD_PLUGIN "Build plugin" ON)
OPTION(BUILD_DOCS "Build documentation" ON)

# Used to resolve absolute path names
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

SET(RW_ROOT "$ENV{RW_ROOT}")
SET(RWS_ROOT "$ENV{RWS_ROOT}")
SET(RWSIM_ROOT "$ENV{RWSIM_ROOT}")
SET(RobWork_DIR ${RW_ROOT}/cmake)
SET(RobWorkStudio_DIR ${RWS_ROOT}/cmake)
SET(RobWorkSim_DIR ${RWSIM_ROOT}/cmake)
SET(CMAKE_BUILD_TYPE Release)

#Include default settings for constructing a robwork dependent project
FIND_PACKAGE(RobWork REQUIRED)
FIND_PACKAGE(RobWorkStudio REQUIRED)
FIND_PACKAGE(RobWorkSim REQUIRED)
FIND_PACKAGE(ODE)

INCLUDE_DIRECTORIES(
	${ROBWORK_INCLUDE_DIRS}
	${ROBWORKSTUDIO_INCLUDE_DIRS}
	${ROBWORKSIM_INCLUDE_DIRS}
	src
	ext
)
LINK_DIRECTORIES(
	${ROBWORK_LIBRARY_DIRS}
	${ROBWORKSTUDIO_LIBRARY_DIRS}
	${ROBWORKSIM_LIBRARY_DIRS}
)

# Set the output dir for generated libraries and binaries
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin" CACHE PATH "Runtime directory" FORCE)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT}/libs" CACHE PATH "Library directory" FORCE)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ROOT}/libs" CACHE PATH "Archive directory" FORCE)

# Target: gripperz
SET(SRC_FILES CACHE INTERNAL "" FORCE)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

add_library(gripperz ${SRC_FILES})
target_link_libraries(gripperz -lGL -lGLU -lglut -lm ${ROBWORKSTUDIO_LIBRARIES} ${ROBWORK_LIBRARIES} ${ROBWORKSIM_LIBRARIES})

# Target: docs
IF(BUILD_DOCS)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		add_custom_target(doc #ALL
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	endif(DOXYGEN_FOUND)
ENDIF()

