cmake_minimum_required(VERSION 3.17)
project(ChessAux)

set(CMAKE_CXX_STANDARD 14)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(ChessAux main.cpp Librerias/Graphics/Game.cpp Librerias/Graphics/TextureManager.cpp Librerias/Graphics/GameObject.cpp Librerias/Graphics/Map.cpp Librerias/Graphics/Moves.cpp)

target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image)
