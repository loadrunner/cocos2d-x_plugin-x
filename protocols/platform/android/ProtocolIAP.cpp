#include "ProtocolIAP.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace cocos2d { namespace plugin {

extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_IAPWrapper_nativeOnPayResult(JNIEnv*  env, jobject thiz, jstring className, jint ret, jstring msg)
	{
		std::string strMsg = PluginJniHelper::jstring2string(msg);
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("ProtocolIAP", "nativeOnPayResult(), Get plugin ptr : %p", pPlugin);
		if (pPlugin != NULL)
		{
			PluginUtils::outputLog("ProtocolIAP", "nativeOnPayResult(), Get plugin name : %s", pPlugin->getPluginName());
			ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(pPlugin);
			if (pIAP != NULL)
			{
				pIAP->onPayResult((PayResultCode) ret, strMsg.c_str());
				ProtocolIAP::ProtocolIAPCallback callback = pIAP->getCallback();
				if(callback)
					callback(ret, strMsg);
			}
		}
	}
	
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_IAPWrapper_nativeOnInventoryResult(JNIEnv*  env, jobject thiz, jstring className, jobjectArray inventory)
	{
		JNIEnv *pEnv = PluginJniHelper::getEnv();
		
		std::vector<std::string> skuList;
		jsize len = pEnv->GetArrayLength(inventory);
		PluginUtils::outputLog("ProtocolIAP", "nativeOnInventoryResult() ---- size %i", len);
		for (int i = 0; i < len; i++)
		{
			jobject myobj = pEnv->GetObjectArrayElement(inventory, i);
			PluginUtils::outputLog("ProtocolIAP", "nativeOnInventoryResult() ---- item %s", PluginJniHelper::jstring2string((jstring) myobj).c_str());
			skuList.push_back(PluginJniHelper::jstring2string((jstring) myobj));
		}
		
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("ProtocolIAP", "nativeOnInventoryResult(), Get plugin ptr : %p", pPlugin);
		if (pPlugin != NULL)
		{
			PluginUtils::outputLog("ProtocolIAP", "nativeOnInventoryResult(), Get plugin name : %s", pPlugin->getPluginName());
			ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(pPlugin);
			if (pIAP != NULL)
			{
				ProtocolIAP::ProtocolIAPInventoryCallback callback = pIAP->getInventoryCallback();
				if(callback)
					callback(skuList);
			}
		}
	}
}

bool ProtocolIAP::_paying = false;

ProtocolIAP::ProtocolIAP()
: _listener(NULL)
{
}

ProtocolIAP::~ProtocolIAP()
{
}

void ProtocolIAP::configDeveloperInfo(TIAPDeveloperInfo devInfo)
{
	if (devInfo.empty())
	{
		PluginUtils::outputLog("ProtocolIAP", "The developer info is empty!");
		return;
	}
	else
	{
		PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
		PluginJniMethodInfo t;
		if (PluginJniHelper::getMethodInfo(t
			, pData->jclassName.c_str()
			, "configDeveloperInfo"
			, "(Ljava/util/Hashtable;)V"))
		{
			// generate the hashtable from map
			jobject obj_Map = PluginUtils::createJavaMapObject(&devInfo);

			// invoke java method
			t.env->CallVoidMethod(pData->jobj, t.methodID, obj_Map);
			t.env->DeleteLocalRef(obj_Map);
			t.env->DeleteLocalRef(t.classID);
		}
	}
}

void ProtocolIAP::payForProduct(TProductInfo info)
{
	if (_paying)
	{
		PluginUtils::outputLog("ProtocolIAP", "Now is paying");
		return;
	}

	if (info.empty())
	{
		if (NULL != _listener)
		{
			onPayResult(kPayFail, "Product info error");
		}
		PluginUtils::outputLog("ProtocolIAP", "The product info is empty!");
		return;
	}
	else
	{
		_paying = true;
		_curInfo = info;

		PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
		PluginJniMethodInfo t;
		if (PluginJniHelper::getMethodInfo(t
			, pData->jclassName.c_str()
			, "payForProduct"
			, "(Ljava/util/Hashtable;)V"))
		{
			// generate the hashtable from map
			jobject obj_Map = PluginUtils::createJavaMapObject(&info);

			// invoke java method
			t.env->CallVoidMethod(pData->jobj, t.methodID, obj_Map);
			t.env->DeleteLocalRef(obj_Map);
			t.env->DeleteLocalRef(t.classID);
		}
	}
}

void ProtocolIAP::payForProduct(TProductInfo info, ProtocolIAPCallback cb)
{
	_callback = cb;
	payForProduct(info);
}

void ProtocolIAP::setResultListener(PayResultListener* pListener)
{
	_listener = pListener;
}

void ProtocolIAP::onPayResult(PayResultCode ret, const char* msg)
{
	_paying = false;
	if (_listener)
	{
		_listener->onPayResult(ret, msg, _curInfo);
	}
	else
	{
		PluginUtils::outputLog("ProtocolIAP", "Result listener is null!");
	}
	_curInfo.clear();
	PluginUtils::outputLog("ProtocolIAP", "Pay result is : %d(%s)", (int) ret, msg);
}

}} // namespace cocos2d { namespace plugin {
