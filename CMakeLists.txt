# CMakeLists files in this project can
# refer to the root source directory of the project as ${rawls-reader_SOURCE_DIR} and
# to the root binary directory of the project as ${rawls-reader_BINARY_DIR}.
cmake_minimum_required (VERSION 3.16.2)
project(rawls-reader)

add_subdirectory(lodepng)
add_subdirectory(rawls)
add_subdirectory(main)