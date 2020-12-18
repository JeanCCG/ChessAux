cmake_minimum_required(VERSION 3.16.3)
project(ChessAux)

set(CMAKE_CXX_STANDARD 14)
include_directories($(CMAKE_CURRENT_SOURCE_DIR)/Librerias)

add_executable(Proyectos_en_Clion main.cpp
        Librerias/gameboard/gameboard.cpp
        Librerias/IA/IA.cpp
        Librerias/interface/interface.cpp
        Librerias/minMax/minMax.cpp
        Librerias/pieces/pieces.cpp)
