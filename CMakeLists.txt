
cmake_minimum_required(VERSION 3.20)

project(AILab2Minimax LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(GameTree STATIC src/GameTree.cpp)
target_include_directories(GameTree PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(Minimax STATIC src/Minimax.cpp)
target_include_directories(Minimax PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(Minimax PUBLIC GameTree)

add_library(AlphaBeta STATIC src/AlphaBeta.cpp)
target_include_directories(AlphaBeta PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(AlphaBeta PUBLIC GameTree)

add_library(Benchmark STATIC src/Benchmark.cpp)
target_include_directories(Benchmark PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(Benchmark PUBLIC Minimax AlphaBeta)

add_executable(AILab2 src/main.cpp)
target_link_libraries(AILab2 PRIVATE Benchmark)

include(CTest)

if(BUILD_TESTING)
    add_executable(GameTreeTests tests/GameTreeTests.cpp)
    target_link_libraries(GameTreeTests PRIVATE GameTree)
    add_test(NAME GameTreeTests COMMAND GameTreeTests)

    add_executable(MinimaxTests tests/MinimaxTests.cpp)
    target_link_libraries(MinimaxTests PRIVATE Minimax)
    add_test(NAME MinimaxTests COMMAND MinimaxTests)

    add_executable(AlphaBetaTests tests/AlphaBetaTests.cpp)
    target_link_libraries(AlphaBetaTests PRIVATE Minimax AlphaBeta)
    add_test(NAME AlphaBetaTests COMMAND AlphaBetaTests)

    add_executable(TraversalTests tests/TraversalTests.cpp)
    target_link_libraries(TraversalTests PRIVATE Minimax AlphaBeta)
    add_test(NAME TraversalTests COMMAND TraversalTests)

    add_executable(VerificationTests tests/VerificationTests.cpp)
    target_link_libraries(VerificationTests PRIVATE Minimax AlphaBeta)
    add_test(NAME VerificationTests COMMAND VerificationTests)

    add_executable(BenchmarkTests tests/BenchmarkTests.cpp)
    target_link_libraries(BenchmarkTests PRIVATE Benchmark)
    add_test(NAME BenchmarkTests COMMAND BenchmarkTests)
endif()
