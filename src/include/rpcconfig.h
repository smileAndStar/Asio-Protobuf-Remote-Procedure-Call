#pragma once

#include <unordered_map>
#include <string>
#include <yaml-cpp/yaml.h>
#include <iostream>

// rpc配置文件
class RpcConfig {
    public:
        /**
         * @brief LoadConfigFile 解析加载配置文件
         * @param config_file 配置文件
         * @return void
         */
        void LoadConfigFile(const std::string &config_file) {
            try {
                // 加载yaml配置文件
                config_ = YAML::LoadFile(config_file);
            } catch (const YAML::Exception& e) {
                std::cerr << "Failed to load YAML file: " << e.what() << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        /**
         * @brief Load 查询配置项信息
         * @param key 配置项key
         * @return 配置项value
         */
        template<typename T>
        T Load(const std::string& key) {
            YAML::Node node = YAML::Clone(config_);  // 深拷贝，避免修改 config_，如果直接node = config_ 是浅拷贝，导致YAML内部使用共享指针，修改node会影响config_
            
            size_t start = 0;
            size_t pos = key.find('.');  // 查找第一个 "."的位置
            
            while (pos != std::string::npos) {  // find 如果没找到会返回 npos
                std::string part = key.substr(start, pos - start);  // 提取子字符串
                
                if(!node[part]) {
                    throw std::runtime_error("Config key not found: " + part);
                }
                
                node = node[part];  // 进入下一层节点
                                    // Tip. yaml 读取嵌套结构： node["parent"]["child"]["grandchild"]
                
                start = pos + 1;
                pos = key.find('.', start);
            }
            
            std::string last_part = key.substr(start);  // 提取最后一个子字符串
            if(!node[last_part]) {
                throw std::runtime_error("Config key not found: " + last_part);
            }

            try {
                return node[last_part].as<T>();
            } catch (const YAML::Exception &e) {
                throw std::runtime_error("Failed to convert config value for key: " + key + " - " + e.what());
            } 
        }
  
    private:
        YAML::Node config_;
};