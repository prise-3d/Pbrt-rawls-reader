add_library(rawls rawls.h rawls.cpp)
add_library(rawls_v1 rawls_v1.h rawls_v1.cpp)

target_link_libraries(rawls LINK_PUBLIC lodepng)
target_link_libraries(rawls LINK_PUBLIC stdc++fs)

target_link_libraries(rawls_v1 LINK_PUBLIC lodepng)
target_link_libraries(rawls_v1 LINK_PUBLIC stdc++fs)

target_include_directories (rawls PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})