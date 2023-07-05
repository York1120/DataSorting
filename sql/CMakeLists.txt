# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (MySQLCreate)

set(CMAKE_CXX_STANDARD 11)


# 添加可执行文件或库
add_executable(MySQLCreate SqlCreate.cpp SqlCreateTest.cpp)

# 查找 libmysqlclient 库
find_library(MYSQLCLIENT_LIB mysqlclient)

if (MYSQLCLIENT_LIB)
    # 将 libmysqlclient 库链接到可执行文件或库
    target_link_libraries(MySQLCreate ${MYSQLCLIENT_LIB})
else()
    message(FATAL_ERROR "libmysqlclient not found")
endif()

