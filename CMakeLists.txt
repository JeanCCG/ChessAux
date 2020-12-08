cmake_minimum_required(VERSION 3.17)
project(Proyecto_de_prueba)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(Proyecto_de_prueba main.cpp Librerias/Graphics/Game.cpp Librerias/Graphics/TextureManager.cpp Librerias/Graphics/GameObject.cpp Librerias/Graphics/Map.cpp Librerias/Graphics/Moves.cpp)

target_link_libraries(Proyecto_de_prueba ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image)