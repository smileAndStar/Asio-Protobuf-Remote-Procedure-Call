set(SRCS
    userservice.cpp
    ../proto_gen/user.pb.cc
)

add_executable(provider
    ${SRCS}
)

target_include_directories(provider
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../proto_gen
)

target_link_directories(provider
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(provider
    # rpc框架
    rpc
    # protobuf库
    protobuf
    # 线程库（protobuf 内部使用）
    pthread
)
