/******************************
author:		qutianhao
function:	模块化数据类型定义， 数据宏定义
date:		2016/8/15
tips:

宏：
	QTH_MAX_PROTOCOL_NUM	模块号最大值
	QTH_MAX_MESSAGE_NUM		消息号最大值
	MAX_NAME_NUM		协议中，用户使用的名称字符数量
类型:
	M_TYPE					Module Type  模块协议号类型
	MSG_TYPE				Message Type 消息协议号类型
*******************************/
#ifndef _MACRO_DEFINE_H_
#define _MACRO_DEFINE_H_

#define QTH_MAX_PROTOCOL_NUM		UCHAR_MAX	
#define QTH_MAX_MESSAGE_NUM			UCHAR_MAX
#define QTH_MAX_CLASS_NAME_NUM		64
#define MAX_NAME_NUM			64
//模块类型定义
typedef		unsigned char		BYTE;
typedef		unsigned char		M_TYPE;		
typedef		unsigned char		MSG_TYPE;	



#endif

