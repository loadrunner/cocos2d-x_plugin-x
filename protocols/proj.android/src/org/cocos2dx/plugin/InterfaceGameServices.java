package org.cocos2dx.plugin;

public interface InterfaceGameServices {
	public final int PluginType = 7;
	
	public void setDebugMode(boolean debug);
	public String getSDKVersion();
	public String getPluginVersion();
	
	public void startSession(int forced);
	public void stopSession();
	public void initiateSignIn();
	public void signOut();
	public void showLeaderboard(String id);
	public void showLeaderboards();
	public void showAchievements();
	public void publishScore(String id, int score);
	public void unlockAchievement(String id);
	
	public boolean isSignedIn();
	
	public void joinAutoMatchMultiplayer();
	public boolean isInMultiplayerRoom();
	public void sendMessage(byte[] buff, boolean reliable);
	public byte[] readMessage();
}
