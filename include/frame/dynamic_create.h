/********************************
author:		qutianhao
function:	��̬��������
date:		2018/07/25
tips:
	�ṩ�������ƴ�����Ļ���
	���������»���, �ַ��������������磬�û����룬���߱����ļ�
		A* p = Create("A"); 
*********************************/
#ifndef DYNAMIC_CREATE_H_
#define DYNAMIC_CREATE_H_
BEGIN_NAMESPACE

	//��̬��������Ϣ���ڽ�list
	template<class T>
	class RunTimeClassEx
	{
	public:
		char* m_pClassName;				//��������
		RunTimeClassEx* m_pNext;		//ָ����һ���ڵ�
		static RunTimeClassEx* s_pFirst;//ָ���׽ڵ�
		T* (*m_pCreateFn)();			//ָ�򴴽���������͵�ָ��
		T* CreateObject();	
	};
	//��̬��Ա��ʼ��
	template<class T> RunTimeClassEx<T>* RunTimeClassEx<T>::s_pFirst = NULL;
	
	template<class T>
	T* RunTimeClassEx<T>::CreateObject()
	{
		if (!m_pCreateFn) return NULL;
		return (*m_pCreateFn)();
	}
		
	//������Ϣ�����ӹ���
	template<class RTC>
	struct initRunTimeClassEx
	{
		initRunTimeClassEx(RTC* pNewClass)
		{
			if (RTC::s_pFirst != NULL)
				pNewClass->m_pNext = RTC::s_pFirst;
				
			RTC::s_pFirst = pNewClass;
		}
	};
END_NAMESPACE

/**************
��̬��������
	params:
		className		��ǰ�������
		baseClassName	�̳еĳ���������
****************/
#define DECLARE_DYNAMIC_CREATE(className, baseClassName) \
	public:\
	static QTH_NAME_SPACE::RunTimeClassEx<baseClassName> class##className;\
	static baseClassName* CreateObject();

#define IMPLEMENT_DYNAMIC_CREATE(className, baseClassName)\
	static char str##className[] = #className;\
	QTH_NAME_SPACE::RunTimeClassEx<baseClassName> className::class##className = {str##className, NULL, &className::CreateObject};\
	static QTH_NAME_SPACE::initRunTimeClassEx<QTH_NAME_SPACE::RunTimeClassEx<baseClassName> > init_##className(&className::class##className);\
	baseClassName* className::CreateObject()\
	{return new className;}

#endif
