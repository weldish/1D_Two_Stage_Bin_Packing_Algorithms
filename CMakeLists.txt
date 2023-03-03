cmake_minimum_required(VERSION 3.24)
project(1D_Two_Stage_Bin_Packing_Algorithms)

set(CMAKE_CXX_STANDARD 17)
set(lib_name "oneDpacking")
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
        src/1D_Algorithms/item_centric_algorithms/FFD_algorithm.cpp
        src/1D_Algorithms/item_centric_algorithms/BFD_algorithm.cpp
        src/1D_Algorithms/item_centric_algorithms/WFD_algorithm.cpp
        )

set(HEADER_ALGOS
        src/include/base_algorithm.h
        src/include/itemCentricAlgo.h
        src/include/FFD_algorithm.h
        src/include/BFD_algorithm.h
        src/include/WFD_algorithm.h
        )



add_library(${lib_name} STATIC
        ${HEADER_LIB}
        ${SOURCE_LIB}
        ${HEADER_ALGOS}
        ${SOURCE_ALGOS}
        )




include_directories(${lib_name})
# HEADER_LIB AND HEADER_ALGOS headers are associated with lib_name
#set_target_properties(${lib_name} PROPERTIES PUBLIC_HEADER "${HEADER_LIB};${HEADER_ALGOS}")

add_executable(1D_Two_Stage_Bin_Packing_Algorithms src/main.cpp)
target_link_libraries(1D_Two_Stage_Bin_Packing_Algorithms ${lib_name})

add_subdirectory(google_tests)