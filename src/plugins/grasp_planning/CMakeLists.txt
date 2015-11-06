
#Target: grasp_planning
SET(UisFiles grasp_planning.ui)
SET(MocSrcFiles)
QT4_WRAP_CPP(MocSrcFiles grasp_planning.hpp)
SET(RccSrcFiles)
QT4_ADD_RESOURCES(RccSrcFiles resources.qrc)
QT4_WRAP_UI(UiHeaders ${UisFiles})

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

add_library(grasp_planning MODULE grasp_planning.cpp ${MocSrcFiles}
${RccSrcFiles}
${UiHeaders})
add_dependencies(grasp_planning gripperz)
target_link_libraries(grasp_planning gripperz)
