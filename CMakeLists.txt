cmake_minimum_required(VERSION 3.5)
project(OpenAS C CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

find_package(OpenCL REQUIRED)

set(HEADERS
	src/as.h
	src/autm.h
	src/vivs.h
	src/neighbor.h
	src/pos.h
)

include_directories(${OpenCL_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/src")

add_executable(OpenASTest ${SOURCES} ${HEADERS} src/test/test.cpp src/test/copit.h)
target_link_libraries(OpenASTest ${OpenCL_LIBRARIES})
if(MINGW)
	target_compile_options(OpenASTest PUBLIC -gdwarf-2)
else()
	target_compile_options(OpenASTest PUBLIC -g)
endif(MINGW)
