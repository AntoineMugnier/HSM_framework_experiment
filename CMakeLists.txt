cmake_minimum_required(VERSION 3.17)
project(Kaori_HSM C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)


#General compile options
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
#C++ specific compile options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-threadsafe-statics")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(Kaori_Framework Kaori_Tests_Files)

add_link_options(-Wl,-gc-sections)
add_executable(HFSM_Windows main.cpp)
