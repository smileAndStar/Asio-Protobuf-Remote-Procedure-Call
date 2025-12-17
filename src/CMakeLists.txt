file(GLOB SOURCE_FILES
    "./*.cpp"
)

find_package(yaml-cpp REQUIRED)

# 创建静态库（因为 muduo 是静态库且未使用 -fPIC 编译）
add_library(rpc STATIC
    ${SOURCE_FILES}
    ./rpcheader/rpcheader.pb.cc
)

target_include_directories(rpc PRIVATE
    ./rpcheader
)

target_link_libraries(rpc PRIVATE
    protobuf    # protobuf 库
    yaml-cpp    # yaml-cpp 库
    muduo_net   # muduo 网络库
    muduo_base  # muduo 基础库
    pthread     # POSIX 线程库
)

