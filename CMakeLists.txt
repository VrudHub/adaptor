cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(proj_name "adaptor")
project(${proj_name})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -g")

add_subdirectory(src)
