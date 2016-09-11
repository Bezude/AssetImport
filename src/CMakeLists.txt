cmake_minimum_required (VERSION 3.0)

project (testExec)

set(BOOST_DIR /Users/benroye/forge/boost_1_61_0)

add_executable(testExec test.cpp Objparser.hpp Objparser.cpp)
target_include_directories(testExec PRIVATE ${BOOST_DIR})
