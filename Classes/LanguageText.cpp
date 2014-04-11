#include "LanguageText.h"
#include <cocos2d.h>
#include "support\tinyxml2\tinyxml2.h"
using cocos2d::CCFileUtils;

LanguageText* LanguageText::getInstance()
{
	static LanguageText instance;
	return &instance;
}

LanguageText::LanguageText()
{
	parse();
}

LanguageText::~LanguageText()
{
	
}

void LanguageText::parse()
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument;
	unsigned long nSize;
	const char* pXmlBuffer = (const char*)CCFileUtils::sharedFileUtils()->getFileData("language_cn.xml", "rb", &nSize);
	doc->Parse(pXmlBuffer);
	tinyxml2::XMLElement* rootElement = doc->RootElement();    
	tinyxml2::XMLElement* word = rootElement->FirstChildElement();
	int i=0;
	for (; word != NULL; word = word->NextSiblingElement() ) 
	{
		const char* key = word->Attribute("key");
		const char* text = word->Attribute("text");

		if (key && text)
		{
			mStringMap.insert(map<string,string>::value_type(key,text));
		}
		
		i++;
	}

	delete[] pXmlBuffer;
}

map<string,string>& LanguageText::getStringMap()
{
	return mStringMap;
}

std::string LanguageText::getString( const char* key )
{
	return mStringMap[key];
}
