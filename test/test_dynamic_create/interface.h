/**********************
动态创建的基类
***********************/
#ifndef INTERFACE_H_
#define INTERFACE_H_
#include "utility/utility.h"
#include "frame/dynamic_create.h"

//该接口下所有实现的模块
enum EModule
{
	MODULE_A,			//模块A
	MODULE_B,			//模块B
	MODULE_MAX,
};

//该接口下事件
enum EEvent{
	EVENT_START = 199,	//事件起始符

	EVENT_HELLO_WORLD,


	EVENT_END,		//事件结束符
};

//短式声明
class baseClass;


//模块管理类，在构造的时候自动创建出 EModule 下的所有模块
/*
	第一个参数是接口类
	第二个参数是自动创建模块的数量 
*/
typedef QTH_NAME_SPACE::CDataManager<baseClass, MODULE_MAX>	ClassManager;


//定义任意一个基类接口
class baseClass
{
	DECLARE_BASE_EX(baseClass)
public:
	baseClass(){}
	virtual ~baseClass(){}    //注意：没有虚析构，delete 会不完整
		
	//必须实现的接口
	void SetManager(ClassManager* p){}
	ClassManager* GetManager()		{return NULL;}


	//自定义的虚函数
	virtual void print()				 = 0;
};




#endif
