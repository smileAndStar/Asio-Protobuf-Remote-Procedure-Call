#pragma once

#include <google/protobuf/service.h>
#include <string>

class RpcController : public google::protobuf::RpcController {
    public:
        RpcController();

        /**
         * 清空 RpcController 的内部状态（错误信息、取消状态等），以便复用
         * 不能在 RPC 还未结束时调用，否则可能造成状态混乱或崩溃
         * @return void
         */
        void Reset();

        /**
         * 判断这次 RPC 调用是否失败
         * 必须在调用结束之后才能调用，否则结果未定义
         * @return 如果 RPC 调用失败则返回 true，否则返回 false
         */
        bool Failed() const;

        /**
         * 返回一个可读的错误信息
         * @return 错误信息字符串
         */
        std::string ErrorText() const;

        /**
         * 客户端通知 RPC 系统希望取消本次调用
         */
        void StartCancel();

        /**
         * 服务端主动将这次 RPC 设置为失败，并告诉客户端失败原因
         * @param reason 失败原因字符串
         */
        void SetFailed(const std::string& reason);

        /**
         * 让服务端检查客户端是否已取消 RPC
         * @return 如果客户端已取消则返回 true，否则返回 false
         */
        bool IsCanceled() const;

        /**
         * 服务端注册一个回调，当客户端取消 RPC 时自动执行一次
         * @param callback 回调函数
         */
        void NotifyOnCancel(google::protobuf::Closure* callback);

    private:
        bool failed_ = false;   // RPC 方法执行过程中的状态
        std::string errText_ = ""; // 错误信息
};