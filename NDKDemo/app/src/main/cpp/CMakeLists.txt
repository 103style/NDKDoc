cmake_minimum_required(VERSION 3.4.1)

add_library(
        bitmap
        SHARED
        bitmap.cpp)

add_library(
        list_dir_all_file
        SHARED
        list_dir_all_file.cpp)

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

include_directories(src/main/cpp/)

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

target_link_libraries(
        list_dir_all_file
        ${log-lib})

target_link_libraries(
        bitmap
        jnigraphics
        ${log-lib})