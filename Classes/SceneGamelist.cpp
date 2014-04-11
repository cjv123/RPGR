#include "SceneGamelist.h"

#ifdef WIN32
#include <direct.h>
#include <Windows.h>
#endif

#ifdef WIN32
#include <iconv.h>
#endif

#ifdef ANDROID
#include <dirent.h>  
#include <sys/stat.h> 
#endif

#include "SceneMain.h"
#include "StateLoading.h"
#include "AndroidInterface.h"

CCScene* SceneGameList::scene()
{
	CCScene *scene = CCScene::create();
	SceneGameList* layer = SceneGameList::create();
	scene->addChild(layer);
	return scene;
}


bool SceneGameList::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	set_adview_visible(1);

	m_pUiLayer = UILayer::create();
	m_pUiLayer->scheduleUpdate();
	addChild(m_pUiLayer);
	UIScrollView* scrollView = UIScrollView::create();        
	scrollView->setDirection(SCROLLVIEW_DIR_VERTICAL);
	scrollView->setBounceEnabled(true);
	scrollView->setTouchEnable(true);
	scrollView->setSize(getContentSize());
	scrollView->setInnerContainerSize(scrollView->getRect().size);
	m_pUiLayer->addWidget(scrollView);


	

#ifdef WIN32
	WIN32_FIND_DATA FindFileData;

	m_rompath =CCFileUtils::sharedFileUtils()->getWritablePath() + "\RMRunnerGames";
	string szFind = m_rompath;

	szFind+="\\*.*";

	HANDLE hFind=::FindFirstFile(szFind.c_str(),&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)   
	{
		CCLOG("no this director:%s\n",szFind.c_str());
		return false;
	}

	int i=0;
	while(1)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!='.')
			{
				UIButton* button = UIButton::create();
				CCSize s = getContentSize();
				button->setScale9Enabled(true);
				button->loadTextures("button.png", "buttonHighlighted.png", "");
				button->setSize(CCSizeMake(getContentSize().width,50));
				scrollView->addChild(button);
				
				char outtmp[1024]={0};
				const char *pin = FindFileData.cFileName;
				char *pout = outtmp;
				size_t inlen = strlen(FindFileData.cFileName);
				size_t outlen = sizeof(outtmp);
				iconv_t iv= iconv_open("UTF-8","GBK");

				size_t ret =iconv(iv, &pin, &inlen, &pout,&outlen);
				if (ret !=-1)
					button->setTitleText(outtmp);

				button->setPosition(ccp(button->getRect().size.width / 2,
					scrollView->getInnerContainerSize().height - button->getRect().size.height / 2 - i*button->getRect().size.height));
				button->setTouchEnable(true);
				button->setTag(i);
				button->addTouchEventListener(this,toucheventselector(SceneGameList::onButtonClick));
				
				m_paths.push_back(FindFileData.cFileName);

				i++;
			}
			
		}
		
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind); 
#endif

#ifdef ANDROID
	m_rompath =  get_rom_path();
	const char* path = m_rompath.c_str();
	DIR* dp = NULL;
	struct dirent *dmsg;
	int i=0;
	if ((dp = opendir(path)) != NULL)
	{
		int i=0;
		while ((dmsg = readdir(dp)) != NULL)
		{

			if (!strcmp(dmsg->d_name, ".") || !strcmp(dmsg->d_name, ".."))
				continue;

			if (dmsg->d_type == DT_DIR )
			{
				char *temp;
				temp=dmsg->d_name;
				if(strchr(dmsg->d_name, '.'))
				{
					if((strcmp(strchr(dmsg->d_name, '.'), dmsg->d_name)==0))
					{
						continue;
					}
				}

				UIButton* button = UIButton::create();
				CCSize s = getContentSize();
				button->setScale9Enabled(true);
				button->loadTextures("button.png", "buttonHighlighted.png", "");
				button->setSize(CCSizeMake(getContentSize().width,50));
				scrollView->addChild(button);
				button->setTitleText(dmsg->d_name);

				button->setPosition(ccp(button->getRect().size.width / 2,
					scrollView->getInnerContainerSize().height - button->getRect().size.height / 2 - i*button->getRect().size.height));
				button->setTouchEnable(true);
				button->setTag(i);
				button->addTouchEventListener(this,toucheventselector(SceneGameList::onButtonClick));

				m_paths.push_back(dmsg->d_name);
				i++;
			}
		}
	}
	closedir(dp);
#endif



	return true;
}

void SceneGameList::onButtonClick( CCObject* pObj,TouchEventType eventType )
{
	if (eventType == TOUCH_EVENT_ENDED)
	{
		UIButton* button = (UIButton*)pObj;
		int index = button->getTag();

		SceneMain::workPath = m_rompath;
		SceneMain::workPath+="/";SceneMain::workPath+=m_paths[index] + "/";

		CCDirector::sharedDirector()->replaceScene(StateLoading::scene());
	}
}


