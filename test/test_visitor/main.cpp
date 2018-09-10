/*
	测试 GOF visitor模式
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 服务头文件
#include "utility/visitor.h"

//受到访问的对象接口  文档
class DocElement: public QTH_NAME_SPACE::BaseVisitable<>  //继承于BaseVisitable, 设置其与其子类可以被访问者访问
{
public:
	virtual ~DocElement(){}
	//...

	DEFINE_VISITABLE()
};

//具体的受访问者   图像
class Img: public DocElement
{
public:
	//...
	DEFINE_VISITABLE()
};

//具体的受访问者   文字
class Text: public DocElement
{
public:
	//...
	DEFINE_VISITABLE()
};

//定义一个访问者
class MyVisitor: public QTH_NAME_SPACE::BaseVisitor,		//要求必须继承的接口
				 public QTH_NAME_SPACE::Visitor<Text>,		//这个访问者需要访问哪个对象
				 public QTH_NAME_SPACE::Visitor<Img>		//这个访问者同时访问  Text, Img 两个对象
{
public:
	virtual void Visit(Text& t)
	{
		cout << "访问一个 Text 类" << endl;
	}

	virtual void Visit(Img& i)
	{
		cout << "访问一个 Img 类" << endl;
	}
};
int main(int argc, char* argv[])
{
	MyVisitor v;		//访问者对象

	DocElement* pText = new Text;
	DocElement* pImg = new Img;

	//访问文本
	pText->Accept(v);
	//访问图像
	pImg->Accept(v);

	delete pText;
	delete pImg;
	getchar();
	return 0;
}
