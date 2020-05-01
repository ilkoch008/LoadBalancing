
cmake_minimum_required(VERSION 3.15)
project(sumOfBigNumbers C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)

add_executable(sumOfBigNumbers main.c lol.h misc.c static.c dynamic.c)

target_link_libraries(sumOfBigNumbers ${MPI_LIBRARIES})

include_directories(${MPI_INCLUDE_PATH})

if(MPI_COMPILE_FLAGS)
    set_target_properties(sumOfBigNumbers PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
    set_target_properties(sumOfBigNumbers PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()