/************************
 * function: 	泛化对象工厂，拷贝工厂
 * date;		2016/12/11
 * tips: 			
*************************/
#ifndef 	OBJFACTORY_H_
#define 	OBJFACTORY_H_
#include"utility/functor.h"

BEGIN_NAMESPACE


//异常错误处理
template<class IdentifierType, class ProductType>
 class ExceptionFactoryError
{
public:
	class Exception: public std::exception
	{
		public:
			Exception(const IdentifierType& unknownid): m_unknownID(unknownid){}		
			virtual const char* what(){ return "该ＩＤ没有被注册，因此无法创建出指定对象: "; }
			IdentifierType GetID() 	 {  return m_unknownID;}
		private:
			IdentifierType  m_unknownID;
	};
	
	//返回值无用，仅是为了满足约束
	static ProductType* OnUnknownType(const IdentifierType& err_info)
	{
		throw Exception(err_info);
		return NULL;
	}
};


//空指针返回错误处理
template<class IdentifierType, class ProductType>
class NullPtrFactoryError
{
	public:
	static ProductType* OnUnknownType(const IdentifierType&)
	{
		return NULL;
	}
};


//对象工厂
template<class AbstractProduct,													//抽象基类
		 class IdentifierType = int,											//标识符
		 template<class,class> class FactoryErrorPolicy = NullPtrFactoryError>	//错误策略
		 class Factory: public FactoryErrorPolicy<IdentifierType, AbstractProduct>
{
	public:
		typedef Functor<AbstractProduct*> 					ProductCreator; 				
		typedef std::map<IdentifierType, ProductCreator> 	IdToProductMap;
		typedef typename IdToProductMap::iterator 			FIter;

		Factory(){}
		
		//注册
		void Register(const IdentifierType& id, ProductCreator creator)
		{
			m_data.insert(std::make_pair(id, creator));
		}

		//注销
		void Unregister(const IdentifierType& id)
		{
			m_data.erase(id);
		}

		//获取所有已经注册的ID
		std::vector<IdentifierType> RegisterIDs()
		{
			std::vector<IdentifierType> tempVec;

			for (FIter iter = m_data.begin(); iter != m_data.end(); ++iter)
			{
				tempVec.push_back(iter->first);
			}

			return tempVec;
		}
		
		//对象创建
		AbstractProduct* CreateObject(const IdentifierType& id)
		{
			FIter it = m_data.find(id);
			if (it != m_data.end())
			{
				return (it->second)();
			}

			return FactoryErrorPolicy<IdentifierType, AbstractProduct>::OnUnknownType(id);
		}
		
		private:
		IdToProductMap m_data;

};

END_NAMESPACE

#endif
