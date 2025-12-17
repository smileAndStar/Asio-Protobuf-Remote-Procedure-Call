#pragma once

#include "rpcconfig.h"

/**
 * @brief RpcApplication rpc框架的基础类
 *        提供模块的初始化加载配置文件等功能
 */
class RpcApplication {
    public:
        // 静态方法，在程序开始时调用一次)
        /**
         * @brief Init 初始化操作:加载配置文件
         * @param argc 命令行参数个数
         * @param argv 命令行参数数组
         * @return void
         */
        static void Init(int argc, char *argv[]);
      
        /**
         * @brief GetInstance 获取单例对象(一个类只有一个实例、在程序中唯一)
         * @return RpcApplication& 单例对象引用
         */
        static RpcApplication& GetInstance();

        /**
         * @brief GetConfig 获取配置对象
         * @return RpcConfig& 配置对象引用
         */
        static RpcConfig& GetConfig();
        
    private:
        RpcApplication() = default;
        ~RpcApplication() = default;

        RpcApplication(const RpcApplication&) = delete; // 禁止拷贝构造函数
        RpcApplication& operator=(const RpcApplication&) = delete; // 禁止拷贝赋值函数
        RpcApplication(RpcApplication&&) = delete;  // 禁止移动构造函数
        RpcApplication& operator=(RpcApplication&&) = delete; // 禁止移动赋值函数

    private:
        // rpc 配置对象
        static RpcConfig m_config;
};
