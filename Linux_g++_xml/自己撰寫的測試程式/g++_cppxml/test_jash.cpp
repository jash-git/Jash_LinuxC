#include "tinyxml.h"
#include "tinystr.h"
#include <iostream>
using namespace std;
int main()
{
	//創建一個XML的文檔物件。
	TiXmlDocument *myDocument = new TiXmlDocument("test_jash.xml");
	myDocument->LoadFile();
	//獲得根元素，即Persons。
	TiXmlElement *RootElement = myDocument->RootElement();
	//輸出根元素名稱，即輸出Persons。
	cout << RootElement->Value() << endl;
	//獲得第一個Person節點。
	TiXmlElement *FirstPerson = RootElement->FirstChildElement();
	//輸出接點名Person
	cout << FirstPerson->Value() << endl;
	//獲得第一個Person的name節點和age節點和ID屬性。
	TiXmlElement *NameElement = FirstPerson->FirstChildElement();
	TiXmlElement *AgeElement = NameElement->NextSiblingElement();
	TiXmlAttribute *IDAttribute = FirstPerson->FirstAttribute();
	//輸出第一個Person的name內容，即周星星；age內容，即20；ID屬性，即1。
	cout << NameElement->FirstChild()->Value() << endl;
	cout << AgeElement->FirstChild()->Value() << endl;
	cout << IDAttribute->Value() << endl;
	return 0;
}

