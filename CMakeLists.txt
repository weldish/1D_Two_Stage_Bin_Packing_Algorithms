cmake_minimum_required(VERSION 3.24)
project(1D_Two_Stage_Bin_Packing_Algorithms)

set(CMAKE_CXX_STANDARD 17)

# Get the oneDpacking libirary
set(HEADER_LIB
        src/lib/bin.h
        src/lib/item.h
        src/lib/instance.h
        )

set(SOURCE_LIB
        src/lib/bin.cpp
        src/lib/item.cpp
        src/lib/instance.cpp
        )

add_library(${lib_name} STATIC
        ${HEADER_LIB}
        ${SOURCE_LIB}
        )


add_executable(1D_Two_Stage_Bin_Packing_Algorithms src/main.cpp)

add_subdirectory(google_tests)