cmake_minimum_required(VERSION 3.4.1)

add_library(
        file_operation
        SHARED
        file_operation.cpp)

add_library(encryptor
        SHARED
        encryptor.cpp)

add_library(array_operation
        SHARED
        array_operation.cpp)

find_library(
        log-lib
        log)


target_link_libraries(
        array_operation
        ${log-lib})

target_link_libraries(
        encryptor
        ${log-lib})

target_link_libraries(
        file_operation
        ${log-lib})