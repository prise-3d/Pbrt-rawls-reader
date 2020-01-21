add_library(rawls rawls.h rawls.cpp)

target_link_libraries(rawls LINK_PUBLIC lodepng)
target_link_libraries(rawls LINK_PUBLIC stdc++fs)

target_include_directories (rawls PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})