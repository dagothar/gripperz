
#Target: alignment_experiment
SET(UisFiles alignment_experiment.ui)
SET(MocSrcFiles)
QT4_WRAP_CPP(MocSrcFiles alignment_experiment.hpp)
SET(RccSrcFiles)
QT4_ADD_RESOURCES(RccSrcFiles resources.qrc)
QT4_WRAP_UI(UiHeaders ${UisFiles})

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

add_library(alignment_experiment MODULE alignment_experiment.cpp ${MocSrcFiles}
${RccSrcFiles}
${UiHeaders})
add_dependencies(alignment_experiment gripperz)
target_link_libraries(alignment_experiment gripperz)
