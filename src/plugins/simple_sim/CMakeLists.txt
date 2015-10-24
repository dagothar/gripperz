
#Target: simple_sim
SET(UisFiles simple_sim.ui)
SET(MocSrcFiles)
QT4_WRAP_CPP(MocSrcFiles simple_sim.hpp)
SET(RccSrcFiles)
QT4_ADD_RESOURCES(RccSrcFiles resources.qrc)
QT4_WRAP_UI(UiHeaders ${UisFiles})

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

add_library(simple_sim MODULE simple_sim.cpp ${MocSrcFiles}
${RccSrcFiles}
${UiHeaders})
add_dependencies(simple_sim gripperz)
target_link_libraries(simple_sim gripperz)
