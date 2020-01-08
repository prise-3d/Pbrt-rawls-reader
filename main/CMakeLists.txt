add_executable(rawls_convert rawls_convert.cpp)
add_executable(rawls_merge rawls_merge.cpp)
add_executable(rawls_h_flip rawls_h_flip.cpp)

target_link_libraries(rawls_convert LINK_PUBLIC rawls)
target_link_libraries(rawls_merge LINK_PUBLIC rawls)
target_link_libraries(rawls_h_flip LINK_PUBLIC rawls)

set_property(TARGET rawls_merge PROPERTY CXX_STANDARD 17)
set_property(TARGET rawls_convert PROPERTY CXX_STANDARD 17)
set_property(TARGET rawls_h_flip PROPERTY CXX_STANDARD 17)