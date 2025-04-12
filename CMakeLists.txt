cmake_minimum_required(VERSION 3.30.5)

project(Knapsack)

set(CMAKE_CXX_STANDARD 14)
set( CMAKE_BUILD_TYPE_TMP "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Release" )

add_executable(Knapsack main.cpp

)