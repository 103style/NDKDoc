cmake_minimum_required(VERSION 3.4.1)

add_library( hello-ndk
             SHARED
             src/main/cpp/hello-ndk.cpp)

include_directories(src/main/cpp/include/)

find_library( log-lib # 库路径的变量名
              log ) # 对应的库名

#将预构建库关联到您自己的原生库
target_link_libraries( hello-ndk
                       ${log-lib} )