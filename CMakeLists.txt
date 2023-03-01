cmake_minimum_required(VERSION 3.24)
project(1D_Two_Stage_Bin_Packing_Algorithms)

set(CMAKE_CXX_STANDARD 17)

# Get the oneDpacking libirary
set(HEADER_LIB
        src/include/bin.h
        src/include/item.h
        src/include/instance.h
        )

set(SOURCE_LIB
        src/lib/bin.cpp
        src/lib/item.cpp
        src/lib/instance.cpp
        )


set(SOURCE_ALGOS
        src/1D_Algorithms/base_algorithm.cpp
        src/1D_Algorithms/item_centric_algorithms/itemCentricAlgo.cpp
        )

set(HEADER_ALGOS
        src/include/base_algorithm.h
        src/include/itemCentricAlgo.h
        )



add_library(${lib_name} STATIC
        ${HEADER_LIB}
        ${SOURCE_LIB}
        ${HEADER_ALGOS}
        ${SOURCE_ALGOS}
        )



add_executable(1D_Two_Stage_Bin_Packing_Algorithms src/main.cpp)
include_directories(${include})
# HEADER_LIB AND HEADER_ALGOS headers are associated with lib_name
set_target_properties(${lib_name} PROPERTIES PUBLIC_HEADER "${HEADER_LIB};${HEADER_ALGOS}")



add_subdirectory(google_tests)