# =============================================================================
# Preamble
# =============================================================================
cmake_minimum_required(VERSION 3.11)
project(UCSP_Chess)

set(CMAKE_CXX_STANDARD 14)

# =============================================================================
# Build Types Configuration
# =============================================================================
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
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

# =============================================================================
# Targets
# =============================================================================

add_executable(
  ${PROJECT_NAME} "main.cpp" "src/gameboard/gameboard.cpp"
                  "src/Interface/Interface.cpp" "src/Piece/Piece.cpp")

# "src/IA/IA.cpp"

# cmake_minimum_required(VERSION 3.10) project(ncursed_example_project)

# # Set the C++ standard to C++11 set(CMAKE_CXX_STANDARD 11)

# # Find the ncursed library find_package(Curses REQUIRED) #
# include_directories(${CURSES_INCLUDE_DIR}) is this necessary?, idk, it works #
# without it.

# # Add the main.cpp file as an executable add_executable(ncursed_example
# 7_openai.cpp)

# # Link the ncursed library to the executable
# target_link_libraries(ncursed_example ${CURSES_LIBRARIES})
