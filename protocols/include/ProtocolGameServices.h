#ifndef  __CCX_PROTOCOL_GAMES_H__
#define  __CCX_PROTOCOL_GAMES_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace cocos2d { namespace plugin {

class ProtocolGameServices : public PluginProtocol
{
public:
	ProtocolGameServices();
	virtual ~ProtocolGameServices();
	
	void startSession();
	void startSession(int forced);
	void stopSession();
	void initiateSignIn();
	void signOut();
	bool isSignedIn();
	void showLeaderboard(const std::string& id);
	void showLeaderboards();
	void showAchievements();
	void publishScore(const std::string& id, int score);
	void unlockAchievement(const std::string& id);
	
protected:
	
};

}} // namespace cocos2d { namespace plugin {

#endif   /* ----- #ifndef __CCX_PROTOCOL_GAMES_H__ ----- */
