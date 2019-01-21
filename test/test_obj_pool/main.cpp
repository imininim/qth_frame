/*
	测试对象池， 内存池
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 服务头文件
#include "utility/SmallObj.h"
//使用固定大小的对象池
struct A
{
	A(){ cout << "A::A" << endl;}
	~A(){ cout << "A::~A" << endl;}
	int i;
};

//需要使用小对象池的类
struct B: public QTH_NAME_SPACE::SmallObject<>
{
	B(){ cout << "B::B" << endl;}
	~B(){ cout << "B::~B" << endl;}

	int p;
	int d;
};

//所有继承类都可以自动享有基类的对象池
struct C: public B
{
	C(){ cout << "C::C" << endl;}
	~C(){ cout << "C::~C" << endl;}

	int c;
};


#include <vector>
int main(int argc, char* argv[])
{
	/////////////////////////////////////////////////////////////////////使用1
	//固定大小对象分配器
	QTH_NAME_SPACE::FixedAllocator all;

	/*第一个参数是需要分配的最小单元，  第二个参数是每一页内存最大字节数， 内存按页来进行申请， 当一页内存不够时，会使用new申请第二页内存*/
	all.Initialize(QTH_NAME_SPACE::BlockSize(sizeof(A)), QTH_NAME_SPACE::PageSize(1024)); 
	A* pA = (A*)all.Allocate(); //分配出来的是字节数组，需要手动调用构造和析构函数
	
	new (pA) A;

	pA->i = 1;

	pA->~A();
	all.Deallocate(pA);
	//////////////////////////////////////////////////////////////////////////使用2
	//使用任意大小对象池
	QTH_NAME_SPACE::SmallObjAllocator Alloc(QTH_NAME_SPACE::PageSize(4096), QTH_NAME_SPACE::MaxObjSize(8)); //每一页内存为 4096， 限制最大能够分配大小为8的对象[ps 超过的默认处理是 new]
	int* p = (int*)Alloc.Allocate(sizeof(int));
	*p = 4;

	double* pD = (double*)Alloc.Allocate(sizeof(double));
	*pD = 3.14;

	Alloc.Deallocate(p, sizeof(int));
	Alloc.Deallocate(pD, sizeof(double));

	///////////////////////////////////////////////////////////////////////////使用3
	//小对象池,  重载 new/delete
	{
		cout << "创建一个class B，然后释放..." << endl;
		B* pB = new B;
		pB->d = 3;
		pB->p = 4;
		delete pB;

		cout << "创建一个class C，然后释放..." << endl;
		C* pC = new C;
		pC->c = 1;
		pC->p = 2;
		delete pC;

		/////////////////////////////////////重载 new[]/delete[]    仅在 linux 下享受内存池效果， windows 下并未重载operator new[]/delete[]
		cout << "以new[] 创建5个class B，然后释放..." << endl;
		B* vecB = new B[5];
		for (int i = 0; i < 5; ++i)
		{
			vecB[i].d = i;
		}

		delete[] vecB;
	}
	/////////////////////////////////////////////////////////////////////////使用4
	////任意对象的内存池应用
	//{
	//	cout << "创建一个class D，然后释放..." << endl;
	//	D* pD = new D;
	//	pD->d = 1;
	//	delete pD;
	//}


	getchar();
	return 0;
}