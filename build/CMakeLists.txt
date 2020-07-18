cmake_minimum_required(VERSION 3.10)

project(OSMPoc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_filename_component(PROJ_ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set(SRC_DIR "${PROJ_ROOT}/src")
set(BIN_DIR "${PROJ_ROOT}/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

add_compile_options(
    -g
    -Wall
    -Werror
)

add_executable(OSMPoc "${SRC_DIR}/main.cpp")

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(OSMPoc -lOSMesa ${PNG_LIBRARY})