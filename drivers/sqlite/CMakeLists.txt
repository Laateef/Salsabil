cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include ${CMAKE_SOURCE_DIR}/backends/sqlite3/)

add_library(sqlite_driver src/Exception.cpp src/SqliteDriver.cpp)