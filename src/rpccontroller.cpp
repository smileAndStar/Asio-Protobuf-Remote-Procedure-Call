#include "rpccontroller.h"

RpcController::RpcController() : 
    failed_(false), 
    errText_("") {}

void RpcController::Reset() {
    failed_ = false;
    errText_.clear();
}

bool RpcController::Failed() const {
    return failed_;
}

std::string RpcController::ErrorText() const {
    return errText_;
}

void RpcController::StartCancel() {
    // 尚未实现
}

void RpcController::SetFailed(const std::string &reason) {
    failed_ = true;
    errText_ = reason;
}

bool RpcController::IsCanceled() const {
    return false;
}

void RpcController::NotifyOnCancel(google::protobuf::Closure *callback) {
    // 尚未实现
}