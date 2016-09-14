cmake_minimum_required (VERSION 3.0)

project (ObjParser)

add_library(ObjParser ObjParser.hpp ObjParser.cpp)
target_include_directories (ObjParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
