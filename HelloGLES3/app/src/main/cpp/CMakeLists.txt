
cmake_minimum_required(VERSION 3.4.1)

add_library(native-gles3
        SHARED
        native-gles3.cpp)

target_link_libraries(native-gles3
        android
        EGL
        GLESv3
        log)