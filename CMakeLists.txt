cmake_minimum_required(VERSION 3.0)

project(DBCS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include /usr/local/opengauss/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)

add_compile_options(-std=c++23 -Wall -g)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

link_directories(/usr/local/opengauss/lib)

link_libraries(pq)

add_executable(DBCS ${SRC_LIST} main.cpp)