#include "ProtocolIAP.h"

namespace cocos2d { namespace plugin {

ProtocolIAP::ProtocolIAP() : _listener(nullptr)
{
	
}

ProtocolIAP::~ProtocolIAP()
{
	
}

void ProtocolIAP::configDeveloperInfo(TIAPDeveloperInfo devInfo)
{
	
}
void ProtocolIAP::payForProduct(TProductInfo info)
{
	if (_callback)
		_callback(PayResultCode::kPaySuccess, info.at("IAPId"));
}

void ProtocolIAP::payForProduct(TProductInfo info, ProtocolIAPCallback cb)
{
	_callback = cb;
	payForProduct(info);
}

}} // namespace cocos2d { namespace plugin {
