# CMake工具的最低版本
cmake_minimum_required(VERSION 3.16)

# 将ESP-IDF的源码包含进来
# $ENV{IDF_PATH} 环境变量，ESP-IDF的安装目录
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# 工程名，编译后，生成的二进制文件是 Template.bin
project(Template)