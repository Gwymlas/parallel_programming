cmake_minimum_required(VERSION 3.23)
project(MPI_parallel)

set(CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)

add_executable(MPI_parallel main.cpp)

target_link_libraries(MPI_parallel PUBLIC MPI::MPI_CXX)

