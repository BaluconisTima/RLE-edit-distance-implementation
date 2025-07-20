cmake_minimum_required(VERSION 3.31)
project(praca_roczna)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:536870912") # 512 MB

set(CMAKE_CXX_STANDARD 20)

add_executable(praca_roczna solutions/naive_dp.cpp
        common/utils.cpp
        solutions/chen_chao_naive.cpp
        structures/gridTable.cpp
        structures/minQueue.cpp
        main.cpp
        solutions/chen_chao_advance.cpp
        structures/GeometricView.cpp
        "structures/BST/BSTView.cpp"
        structures/BST/BstNode.cpp
        solutions/near_optimal.cpp
        common/testing/geometric_tests.cpp
)
