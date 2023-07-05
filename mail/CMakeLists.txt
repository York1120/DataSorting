# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (EMailTest)
#
if(WIN32)
    message(STATUS "windows compiling...")
    add_definitions(-D_PLATFORM_IS_WINDOWS_)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
else(WIN32)
    message(STATUS "linux compiling...")
    add_definitions( -D_PLATFORM_IS_LINUX_)
endif(WIN32)

set(CMAKE_CXX_STANDARD 11)

# 添加一个名为 "SendMailTest" 的可执行文件编译目标
add_executable(EMailTest SendMail.cpp SendMailTest.cpp)