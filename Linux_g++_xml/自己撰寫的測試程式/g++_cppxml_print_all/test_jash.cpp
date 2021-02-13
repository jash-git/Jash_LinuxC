#include "tinyxml.h"
#include "tinystr.h"
#include <iostream>
#include <cstdio>
using namespace std;
void printNode(TiXmlNode* node);
int main(int argc, char* argv[])
{
	TiXmlDocument doc("test_jash.xml");
	doc.LoadFile();
	TiXmlNode *root = doc.RootElement();
	printNode(root);
	return 0;
}

void printNode(TiXmlNode* node) //迴圈列印所有的節點
{
	if(node == NULL) return;
		cout<<"["<<node->Value()<<"]"<<endl; //顯示節點的名稱
	TiXmlElement* element = node->ToElement();
	
	if(element && element->GetText()) //顯示節點的內容
           	cerr<<"      -->"<<element->GetText()<<endl;
	
	TiXmlElement* child = node->FirstChildElement();
	while(child!=NULL)
	{
		printNode(child); //遞迴調用顯示節點
		child = child->NextSiblingElement();
	}
}
