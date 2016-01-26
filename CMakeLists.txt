
cmake_minimum_required (VERSION 2.8)
project (example-httpserver)
add_definitions(-DNOMINMAX) # to force Visual Studio to not generate its own std::min, std::max
add_definitions(-Wfatal-errors)
#add_definitions(-std=c++11)

set(CMAKE_CXX "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -Wall -O2")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -Wall -g -O0 -pg -ftest-coverage -fprofile-arcs")


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/fir")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/" )


set (SRC_HTTPSERVER
	httpserver/main.cpp
)

add_executable(httpserver ${SRC_HTTPSERVER})
target_link_libraries(httpserver pthread fir)
