cmake_minimum_required(VERSION 3.4.1)

add_library(
        file_operation
        SHARED
        file_operation.cpp)

find_library(
        log-lib
        log)

target_link_libraries(
        file_operation
        ${log-lib})