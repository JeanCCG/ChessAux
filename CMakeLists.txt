# =============================================================================
# Preamble
# =============================================================================
cmake_minimum_required(VERSION 3.11)
project(
  UCSP_Chess
  VERSION 2.0.0
  DESCRIPTION "c++ hello world library"
  HOMEPAGE_URL https://github.com/JeanCCG/ChessAux)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# option(BUILD_DOC "Build Documentation" ON)

# set(CMAKE_CXX_COMPILER "/usr/bin/clang++-10")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11") # for using enum

# set(PROJECT_SOURCE_DIR "src")

# add_subdirectory(${PROJECT_SOURCE_DIR})

# if(BUILD_DOC) add_subdirectory(docs) endif()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})

# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# include(CPack)

# cmake_export_compile_commands()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
# Build Types Configuration
# =============================================================================

#[[
g++ -Wall -Wextra -Wpedantic -Werror \
	-pthread \
	-fsanitize=address \
	-O0 -g \
	-fsanitize=address \
	-fsanitize-address-use-after-scope \
	-fsanitize=undefined \
	-fsanitize=thread \
	-fno-sanitize-recover \
	-fstack-protector \
	-fstack-check \
	-fstack-protector-strong \
	-D_FORTIFY_SOURCE=2 \
	-o your_executable source_file.cpp
#]]

if(CMAKE_CONFIGURATION_TYPES)
  if(NOT "Profile" IN_LIST CMAKE_CONFIGURATION_TYPES)
    list(APPEND CMAKE_CONFIGURATION_TYPES Profile)
  endif()
else()
  set(allowableBuildTypes Debug Release Profile)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowableBuildTypes}")
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        Debug
        CACHE STRING "" FORCE)
    message("debug build type will be used by default")
  elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowableBuildTypes)
    message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
  endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("adding debug build type configuration")
  add_compile_options(-Wall -Wextra -Wpedantic)
  add_compile_options(-Wimplicit-fallthrough -Wsign-conversion)
  add_compile_options(-pthread) # Enable multithreading
  add_compile_options(-fsanitize=address) # ASAN
  link_libraries(-fsanitize=address)
  # add_link_options(-fsanitize=address)
  add_compile_options(-fsanitize=undefined)
  link_libraries(ubsan)
  add_compile_options(-O0 -g) # disable optimizations and keep symbols
  add_compile_options(-fsanitize-address-use-after-scope) # better ASAN and

  # link_libraries(-fsanitize=address) add_compile_options(-fsanitize=thread) #
  # Thread sanitizer

  # add_compile_options(-fstack-protector -fstack-check -fstack-protector-all)
  add_compile_options(-fstack-protector-all)
  # add_compile_definitions(-D_FORTIFY_SOURCE=2)

  # IF COMPILER == GCC
  add_compile_options(-fdiagnostics-color=always)
  # add_compile_options(-fdiagnostics-format=pretty)# this didn't work
  #[[
  #IF COMPILER == Clang
  add_compile_options(-fcolor-diagnostics -fpretty-diagnostics)
  ]]
  # Make ASAN output pretty

  #[[add_compile_options(--pretty-print)
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --pretty-print")



  ]]
  #
  #[[Make ASAN output pretty through asan_symbolize
  ]]
  set(ENV{ASAN_SYMBOLIZER_PATH} "/usr/bin/asan_symbolize")
  set(ENV{ASAN_OPTIONS} "symbolize=1")
  #[[ GARBAGE
  set(ASAN_SYMBOLIZER_PATH "/usr/bin/asan_symbolize")
  ]]
endif()

# =============================================================================
# Libraries
# =============================================================================

# # Link against Conan-installed dependencies

# target_link_libraries(YourProject PRIVATE ${CONAN_LIBS})

# add_subdirectory("/home/marcus/cpp-tools/benchmark")

# =============================================================================
# Variables
# =============================================================================

set(SOURCES
    # "main.cpp"
    "src/Gameboard/Gameboard.cpp"
    "src/Game_settings/Game_settings.cpp"
    "src/Move/Move.cpp"
    "src/Bearing/Bearing.cpp"
    "src/Piece/Piece.cpp"
    "src/enums/enums.cpp"
    "src/Map/Map.cpp"
    "src/Score/Score.cpp"
    # Interface
    "src/Interface/Interface.cpp"
    "src/Interface/display_gameboard.cpp"
    "src/Interface/print_title.cpp"
    "src/Interface/get_user_key_input.cpp"
    "src/Interface/main_interface.cpp"
    "src/Interface/game_results_interface.cpp"
    "src/Interface/human_vs_computer_interface.cpp"
    "src/Interface/human_vs_human_interface.cpp")

# bad practice, but easy to implement command.

# file( GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# discussion stack overflow post
# https://stackoverflow.com/questions/2110795/how-to-use-all-c-files-in-a-directory-with-the-cmake-build-system

# =============================================================================
# Targets
# =============================================================================

add_executable(${PROJECT_NAME} "main.cpp" ${SOURCES})
# add_executable(testing "gb_test_destroyer.cpp" ${SOURCES})

# add_executable(${PROJECT_NAME} "main.cpp" ${SOURCES})

# =============================================================================
# Linking ncurses library to main
# =============================================================================

# # Find the ncursed library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR}) # is this necessary?, idk, it works
                                           # without it

# # Link the ncursed library to the executable
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
# target_link_libraries(testing ${CURSES_LIBRARIES})
