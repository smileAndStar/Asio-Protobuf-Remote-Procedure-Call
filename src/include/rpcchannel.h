#pragma once

#include <google/protobuf/service.h>

class RpcChannel : public google::protobuf::RpcChannel {
    public:
        // 重写基类的虚函数
        /**
         * @brief 通过RPC方式调用远程方法
         *        在客户端 Stub 对象调用 rpc 方法时(见 example的calluserservice.cpp)，框架会触发该函数的调用
         *        该函数需要通过网络将 rpc 方法调用请求发送给远程的 rpc 服务端，然后等待 rpc 服务端返回响应结果 
         * @param method 要调用的远程方法的描述信息(由protobuf框架生成)
         * @param controller 控制调用过程的控制器
         * @param request 包含调用参数的请求消息
         * @param response 用于存储从远程方法接收到的响应消息
         * @param done 调用完成后的回调函数
         */
        void CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller,
                        const google::protobuf::Message* request,
                        google::protobuf::Message* response,
                        google::protobuf::Closure* done) override;
};