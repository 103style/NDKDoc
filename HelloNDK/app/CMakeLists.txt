cmake_minimum_required(VERSION 3.4.1)

add_library(hello-ndk
        SHARED
        src/main/cpp/hello-ndk.cpp)

add_library(array_operation
        SHARED
        src/main/cpp/array_operation.cpp)

add_library(encryptor
        SHARED
        src/main/cpp/encryptor.cpp)

include_directories(src/main/cpp/include/)

find_library(log-lib # 库路径的变量名
        log) # 对应的库名

#将预构建库关联到您自己的原生库
target_link_libraries(hello-ndk
        ${log-lib})

target_link_libraries(
        array_operation
        ${log-lib})

target_link_libraries(
        encryptor
        ${log-lib})