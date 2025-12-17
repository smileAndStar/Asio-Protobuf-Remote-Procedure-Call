set(SRCS
    calluserservice.cpp
    ../proto_gen/user.pb.cc
)

add_executable(consumer
    ${SRCS}
)

target_include_directories(consumer
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../proto_gen
)

target_link_directories(consumer
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(consumer
    # rpc框架
    rpc
    # protobuf库
    protobuf
    # 线程库（protobuf 内部使用）
    pthread
)
