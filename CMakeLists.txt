cmake_minimum_required(VERSION 3.10)

project(aprpc)

# === 编译标准 ===
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === 开启编译器警告（建议开发时打开）===
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")

# === 头文件路径 ===
include_directories(
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_SOURCE_DIR}/example
)

# === 设置可执行文件输出路径 ===
set(EXECUTABLE_OUTPUT_PATH 
    ${PROJECT_SOURCE_DIR}/bin
)

# === 设置库文件输出路径 ===
set(LIBRARY_OUTPUT_PATH    
    ${PROJECT_SOURCE_DIR}/lib
)

# === 子目录 ===
# rpc框架源码
add_subdirectory(src)
# 示例代码
add_subdirectory(example)
# 配置文件
add_subdirectory(config)