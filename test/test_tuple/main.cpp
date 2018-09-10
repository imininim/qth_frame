/*
	测试 tuple 元组
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 tuple头文件
#include "utility/tuple_base.h"
#include "utility/functional.h"
struct A
{
	A()
	{ 
		cout << "A::A" << endl;
	}
	~A()
	{
		cout << "A::~ " << endl;
	}
	A(const A& a) { 
		cout << "A::copy" << endl;
		i = a.i;
	}
	void operator=(const A& a) 
	{
		cout << "A::copy operator=" << endl;
		i = a.i;
	}
	int i;
};

void foo(){ }


//非定长度对象的序列化
struct SSizeUnFixed: public QTH_NAME_SPACE::Serializable
{
	//对象大小
	virtual size_t size() const
	{
		return sizeof(size_t) * (m_vec.size() + 1);
	}
	//序列化
	virtual size_t serialize(void* pBuffer, int len)
	{
		size_t* pIntBuffer = (size_t*)pBuffer;
		//存储数据长度
		*pIntBuffer = m_vec.size();
		++pIntBuffer;
		//存储实际数据
		for (size_t i = 0; i < m_vec.size(); ++i)
			*pIntBuffer ++ = m_vec[i];
		//返回存储的数据大小
		return size();
	}
	//反序列化
	virtual size_t unserialize(void* pBuffer, int len)
	{
		m_vec.clear();

		//读取数据长度
		size_t *pIntBuffer = (size_t*)pBuffer;
		size_t num = *pIntBuffer ++;

		for (size_t i =0 ; i < num; ++i, ++pIntBuffer)
			m_vec.push_back(*pIntBuffer);

		return size();
	}

	std::vector<int> m_vec;
};

//定长对象序列化受到对齐补齐的影响， 如果想要准确的由程序员自身把握，别偷懒， 继承于 Serializable 并实现其接口
struct SSizeFixed
{
	int i;	
	double pi;

	SSizeFixed(int i)
	{
		this->i = i;
		this->pi = 3.14;
	}
		
};

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;
	int i = 0;
	std::string str = "haha";
	double d = 3.14;
	
	cout << "-------------通常使用-----------------" << endl;
	{
		//构造
		tuple_base<TYPELIST(int&,std::string, const double)> t(i, str, d);

		//拷贝
		tuple_base<TYPELIST(int&,std::string, const double)> t2 = t;

		cout << "-------------------------" << endl;
		//输出访问
		cout << "int = " << get<int&>(t2) << endl;
		cout << "std::string = " << get<std::string>(t2) << endl;
		cout << "const double = " << get<const double>(t2) << endl;

		//修改
		get<int&>(t2) = 1;
		get<std::string>(t2) = "world";
		//get<const double>(t2) = 4.44;   //编译错误：不允许修改const

		//按照位置访问
		get<0>(t2) = 2;
		get<1>(t2) = "hello world";

		//get<2>(t2) = 4.44;
		//修改该后的数值
		cout << "---------------------------------" << endl;
		cout << "输出修改后的数值:" << endl;
		cout << "int = " << get<int&>(t2) << endl;
		cout << "std::string = " << get<std::string>(t2) << endl;
		cout << "const double = " << get<const double>(t2) << endl;

		//原始值
		cout << "-----------------------------------" << endl;
		cout << "输出引用之前的值:" << endl;
		cout << "i = " << i << endl;
		cout << "std::string = " << str << endl;
		cout << "double = " << d << endl;
	}
	//


	cout << "-------------占用内存大小-----------------" << endl;
	{
		//tuple 大小
		tuple_base<TYPELIST(int)> tu4; 					//默认构造, 一个元素
		get<0>(tu4) = 3;				 					//修改int 的值
		cout << "pos0 = " 		<< get<0>(tu4) << endl;		//输出int 的值
		cout << "type int = "	<< get<int>(tu4) << endl;	//通过类型名称来查询值
		cout << "sizeof(tu4) =" << sizeof(tu4) << endl;		//tu4 大小
		cout << "---------more----------------" << endl;
		//多个元素
		tuple_base<TYPELIST(A, int ,void(*)() )> tuMore(A(), 1, foo);	//带参数的构造
		cout << get<1>(tuMore) << endl;									//输出 int 类型的值
		get<2>(tuMore)();												//调用 函数指针
		cout << 	"sizeof(tuMore) = " << sizeof(tuMore) << endl;		//输出大小
		
		//数组类型
		tuple_base<TYPELIST(char[5])> tu;
		cout << "char[5] = " << get<char[5]>(tu) << endl;
		get<char[5]>(tu)[0] = 'z';
		cout << "after modify, char[5] = " << get<char[5]>(tu) << endl;

		char a[10] = {"abc"};
		tuple_base<TYPELIST(char[10])> tu1(a);
		cout << "char[10] = " << get<0>(tu1) << endl;

		(get<0>(tu1))[0] = 'z';
		cout << "after modify, char[10] = " << get<char[10]>(tu1) << endl;		

		//std::string str("aasdf");
		
	}

	cout << "-------------数组-----------------" << endl;
	{
		//数组默认初始化
		tuple_base<TYPELIST(A[2])> tu1;
		cout << "tu1: " << get<0>(tu1)[0].i << endl;
		cout << "tu1: " << get<0>(tu1)[1].i << endl;

		//数组单参数构造 实际是使用operator=, 不是调用类A 的拷贝构造
		A arrA[2];
		arrA[0].i = 1;
		arrA[1].i = 2;
		tuple_base<TYPELIST(A[2])> tu2(arrA);   
		cout << "tu2: " << get<0>(tu2)[0].i << endl;
		cout << "tu2: " << get<0>(tu2)[1].i << endl;

		get<0>(tu2)[0].i = 3;
		cout << "after modify i = 3, tu2: " << endl;
		cout << "tu2: " << get<0>(tu2)[0].i << endl;
		cout << "tu2: " << get<0>(tu2)[1].i << endl;

		//拷贝
		tu1 = tu2;
		cout << "tu1: " << get<0>(tu1)[0].i << endl;
		cout << "tu1: " << get<0>(tu1)[1].i << endl;
	}
	
	
	cout << "-------------序列化大小-----------------" << endl;
	{
		//固定长度的对象大小
		tuple_base<TYPELIST(SSizeFixed)> tu(SSizeFixed(1));
		cout << "size tu : " << tuple_size(tu) << endl;
		
		//字符串的数据大小
		tuple_base<TYPELIST(std::string)> tuStr;
		cout << "size tuStr: " << tuple_size(tuStr) << endl;
		get<std::string>(tuStr) = "hehe";
		cout << "4 char size tuStr: " << tuple_size(tuStr) << endl;

		//非固定长度大小对象
		tuple_base<TYPELIST(SSizeUnFixed)> tu1;
		cout << "size tu1 : " << tuple_size(tu1) << endl;

		//放几个元素进去
		get<SSizeUnFixed>(tu1).m_vec.push_back(1);
		get<SSizeUnFixed>(tu1).m_vec.push_back(2);
		get<SSizeUnFixed>(tu1).m_vec.push_back(3);

		cout << "3 size tu1 : " << tuple_size(tu1) << endl;
	}
		
	cout << "-------------序列化tuple-----------------" << endl;
	//固定长度的tuple 序列化, 反序列化
	{
		char buffer[256] = {0};
		size_t size = 0;

		//序列化
		tuple_base<TYPELIST(int, double)> tu1(1, 1.2);			
		size = tuple_serialize(tu1, buffer, 256);
		assert(size == tuple_size(tu1) );
		cout << "serialize: int=" << *((int*)buffer)  << ", double=" << *( (double*)(buffer+sizeof(int)) ) << endl;

		//反序列化
		tuple_base<TYPELIST(int, double)> tu1Copy;
		size = 0;
		size = tuple_unserialize(tu1Copy, buffer, 256);
		assert(size == tuple_size(tu1Copy) );
		cout << "unserialize: int=" << get<int>(tu1Copy) << ", double=" << get<double>(tu1Copy) << endl;

		memset(buffer, 0, 256);
		//数组序列化
		tuple_base<TYPELIST(int[3])> tu2;
		get<0>(tu2)[0] = 0;
		get<0>(tu2)[1] = 1;
		get<0>(tu2)[2] = 2;
		assert(tuple_serialize(tu2, buffer,256) == tuple_size(tu2) );
		//数组反序列化
		tuple_base<TYPELIST(int[3])> tu2Copy;
		tuple_unserialize(tu2Copy, buffer, 256);
		cout << "int arr[0] = " << get<0>(tu2Copy)[0] << endl;
		cout << "int arr[1] = " << get<0>(tu2Copy)[1] << endl;
		cout << "int arr[2] = " << get<0>(tu2Copy)[2] << endl;
	}

	//用户自定义class的序列化， 反序列化
	{
		char buffer[256] = {};
		size_t size = 0;
		//序列化
		tuple_base<TYPELIST(SSizeUnFixed)> tu1;
		get<SSizeUnFixed>(tu1).m_vec.push_back(1);
		get<SSizeUnFixed>(tu1).m_vec.push_back(2);
		assert(tuple_serialize(tu1, buffer, 256) == tuple_size(tu1) );
		//反序列化
		tuple_base<TYPELIST(SSizeUnFixed)> tu1Copy;
		tuple_unserialize(tu1Copy, buffer, 256);
		cout << "vector[0] = " << get<SSizeUnFixed>(tu1Copy).m_vec[0] << endl;
		cout << "vector[1] = " << get<SSizeUnFixed>(tu1Copy).m_vec[1] << endl;

		memset(buffer, 0, 256);

		//用户自定义类数组序列化，反序列化
		tuple_base<TYPELIST(SSizeUnFixed[2])> tu2;
		get<SSizeUnFixed[2]>(tu2)[0].m_vec.push_back(0);
		get<SSizeUnFixed[2]>(tu2)[1].m_vec.push_back(1);
		assert(tuple_serialize(tu2, buffer, 256) == tuple_size(tu2) );

		//反序列化
		tuple_base<TYPELIST(SSizeUnFixed[2])> tu2Copy;
		tuple_unserialize(tu2Copy, buffer, 256);
		cout << "SSizeUnFixed[0] = " << get<SSizeUnFixed[2]>(tu2)[0].m_vec[0] << endl;
		cout << "SSizeUnFixed[1] = " << get<SSizeUnFixed[2]>(tu2)[1].m_vec[0] << endl;
	}
	
	// std::string 序列化,反序列化
	{
		char buffer[256] = {};
		size_t size = 0;
		//序列化
		tuple_base<TYPELIST(std::string)> tu1("hehe");
		assert(tuple_serialize(tu1, buffer, 256) ==  tuple_size(tu1) );
		//反序列化
		tuple_base<TYPELIST(std::string)> tu1Copy;
		tuple_unserialize(tu1Copy, buffer, 256);
		cout << "string unserialize: " << get<std::string>(tu1Copy).c_str() << endl;

		memset(buffer, 0, 256);

		//数组序列化
		tuple_base<TYPELIST(std::string[2])> tu2;
		get<0>(tu2)[0] = "hehe0";
		get<0>(tu2)[1] = "hehe1";
		assert(tuple_serialize(tu2, buffer, 256) == tuple_size(tu2) );

		//数组反序列化
		tuple_base<TYPELIST(std::string[2])> tu2Copy;
		tuple_unserialize(tu2Copy, buffer, 256);
		cout << "std::string[0] = " << get<0>(tu2Copy)[0].c_str() << endl;
		cout << "std::string[1] = " << get<0>(tu2Copy)[1].c_str() << endl;
	}
		
	getchar();
	return 0;
}

