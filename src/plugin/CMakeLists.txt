if (BUILD_PLUGIN)
	# Target: GraspPlugin
	SET(UisFiles GraspPlugin.ui DesignDialog.ui TaskDialog.ui)
	SET(MocSrcFiles )
	QT4_WRAP_CPP(MocSrcFiles GraspPlugin.hpp DesignDialog.hpp TaskDialog.hpp)
	SET(RccSrcFiles)
	QT4_ADD_RESOURCES(RccSrcFiles resources.qrc)
	QT4_WRAP_UI(UiHeaders ${UisFiles})
	
	SET(CMAKE_INCLUDE_CURRENT_DIR ON)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

	add_library(GraspPlugin MODULE GraspPlugin.cpp DesignDialog.cpp TaskDialog.cpp ${MocSrcFiles} ${RccSrcFiles} ${UiHeaders})
	add_dependencies(GraspPlugin gripperz)
	target_link_libraries(GraspPlugin gripperz)
endif ()
