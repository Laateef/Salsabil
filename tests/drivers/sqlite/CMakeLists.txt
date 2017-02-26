cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
${gtest_SOURCE_DIR} 
${gtest_SOURCE_DIR}/include
${CMAKE_SOURCE_DIR}/drivers/sqlite/include
)

enable_testing()

add_executable(sqlite_driver_tests src/SqliteDriverTest.cpp)

target_link_libraries(sqlite_driver_tests gmock_main sqlite_driver sqlite3_backend)

add_test(sqlite_driver_tests sqlite_driver_tests)
