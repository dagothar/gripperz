
#Target: gripper_design
SET(UisFiles gripper_design.ui)
SET(MocSrcFiles)
QT4_WRAP_CPP(MocSrcFiles gripper_design.hpp)
SET(RccSrcFiles)
QT4_ADD_RESOURCES(RccSrcFiles resources.qrc)
QT4_WRAP_UI(UiHeaders ${UisFiles})

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

add_library(gripper_design MODULE gripper_design.cpp ${MocSrcFiles}
${RccSrcFiles}
${UiHeaders})
add_dependencies(gripper_design gripperz)
target_link_libraries(gripper_design gripperz)
