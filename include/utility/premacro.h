/********************************
author:		qutianhao
function:	编译期间 宏的预处理
date:		2018/01/22
tips:
	提供宏的处理工具
*********************************/
#ifndef _PRE_MACRO_H_
#define _PRE_MACRO_H_

#define ARGS_IMP(_1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10, \
	_11,_12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21,_22, _23, _24, _25, _26, _27, _28, _29, _30, \
	_31,_32, _33, _34, _35, _36, _37, _38, _39, _40, \
	_41,_42, _43, _44, _45, _46, _47, _48, _49, _50, \
	_51,_52, _53, _54, _55, _56, _57, _58, _59, _60, \
	_61, _62, _63, _64,\
	N, ...) N


//获取宏总共有得参数个数
#define HELPER_GET_ARGS_COUNT(args)  ARGS_IMP args 
#define GET_ARGS_COUNT(...)  HELPER_GET_ARGS_COUNT((__VA_ARGS__,64,63,62,61,60,\
	59,58,57,56,55,54,53,52,51,50,\
	49,48,47,46,45,44,43,42,41,40,\
	39,38,37,36,35,34,33,32,31,30,\
	29,28,27,26,25,24,23,22,21,20,\
	19,18,17,16,15,14,13,12,11,10,\
	9, 8, 7, 6, 5, 4, 3, 2, 1, 0,))

	//宏拼接
#define INTER_CONNECT3(M,count)		M##count
#define INTER_CONNECT2(M,count)		INTER_CONNECT3(M,count)
#define INTER_CONNECT1(M, count)	INTER_CONNECT2(M, count)
#define MACCONNECT(M,count)			INTER_CONNECT1(M,count)

#define  PRIVATE_ARGS_GLUE(x,y) x y

/****************************
function:	宏重载工具
example:
	#define TEST(...) INVOKE_ARG_MACRO(TEST, ##__VA_ARGS__)

	#define TEST1(param1)					cout << param1 << endl;
	#define TEST2(param1, param2)			cout << param1 << param2 << endl;
	#define TEST3(param1, param2, param3)	cout << param1 << param2 << param3 << endl;
	
	int main()
	{
		TEST("a");		==>		a
		TEST("b", 1);	==>		b1
		TEST("c", 2);	==>		c2
	}

*****************************/
#define INVOKE_ARG_MACRO(M,...) PRIVATE_ARGS_GLUE(MACCONNECT(M,GET_ARGS_COUNT(__VA_ARGS__)), (__VA_ARGS__))

#endif

