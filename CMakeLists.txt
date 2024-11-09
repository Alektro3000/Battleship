cmake_minimum_required(VERSION 3.25.0)
cmake_policy(SET CMP0167 NEW)
project(Battleship)

find_package(Catch2 3 REQUIRED)
find_package(Boost 1.86.0 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(tests Battleship.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)

add_executable(main Battleship.cpp)