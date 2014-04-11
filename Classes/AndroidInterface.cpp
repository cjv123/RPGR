#include "AndroidInterface.h"
#include <cocos2d.h>
using namespace cocos2d;
int g_clickad_flag = 0;

#ifdef ANDROID
JavaVM* g_javaVM = NULL;
jclass g_cls;

JNIEXPORT void JNICALL Java_com_zfteam_RMRunner_RMRunner_initJVM(JNIEnv *env, jclass)
{
	CCLOG("init jvm bengin");
	env->GetJavaVM(&g_javaVM);
	CCLOG("init jvm end");
}

jmethodID getMethodID(JNIEnv* env, const char* method, const char* parames)
{
	jclass g_cls =  env->FindClass("com/zfteam/RMRunner/RMRunner");
	if (!g_cls)
	{
		CCLOG("com/zfteam/RMRunner/RMRunner class is not found.");
		return NULL;
	}
	jmethodID methodId = env->GetStaticMethodID(g_cls, method,parames);
	return methodId;
}
#endif

void set_adview_visible(int showflag)
{
#ifdef ANDROID
	JNIEnv *env;
	g_javaVM->AttachCurrentThread(&env, NULL);
	jmethodID methodId = getMethodID(env,"setAdViewVisable","(I)V");

	if (!methodId)
	{
		CCLOG("setAdViewVisable function is not found.");
		return;
	}

	env->CallStaticObjectMethod( g_cls, methodId,showflag);
#endif
}


void exit_game()
{
#ifdef ANDROID
	JNIEnv *env;
	g_javaVM->AttachCurrentThread(&env, NULL);
	jmethodID methodId = getMethodID(env,"onClickBackButton","()V");

	if (!methodId)
	{
		CCLOG("onClickBackButton function is not found.");
		return;
	}

	env->CallStaticObjectMethod(g_cls, methodId);
#endif
}

std::string get_rom_path()
{
#ifdef ANDROID
	JNIEnv *env;
	g_javaVM->AttachCurrentThread(&env, NULL);
	jmethodID methodId = getMethodID(env,"getROMPath","()Ljava/lang/String;");

	if (!methodId)
	{
		CCLOG("getROMPath function is not found.");
		return "";
	}

	jstring jstr = (jstring)env->CallStaticObjectMethod(g_cls, methodId);
	char* path = (char*) env->GetStringUTFChars(jstr, 0);
	return path;
#endif
	return "";
}


