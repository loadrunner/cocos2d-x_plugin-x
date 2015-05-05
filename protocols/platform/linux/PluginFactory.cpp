#include "PluginFactory.h"
#include "ProtocolAnalytics.h"
#include "ProtocolGameServices.h"
#include "ProtocolIAP.h"
#include <cstring>

namespace cocos2d { namespace plugin {

PluginFactory::PluginFactory()
{

}

PluginFactory::~PluginFactory()
{

}

PluginFactory* PluginFactory::getInstance()
{
	return 0;
}

void PluginFactory::purgeFactory()
{
	
}

PluginProtocol* PluginFactory::createPlugin(const char* name)
{
	if (strcmp(name, "GoogleAnalytics") == 0)
		return new ProtocolAnalytics();
	else if (strcmp(name, "GpsGames") == 0)
		return new ProtocolGameServices();
	else if (strcmp(name, "IAPGooglePlay") == 0)
		return new ProtocolIAP();
	
	return nullptr;
}

}} //namespace cocos2d { namespace plugin {
