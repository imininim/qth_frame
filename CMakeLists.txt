#项目需要camke 版本
cmake_minimum_required(VERSION 2.8)

#项目工程名称
project(qthserver)


#包含子目录

#日志库
add_subdirectory(3rd/log4cpp)

#源码目录
add_subdirectory(src)

#TCP服务端测试程序(回显服务器)
add_subdirectory(test/tcp_server)

#TCP 压力测试客户端程序(回显客户端)
add_subdirectory(test/tcp_client_random_pressure)

#shared_from_this 测试程序
add_subdirectory(test/test_enable_shared_from_this)

#单例测试程序
add_subdirectory(test/test_single)

#装饰指针
add_subdirectory(test/test_decorate_ptr)

#对象池测试
add_subdirectory(test/test_obj_pool)

#测试日志
add_subdirectory(test/test_logger)

#测试xml解析器
add_subdirectory(test/test_xml)

#测试函数对象
add_subdirectory(test/test_function)

#测试tuple
add_subdirectory(test/test_tuple)

#测试memory
add_subdirectory(test/test_memory)

#测试 类型特性检测
add_subdirectory(test/test_typetraits)

#测试 bind
add_subdirectory(test/test_bind)

#测试 factory
add_subdirectory(test/test_factory)

#测试 visitor
add_subdirectory(test/test_visitor)

#测试 线程处理器
add_subdirectory(test/test_thread_processor)

#测试基础框架
add_subdirectory(test/test_frame)

#上传下载tcp服务器
add_subdirectory(test/example_tcp)
#上传下载tcp 测试客户端
add_subdirectory(test/example_tcp_client)

#动态创建
add_subdirectory(test/test_dynamic_create)

#http服务端
add_subdirectory(test/test_http_server)


