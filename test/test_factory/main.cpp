/*
	测试 泛型工厂
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 工厂头文件
#include"utility/objFactory.h"

class Base
{
public:
	virtual void print()
	{
		cout << "base " << endl;
	}
};

class Deriver : public Base
{
public:
	static Base* Create() { return new Deriver;}

	virtual void print()
	{
		cout << "Deriver" << endl;
	}

};

class Deriver2: public Base
{
public:
	Deriver2(){} 	
	static Base* Create() { return new Deriver2; }
	virtual void print()
	{
		cout << "Derivier2" << endl;
	}
};


int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;
	Factory<Base> factory;

	factory.Register(1, &Deriver::Create);
	factory.Register(2, &Deriver2::Create);

	Base* p  = factory.CreateObject(1);
	Base* p2 = factory.CreateObject(2);

	p->print();
	p2->print();
	getchar();
	return 0;
}
