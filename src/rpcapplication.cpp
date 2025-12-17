#include <iostream>
#include <unistd.h>
#include "rpcapplication.h"

RpcConfig RpcApplication::m_config;

void ShowArgsHelp() {
    std::cout << "Usage: command [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -i,  configfile name" << std::endl;
}

void RpcApplication::Init(int argc, char* argv[]) {
    if (argc < 2) {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int opt = 0;
    std::string config_file;
    while((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
            case 'i':
                config_file = optarg;
                break;
            
            case '?':
                // getopt 遇到未知选项或缺少参数时返回 '?'
                ShowArgsHelp();
                exit(EXIT_FAILURE);

            default:
                break;
        }
    }

    // 检查是否提供了配置文件
    if (config_file.empty()) {
        std::cerr << "Error: config file not provided\n" << std::endl;
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    // 加载配置文件
    m_config.LoadConfigFile(config_file);
    // 检查配置文件是否加载成功
     
    std::cout << "RPC Server will start at " << m_config.Load<std::string>("rpc.server_ip")
                                      << ":" << m_config.Load<int>("rpc.server_port") << "\n"
                 << "ZooKeeper address is " << m_config.Load<std::string>("zookeeper.server_ip") 
                                      << ":" << m_config.Load<int>("zookeeper.server_port") << std::endl;
}
    
RpcApplication& RpcApplication::GetInstance() {
    static RpcApplication instance; // 线程安全，只会创建一次
    return instance;
}

RpcConfig& RpcApplication::GetConfig() {
    return m_config;
}