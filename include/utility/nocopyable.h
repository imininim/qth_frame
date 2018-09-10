/********************************
function:	对象可拷贝性组件
date:		2016/09/13
tips:		
			使用方式： 私有继承， 实现类对象的不可拷贝特性
*********************************/
#ifndef NOCOPYABLE_H_
#define NOCOPYABLE_H_
BEGIN_NAMESPACE

class QTH_UTILITY_EXPORT Nocopyable
{
protected:
	Nocopyable();
	~Nocopyable();

private:
	Nocopyable(const Nocopyable&);
	void operator=(const Nocopyable&);
};


END_NAMESPACE
#endif
