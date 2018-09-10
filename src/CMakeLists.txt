include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/3rd/boost_1_60_0)

#添加boost 链接库目录
link_directories(${PROJECT_SOURCE_DIR}/3rd/boost_1_60_0/stage/lib)
#添加log4cpp 链接库目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

#查找src下所有cpp文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src CPP_SRC)
#构建一个动态库
add_library(qthserver SHARED ${CPP_SRC} ${CPP_SRC_TEST})

##########################################################################################
#版本控制
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(BUILD_MODE "Debug")
	message(STATUS "**************set Debug mode**************")

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(BUILD_MODE "Release")
	message(STATUS "***************set Release mode**********")

else()
	message(FATAL_ERROR "**********cmake -DCMAKE_BUILD_TYPE=Debug or
	Release*************")

endif()

#######################################################################################
#平台选项
if(UNIX)
	message(STATUS "************start linux compile**********")
	#设置输出目录
	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${BUILD_MODE})
	#编译选项
	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -D_DEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -g -ggdb -DNDEBUG")
	#安装目录
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)	
	install(PROGRAMS ${PROJECT_SOURCE_DIR}/bin/${BUILD_MODE}/libqthserver.so DESTINATION lib)
elseif(WIN32)
	message(STATUS "**********start windows compilie**************")
	#设置输出目录
	set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
	#编译选项
	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -D_DEBUG /MDd /Od /Zi /W3")
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -DNDEBUG /MD /O2 /W3")

elseif()
	message(FATAL_ERROR "**************only windows or linux*************")
endif()
####################################################
#链接 log4cpp
target_link_libraries(qthserver log4cpp)
#项目生成顺序
ADD_DEPENDENCIES(qthserver log4cpp)
#链接 boost_thread

#unix 没有自动链接库， 所以需要手动添加boost 链接
if(UNIX)
target_link_libraries(qthserver ${PROJECT_SOURCE_DIR}/3rd/boost_1_60_0/stage/lib/libboost_system.a)
target_link_libraries(qthserver ${PROJECT_SOURCE_DIR}/3rd/boost_1_60_0/stage/lib/libboost_chrono.a)
target_link_libraries(qthserver ${PROJECT_SOURCE_DIR}/3rd/boost_1_60_0/stage/lib/libboost_thread.a)
endif()

#######################################################

#平台需要的库
if(UNIX)
	target_link_libraries(qthserver rt)
	target_link_libraries(qthserver pthread)
endif()
