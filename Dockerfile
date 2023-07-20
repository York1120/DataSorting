# 使用基础镜像
FROM ubuntu:20.04

# 设置工作目录
WORKDIR /app

# 复制 CMakeLists.txt 和 build.sh 到容器中的 /app 目录
COPY CMakeLists.txt build.sh /app/

# 复制源代码到容器中的 /app 目录
COPY . /app

# 安装必要的软件包（例如，cmake、gcc、g++等）
RUN apt-get update && \
    apt-get install -y cmake g++ libmysqlclient-dev

# 在容器中的 /app 目录中执行以下命令
RUN cd /app/build && \
    cmake .. && \
    make

# 指定容器启动时需要执行的命令
CMD ["./build/DataSorting"]
