cmake_minimum_required (VERSION 2.8)
project (MySQLCreate)

set(CMAKE_CXX_STANDARD 17)

# 添加可执行文件或库
add_executable(MySQLCreate SqlCreate.cpp SqlCreateTest.cpp ../log/log.cpp)

# 查找 libmysqlclient 库
find_library(MYSQLCLIENT_LIB mysqlclient)

if (MYSQLCLIENT_LIB)
    # 将 libmysqlclient 库链接到可执行文件或库
    target_link_libraries(MySQLCreate ${MYSQLCLIENT_LIB})
else()
    message(FATAL_ERROR "libmysqlclient not found")
endif()

# 设置链接库的路径
link_directories(/usr/lib/gcc/x86_64-linux-gnu/7)

# 链接libstdc++fs库
target_link_libraries(MySQLCreate stdc++fs)

# 链接librt和pthread库
find_library(LIBRT_LIB rt)
find_library(PTHREAD_LIB pthread)

if (LIBRT_LIB AND PTHREAD_LIB)
    target_link_libraries(MySQLCreate ${LIBRT_LIB} ${PTHREAD_LIB})
else()
    message(FATAL_ERROR "librt or pthread not found")
endif()
