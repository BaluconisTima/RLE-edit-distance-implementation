cmake_minimum_required(VERSION 3.31)
project(praca_roczna)

set(CMAKE_CXX_STANDARD 20)

add_executable(praca_roczna naive_dp.cpp
        common_utils.cpp
        chen_chao_naive.cpp
        gridTable.cpp
        minQueue.cpp
        main.cpp
        chen_chao_advance.cpp
        GeometricView.cpp
        "binaryTreeRepresentation.cpp"
        BstNode.cpp
        near_optimal_solution.cpp
)
