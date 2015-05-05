#include "ProtocolGameServices.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace cocos2d { namespace plugin {

ProtocolGameServices::ProtocolGameServices()
{
	
}

ProtocolGameServices::~ProtocolGameServices()
{
	
}

void ProtocolGameServices::startSession()
{
	startSession(1);
}

void ProtocolGameServices::startSession(int forced)
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "startSession", "(I)V"))
	{
		t.env->CallVoidMethod(pData->jobj, t.methodID, forced);
		t.env->DeleteLocalRef(t.classID);
	}
}

void ProtocolGameServices::stopSession()
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "stopSession", "()V"))
	{
		t.env->CallVoidMethod(pData->jobj, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void ProtocolGameServices::initiateSignIn()
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "initiateSignIn", "()V"))
	{
		t.env->CallVoidMethod(pData->jobj, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void ProtocolGameServices::signOut()
{
	PluginUtils::callJavaFunctionWithName(this, "signOut");
}

void ProtocolGameServices::publishScore(const std::string& id, int score)
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "publishScore", "(Ljava/lang/String;I)V"))
	{
		jstring idd = t.env->NewStringUTF(id.c_str());
		t.env->CallVoidMethod(pData->jobj, t.methodID, idd, score);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(idd);
	}
}

void ProtocolGameServices::showLeaderboard(const std::string& id)
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "showLeaderboard", "(Ljava/lang/String;)V"))
	{
		jstring idd = t.env->NewStringUTF(id.c_str());
		t.env->CallVoidMethod(pData->jobj, t.methodID, idd);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(idd);
	}
}

void ProtocolGameServices::showLeaderboards()
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "showLeaderboards", "()V"))
	{
		t.env->CallVoidMethod(pData->jobj, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void ProtocolGameServices::unlockAchievement(const std::string& id)
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "unlockAchievement", "(Ljava/lang/String;)V"))
	{
		jstring idd = t.env->NewStringUTF(id.c_str());
		t.env->CallVoidMethod(pData->jobj, t.methodID, idd);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(idd);
	}
}

void ProtocolGameServices::showAchievements()
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "showAchievements", "()V"))
	{
		t.env->CallVoidMethod(pData->jobj, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

bool ProtocolGameServices::isSignedIn()
{
	return PluginUtils::callJavaBoolFuncWithName(this, "isSignedIn");
}

}} // namespace cocos2d { namespace plugin {
