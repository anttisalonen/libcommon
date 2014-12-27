cmake_minimum_required (VERSION 2.6)
project (common)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wall")
find_package(SDL REQUIRED)
find_package(SDL_ttf REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
add_library(common TextRenderer.cpp DriverFramework.cpp SDLSurface.cpp
	     Texture.cpp SDL_utils.cpp Color.cpp Math.cpp Clock.cpp
	     Steering.cpp Random.cpp Matrix22.cpp Matrix44.cpp Quaternion.cpp
	     Line.cpp Geometry.cpp)
add_executable(common_test GeometryTest.cpp QuadtreeTest.cpp MathTest.cpp test.cpp)
target_link_libraries(common_test common)

install (TARGETS common DESTINATION lib)
install (FILES AStar.h Color.h FontConfig.h LineQuadTree.h Matrix44.h Quaternion.h SDLSurface.h Steering.h Vector2.h
	CellSpacePartition.h DriverFramework.h Geometry.h Math.h Partition.h Random.h SDL_utils.h TextRenderer.h Vector3.h
	Clock.h Entity.h Line.h Matrix22.h QuadTree.h Rectangle.h Serialization.h Texture.h Vehicle.h DESTINATION include/common)
