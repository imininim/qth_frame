/*********************
function:	访问者Visitor 模式的泛化 
date:		2017/01/23
author:		屈天豪
tips:		
	实现GOF visitor模式的泛化版本, 解决GOF版本无法很好的改变类层次结构的问题
**************************/
#ifndef VISITOR_H_
#define VISITOR_H_
#include "utility/compile.h"

BEGIN_NAMESPACE

	/*访问者基类， 所有访问者需要继承于此类*/
	class BaseVisitor
	{
	public:
		virtual ~BaseVisitor(){}
	};

	/*访问者基类， 访问者需要继承此类以添加需要访问的类对象(public Visitor<你需要访问的类对象>) */
	template<class T, class R = void>
	class Visitor
	{
	public:
		typedef		R		ReturnType;
		virtual ReturnType Visit(T&)		= 0;
	};

	/*受访问者基类*/
	template<class R = void>
	class BaseVisitable
	{
	public:
		typedef		R	ReturnType;
		virtual ~BaseVisitable(){}
		virtual ReturnType Accept(BaseVisitor& v)		= 0;

	protected:
		template<class ConVisitor>
		static ReturnType AcceptImp(ConVisitor& visited, BaseVisitor& guest)
		{
			if (Visitor<ConVisitor>* p = dynamic_cast<Visitor<ConVisitor>*>(&guest) )
				return p->Visit(visited);

			return ReturnType();
		}

	};

	//受访者帮助宏
#define		DEFINE_VISITABLE() public:\
			virtual ReturnType Accept(QTH_NAME_SPACE::BaseVisitor& guest) \
			{ return AcceptImp(*this, guest); }


END_NAMESPACE
#endif
