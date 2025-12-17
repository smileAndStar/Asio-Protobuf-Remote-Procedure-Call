#include "rpcchannel.h"
#include <google/protobuf/descriptor.h>
#include "rpcheader.pb.h"
#include "rpcapplication.h"
#include "rpccontroller.h"
#include <boost/asio.hpp>

void RpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                           google::protobuf::RpcController* controller,
                           const google::protobuf::Message* request,
                           google::protobuf::Message* response,
                           google::protobuf::Closure* done) {

    const google::protobuf::ServiceDescriptor* sd = method->service();  // 获取服务描述符
    std::string service_name = sd->name();                      // 获取服务名称
    std::string method_name = method->name();                    // 获取方法名称

    // ==================== 组织rpc请求的字符流 ====================
    /**
     * 将 rpc 方法调用请求发送给远程的 rpc 服务端，然后等待 rpc 服务端返回响应结果 
     * 发送的字符流包含的信息：
     * 1.数据头的长度 header_size (4字节)
     * 2.数据头 header_str: service_name + method_name + args_size (header_size字节)
     * 3.请求参数 args_str  (args_size字节)
     */
                    
    std::string args_str;   // 存储序列化后的请求参数
    // 序列化请求参数
    if (!request->SerializeToString(&args_str)) {
        // 序列化请求参数失败
        controller->SetFailed("request SerializeToString failed!");
        return;
    }

    // 构建RPC数据头
    rpcheader::RpcHeader rpcheader;
    rpcheader.set_service_name(service_name);   // service_name
    rpcheader.set_method_name(method_name);     // method_name
    rpcheader.set_args_size(args_str.size());   // args_size

    std::string header_str; // 存储序列化后的数据头
    if (!rpcheader.SerializeToString(&header_str)) {
        controller->SetFailed("rpcheader SerializeToString failed!");
        return;
    }

    // 数据头的长度
    uint32_t header_size = htonl(header_str.size());  // 主机字节序转网络字节序

    // 组装发送数据
    std::string send_buf;
    send_buf.append((char*)&header_size, 4);    // 1. 四字节的 header_size
    send_buf.append(header_str);                // 2. 数据头 header_str ：service_name + method_name + args_size
    send_buf.append(args_str);                   // 3. 请求参数 args_str
    // ============================================================

    // ==================== 通过网络发送rpc请求 ====================
    // 读取配置
    std::string ip = RpcApplication::GetConfig().Load<std::string>("rpc.server_ip");
    uint16_t port = RpcApplication::GetConfig().Load<int>("rpc.server_port");

    // 使用Boost.Asio发起网络请求
    try {
        // 使用ASIO进行网络通信
        boost::asio::io_context io_context; // 创建IO上下文
        boost::asio::ip::tcp::socket socket(io_context);    // 创建TCP套接字
        boost::asio::ip::tcp::resolver resolver(io_context);    // 创建解析器

        // 连接服务器
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip, std::to_string(port));    // 解析服务器地址和端口
        boost::asio::connect(socket, endpoints);    // 连接服务器

        // 发送请求
        boost::asio::write(socket, boost::asio::buffer(send_buf));    // 发送请求数据(write 会阻塞直到数据发送完毕)

        // 接收响应 - 使用长度前缀方式避免消息边界问题
        // 1. 先读取4字节的响应长度
        uint32_t response_len = 0;
        boost::asio::read(socket, boost::asio::buffer(&response_len, sizeof(response_len)));
        response_len = ntohl(response_len);  // 网络字节序转主机字节序

        // 2. 根据长度读取完整的响应消息体
        std::vector<char> response_body(response_len);
        boost::asio::read(socket, boost::asio::buffer(response_body));

        // 解析响应
        std::string response_str(response_body.data(), response_len);
        if (!response->ParseFromString(response_str)) {
            controller->SetFailed("ParseFromString response failed!");
            return;
        }

    } catch (std::exception& e) {
        controller->SetFailed("RPC call exception: " + std::string(e.what()));
    }
}