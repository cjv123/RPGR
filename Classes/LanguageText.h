#ifndef _LANGUAGETEXT_H
#define _LANGUAGETEXT_H

#include <string>
#include <map>
using namespace std;

class LanguageText
{
public:
	static LanguageText* getInstance();
	void parse();
	map<string,string>& getStringMap();
	string getString(const char* key);
private:
	
	LanguageText();
	~LanguageText();

	map<string,string> mStringMap;
};

#endif
