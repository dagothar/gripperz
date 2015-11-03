SET(SRC_FILES
	${SRC_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/JawPrimitive.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Beam.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EulerBeam.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BeamFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MeshGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParametrizedMeshGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OldFingerGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OpenSCADGenerator.cpp
	CACHE INTERNAL ""
)

add_subdirectory(loaders)