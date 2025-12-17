#include <iostream>
#include "rpcapplication.h"
#include "user.pb.h"
#include "rpcchannel.h"
#include "rpccontroller.h"


int main(int argc, char **argv) {
    // 框架初始化
    RpcApplication::Init(argc, argv);

    // 通过rpc调用远程的用户服务
    fixbug::UserServiceRPC_Stub stub(new RpcChannel());
    // rpc 方法的请求
    fixbug::LoginRequest request;
    request.set_username("zhang san");
    request.set_password("123456");
    // rpc 方法的响应
    fixbug::LoginResponse response;
    // 发起 rpc 方法的调用
    stub.Login(nullptr, &request, &response, nullptr);

    // 读取 rpc 调用的结果
    if (response.result().errcode() == 0) {
        std::cout << "rpc login response: " << response.success() << std::endl;
    } else {
        std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
    }

    // 注册
    fixbug::RegisterRequest register_request;
    register_request.set_id(666);
    register_request.set_username("smile");
    register_request.set_password("114514");

    fixbug::RegisterResponse register_response; // rpc 方法的响应
    RpcController controller;   // rpc 控制器
    
    stub.Register(&controller, &register_request, &register_response, nullptr);

    if (controller.Failed()) {
        std::cout << "rpc register error: " << controller.ErrorText() << std::endl;
    } else {    // 正常返回
        if (register_response.result().errcode() == 0) {
            std::cout << "rpc register response: " << register_response.success() << std::endl;
        } else {
            std::cout << "rpc register response error: " << register_response.result().errmsg() << std::endl;
        }
    }
    
    return 0;
}