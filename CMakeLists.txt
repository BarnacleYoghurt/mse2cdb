cmake_minimum_required(VERSION 3.7)
project(mse2cdb)

set(CMAKE_CXX_STANDARD 11)

find_package(sqlite3 REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(mse2cdb ${SOURCE_FILES})