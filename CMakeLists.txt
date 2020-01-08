# CMakeLists files in this project can
# refer to the root source directory of the project as ${rawls-tools_SOURCE_DIR} and
# to the root binary directory of the project as ${rawls-tools_BINARY_DIR}.
cmake_minimum_required (VERSION 3.10.2)
project(rawls-tools)

add_subdirectory(lodepng)
add_subdirectory(rawls)
add_subdirectory(main)
