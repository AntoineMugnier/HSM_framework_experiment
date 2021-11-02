#project setup
cmake_minimum_required(VERSION 3.16)
project(H735_SW CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Og -g)

add_subdirectory(KaoriFramework)
add_subdirectory(Examples)
