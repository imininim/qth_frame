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

/********************
function: 宏 int 转换bool
*********************/
#define I_TO_BOOL(i) I_TO_BOOL_##i
#define I_TO_BOOL_0		0
#define I_TO_BOOL_1		1
#define I_TO_BOOL_2		1
#define I_TO_BOOL_3		1
#define I_TO_BOOL_4		1
#define I_TO_BOOL_5		1
#define I_TO_BOOL_6		1
#define I_TO_BOOL_7		1
#define I_TO_BOOL_8		1
#define I_TO_BOOL_9		1
#define I_TO_BOOL_10	1

#define I_TO_BOOL_11	1
#define I_TO_BOOL_12	1
#define I_TO_BOOL_13	1
#define I_TO_BOOL_14	1
#define I_TO_BOOL_15	1
#define I_TO_BOOL_16	1
#define I_TO_BOOL_17	1
#define I_TO_BOOL_18	1
#define I_TO_BOOL_19	1
#define I_TO_BOOL_20	1

#define I_TO_BOOL_21	1
#define I_TO_BOOL_22	1
#define I_TO_BOOL_23	1
#define I_TO_BOOL_24	1
#define I_TO_BOOL_25	1
#define I_TO_BOOL_26	1
#define I_TO_BOOL_27	1
#define I_TO_BOOL_28	1
#define I_TO_BOOL_29	1
#define I_TO_BOOL_30	1

#define I_TO_BOOL_31	1
#define I_TO_BOOL_32	1
#define I_TO_BOOL_33	1
#define I_TO_BOOL_34	1
#define I_TO_BOOL_35	1
#define I_TO_BOOL_36	1
#define I_TO_BOOL_37	1
#define I_TO_BOOL_38	1
#define I_TO_BOOL_39	1
#define I_TO_BOOL_40	1

#define I_TO_BOOL_41	1
#define I_TO_BOOL_42	1
#define I_TO_BOOL_43	1
#define I_TO_BOOL_44	1
#define I_TO_BOOL_45	1
#define I_TO_BOOL_46	1
#define I_TO_BOOL_47	1
#define I_TO_BOOL_48	1
#define I_TO_BOOL_49	1
#define I_TO_BOOL_50	1

#define I_TO_BOOL_51	1
#define I_TO_BOOL_52	1
#define I_TO_BOOL_53	1
#define I_TO_BOOL_54	1
#define I_TO_BOOL_55	1
#define I_TO_BOOL_56	1
#define I_TO_BOOL_57	1
#define I_TO_BOOL_58	1
#define I_TO_BOOL_59	1
#define I_TO_BOOL_60	1

#define I_TO_BOOL_61	1
#define I_TO_BOOL_62	1
#define I_TO_BOOL_63	1
#define I_TO_BOOL_64	1

/*************************
function: if_else 宏
tips:
	
*************************/
#define IF_MACRO(condition, true_con, false_con)	IF_ ## I_TO_BOOL(condition) (true_con, false_con)
#define IF_0(true_con, false_con)	false_con
#define IF_1(true_con, false_con)	true_con


#endif

