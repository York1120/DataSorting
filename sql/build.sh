#!/bin/bash

# 创建并进入 build 目录
mkdir build
cd build

# 使用 cmake 生成 Makefile
cmake ..

# 使用 make 构建项目
make

# 返回上级目录
cd ..

# 执行可执行文件
./build/MySQLCreate
