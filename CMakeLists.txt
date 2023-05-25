cmake_minimum_required (VERSION 3.8)

project ("sandbox")

add_subdirectory ("src")
target_include_directories(program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
