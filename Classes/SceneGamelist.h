#ifndef _SCENEGAMELIST_H
#define _SCENEGAMELIST_H

#include <cocos2d.h>
using namespace cocos2d;
#include <cocos-ext.h>
using namespace cocos2d::extension;
#include <string>
using namespace std;

class SceneGameList : public CCLayer
{
public:
	CREATE_FUNC(SceneGameList);

	static CCScene* scene();

	virtual bool init();
	
private:
	void onButtonClick(CCObject* pObj,TouchEventType eventType);

	UILayer* m_pUiLayer;

	vector<string> m_paths;

	string m_rompath;
};

#endif
