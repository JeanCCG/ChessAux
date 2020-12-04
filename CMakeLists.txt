cmake_minimum_required(VERSION 3.16.3)
project(Chess_UCSP)

set(CMAKE_CXX_STANDARD 14)

include_directories($(CMAKE_CURRENT_SOURCE_DIR)/Librerias)
add_executable(Chess_UCSP main.cpp)
