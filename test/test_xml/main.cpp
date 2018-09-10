/*
	测试 xml 解析器
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 xml 头文件
#include "xml/tinyxml2.h"

/*
	创建xml
*/
void createXML(void)
{
	using namespace QTH_NAME_SPACE;

	 const char* declaration ="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	 XMLDocument doc;
	 XMLError errCode = doc.Parse(declaration);
	 if (errCode != XML_SUCCESS)
	 {
		 cout << "解析xml字符串发生错误: " << doc.ErrorIDToName(errCode) << endl;
		 return;
	 }

	 XMLElement* root = doc.NewElement("root");			//根节点
	 XMLElement* item1 = doc.NewElement("item1");		//根节点下第一个子节点
	 XMLElement* item2 = doc.NewElement("item2");		//根节点下第二个子节点

	 //设置xml节点的属性
	 item1->SetAttribute("it1_type1", 0);
	 item1->SetAttribute("it1_type2", 1);

	 item2->SetAttribute("it2_type1", 3);
	 item2->SetAttribute("it2_type2", 4);

	 root->SetAttribute("rootID", "hello_root");

	 //两个子节点插入到根节点下
	 root->InsertEndChild(item1);
	 root->InsertEndChild(item2);

	 //根节点插入到xml文档中
	 doc.InsertEndChild(root);

	 //保存xml
	 doc.SaveFile("./createXML.xml");
	 doc.Print();
}

/*查询XML*/
void findXML(void)
{
	using namespace QTH_NAME_SPACE;
	XMLDocument doc;
	 XMLError errCode = doc.LoadFile("./userAccount.xml");
	 if (errCode != XML_SUCCESS)
	 {
		 cout << "解析xml字符串发生错误: " << doc.ErrorIDToName(errCode) << endl;
		 return;
	 }

	 //获取根节点
	 XMLElement* root = doc.RootElement();
	 if (!root)
	 {
		 cout << "查找不到根节点" << endl;
		 return;
	 }

	 //输出 item1  节点
	 XMLElement* item = root->FirstChildElement("item1");
	 while (item)
	 {
		 const char* pType1 = item->Attribute("it1_type1");
		 if (pType1)
			cout << "item:  it1_type1=" << pType1 << endl;

		 const char* pType2 = item->Attribute("it1_type2");
		 if (pType2)
			cout << "item:  it1_type2=" << pType2 << endl;

		 const char* pNull  = item->Attribute("null_exist");	//获取一个不存在的属性
		 if (pNull)
			cout << "item:  null_exist=" << pNull << endl;
		 else
			 cout << "item: null_exist 属性不存在" << endl;

		 const char* pTye21 = item->Attribute("it2_type1");
		
		 item = item->NextSiblingElement("item1");  //下一个 item1 节点
	 }

	 cout << "------item2-----------------------" << endl;
	 //输出 item2  节点
	 XMLElement* item2 = root->FirstChildElement("item2");
	 while (item2)
	 {
		 const char* pType1 = item2->Attribute("it2_type1");
		 if (pType1)
			cout << "item2:  it2_type1=" << pType1 << endl;

		 const char* pType2 = item2->Attribute("it2_type2");
		 if (pType2)
			cout << "item2:  it2_type2=" << pType2 << endl;

		 const char* pNull  = item2->Attribute("null_exist");	//获取一个不存在的属性
		 if (pNull)
			cout << "item2:  null_exist=" << pNull << endl;
		 else
			  cout << "item2: null_exist 属性不存在" << endl;

		 item2 = item2->NextSiblingElement("item2");  //下一个 item2 节点
	 }


}

/*修改XML*/
void modifyXML()
{
	using namespace QTH_NAME_SPACE;
	XMLDocument doc;
	XMLError errCode = doc.LoadFile("./createXML.xml");
	if (errCode != XML_SUCCESS)
	{
		cout << "解析xml字符串发生错误: " << doc.ErrorIDToName(errCode) << endl;
		return;
	}

	//获取根节点
	XMLElement* root = doc.RootElement();
	if (!root)
	{
		cout << "查找不到根节点" << endl;
		return;
	}

	XMLElement* item2 = root->FirstChildElement("item2");
	if (item2)
	{
		item2->SetAttribute("it2_type1", "modify"); //重新设定属性值
	}

	doc.SaveFile("./createXML.xml");

	doc.Print();
}

/*删除XML*/
void deleteXML()
{
	using namespace QTH_NAME_SPACE;
	XMLDocument doc;
	XMLError errCode = doc.LoadFile("./createXML.xml");
	if (errCode != XML_SUCCESS)
	{
		cout << "解析xml字符串发生错误: " << doc.ErrorIDToName(errCode) << endl;
		return;
	}

	//获取根节点
	XMLElement* root = doc.RootElement();
	if (!root)
	{
		cout << "查找不到根节点" << endl;
		return;
	}

	XMLElement* item2 = root->FirstChildElement("item2");
	while (item2)
	{
		XMLElement* pItem = item2;
		item2 = item2->NextSiblingElement("item2"); 

		root->DeleteChild(pItem);		//删除item2 节点
	}

	doc.SaveFile("./createXML.xml");
	doc.Print();
}


int main(int argc, char* argv[])
{
	//创建一个 XML  ./createXML.xml
	/*cout << "create -----------" << endl;
	createXML();
	cout << endl << endl;
*/
	//查询	XML ./createXML.xml
	cout << "find -----------" << endl;
	findXML();
	cout << endl << endl;
/*
	//修改 XML ./createXML.xml item2 这个节点
	cout << "modify -----------" << endl;
	modifyXML();
	cout << endl << endl;

	//删除 XML ./createXML.xml item2 这个节点
	cout << "delete node -----------" << endl;
	deleteXML();
*/
	cout << endl << endl;
	getchar();
	return 0;
}
