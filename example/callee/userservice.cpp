/*
 * 基于rpc框架，将本地的UserService服务发布为rpc远程服务
 * 1. 定义一个UserService类，继承UserServiceRPC
 * 2. 重写Login方法，调用本地的Login方法
 * 3. 在main函数中，完成框架的初始化，服务的发布，启动rpc服务
 */

#include <iostream>
#include <string>
#include "user.pb.h"
#include "rpcapplication.h"
#include "rpcprovider.h"

/*
 * UserService 原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
 */
class UserService : public fixbug::UserServiceRPC {    // 使用在rpc服务发布端
public:
    bool Login(std::string name, std::string pwd) {
        std::cout << "UserService Local Login called!" << std::endl;
        std::cout << "name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }
    
    bool Register(uint32_t id, std::string name, std::string pwd) {
        std::cout << "id: " << id << " name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }

    /* 实现基类的虚函数
     * 下面这些方法都是rpc框架调用的，用户不需要自己调用
     * 1. caller   ===>   Login(LoginRequest)  => muduo =>   callee 
     * 2. callee   ===>    Login(LoginRequest)  => 交到下面重写的这个Login方法上了
     */

    // 登录
    /**
     * @brief 重写基类UserServiceRPC的虚函数 Login
     * @param controller rpc控制器：用于保存rpc调用的状态信息
     * @param request rpc请求消息对象，包含了请求参数
     * @param response rpc响应消息对象，包含了响应数据
     * @param done rpc方法调用完成后的回调操作
     */
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done) {
        // 1.从 LoginRequest 获取参数的值
        // 框架给业务上报了请求参数 LoginRequest ，应用获取相应数据做本地业务
        std::string name = request->username();
        std::string pwd = request->password();
        
        // 2.执行本地服务 login ,并获取返回值
        bool login_result = Login(name, pwd); // 调用本地方法

        // 3. 根据返回值填写 LoginResponse
        // 把响应写入response
        fixbug::ResultCode* code = response->mutable_result();  // mutable_result() 返回 message 内部子 message(这里是 result) 的可修改指针
        // 正常情况
        code->set_errcode(0);   // 0表示没有错误
        code->set_errmsg("");   // 空字符串表示没有错误信息
        response->set_success(login_result);

        // 4. 执行回调，将 LoginResponse 发给rpc client
        // 执行回调操作：执行响应对象数据的序列化和网络发送(由框架完成)
        done->Run();
    }

    // 注册
    /**
     * @brief 重写基类UserServiceRPC的虚函数 Register
     * @param controller rpc控制器：用于保存rpc调用的状态信息
     * @param request rpc请求消息对象，包含了请求参数 
     * @param response rpc响应消息对象，包含了响应数据
     * @param done rpc方法调用完成后的回调操作
     */
    void Register(::google::protobuf::RpcController* controller,
                         const ::fixbug::RegisterRequest* request,
                         ::fixbug::RegisterResponse* response,
                         ::google::protobuf::Closure* done) {
        uint32_t id = request->id();
        std::string name = request->username();
        std::string pwd = request->password();

        bool register_result = Register(id, name, pwd); // 调用本地方法

        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(register_result);

        done->Run();
    }
};


int main(int argc, char* argv[]) {

    // 框架初始化
    RpcApplication::Init(argc, argv);

    // 把UserService对象发布到rpc框架中
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动rpc服务，进入阻塞开始等待远程的rpc调用请求
    provider.Run();
}
