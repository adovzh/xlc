cmake_minimum_required(VERSION 3.15)
project(xlc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(xlc)
add_subdirectory(xlctestapp)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR})