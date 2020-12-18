# cmake_minimum_required(VERSION <specify CMake version here>)
project(Proyectos_en_Clion)

set(CMAKE_CXX_STANDARD 14)

add_executable(Proyectos_en_Clion main.cpp
        Librerias/gameboard/gameboard.cpp
        Librerias/IA/IA.cpp
        Librerias/interface/interface.cpp
        Librerias/pieces/pieces.cpp)