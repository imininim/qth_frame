include_directories(${PROJECT_SOURCE_DIR}/include)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
link_directories(${PROJECT_SOURCE_DIR}/lib)

#搜集需要进行编译的源文件
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/test/example_tcp CPP_SRC)
#指定这是一个exe程序
add_executable(example_tcp ${CPP_SRC})
#动态链接 qthserver 库
target_link_libraries(example_tcp qthserver)


##########################################################################################
#版本控制
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(BUILD_MODE "Debug")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(BUILD_MODE "Release")
endif()

if(UNIX)
	#设置输出目录
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${BUILD_MODE})
elseif(WIN32)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
endif()



if(UNIX)
#平台需要的库
target_link_libraries(example_tcp rt)
target_link_libraries(example_tcp pthread)
endif()



