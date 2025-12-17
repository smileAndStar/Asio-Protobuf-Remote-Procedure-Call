# 配置文件生成

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.yaml
    ${PROJECT_SOURCE_DIR}/bin/config.yaml
    COPYONLY
)
