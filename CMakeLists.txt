project(minidump_test)

cmake_minimum_required(VERSION 3.10)

add_executable(minidump_test src/main.cpp)
target_link_libraries(minidump_test
    Dbghelp.lib
)
