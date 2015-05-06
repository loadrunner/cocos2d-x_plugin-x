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

void ProtocolGameServices::joinAutoMatchMultiplayer()
{
	PluginUtils::callJavaFunctionWithName(this, "joinAutoMatchMultiplayer");
}

bool ProtocolGameServices::isInMultiplayerRoom()
{
	return PluginUtils::callJavaBoolFuncWithName(this, "isInMultiplayerRoom");
}

void ProtocolGameServices::sendMessage(const unsigned char* buff, const int size, bool reliable/* = false */)
{
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "sendMessage", "([BZ)V"))
	{
		jbyteArray jBuff = t.env->NewByteArray(size);
		t.env->SetByteArrayRegion(jBuff, 0, size, (jbyte*) buff);
		t.env->CallVoidMethod(pData->jobj, t.methodID, jBuff, reliable);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(jBuff);
	}
}

unsigned char* ProtocolGameServices::readMessage()
{
	unsigned char* buff = nullptr;
	int size = 0;
	
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(t, pData->jclassName.c_str(), "readMessage", "()[B"))
	{
		jbyteArray jBuff = (jbyteArray) t.env->CallObjectMethod(pData->jobj, t.methodID);
		buff = (unsigned char*) t.env->GetByteArrayElements(jBuff, JNI_FALSE);
		size = t.env->GetArrayLength(jBuff);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(jBuff);
	}
	
	return size > 0 ? buff : nullptr;
}
/*
extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_IAPWrapper_nativeOnMessageReceived(JNIEnv* env, jobject thiz, jstring className, jbyteArray jBuff)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("ProtocolGameServices", "nativeOnMessageReceived(), Get plugin ptr : %p", pPlugin);
		if (pPlugin != NULL)
		{
			PluginUtils::outputLog("ProtocolGameServices", "nativeOnMessageReceived(), Get plugin name : %s", pPlugin->getPluginName());
			ProtocolGameServices* pGS = dynamic_cast<ProtocolGameServices*>(pPlugin);
			if (pGS != NULL)
			{
				char* buff = (char*) env->GetByteArrayElements(jBuff, JNI_FALSE);
				int size = env->GetArrayLength(jBuff);
				
			//	pGS->onPayResult((PayResultCode) ret, strMsg.c_str());
			//	ProtocolIAP::ProtocolIAPCallback callback = pGS->getCallback();
			//	if(callback)
			//		callback(ret, strMsg);
			}
		}
	}
}
*/
}} // namespace cocos2d { namespace plugin {
