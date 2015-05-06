package org.cocos2dx.plugin;

import java.util.concurrent.ConcurrentLinkedQueue;

import org.cocos2dx.libgpsgames.R;

import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.multiplayer.Participant;
import com.google.android.gms.games.multiplayer.realtime.RealTimeMessage;
import com.google.android.gms.games.multiplayer.realtime.RealTimeMessageReceivedListener;
import com.google.android.gms.games.multiplayer.realtime.Room;
import com.google.android.gms.games.multiplayer.realtime.RoomConfig;
import com.google.android.gms.games.multiplayer.realtime.RoomUpdateListener;
import com.google.games.basegameutils.GameHelper;

import android.app.Activity;
import android.content.Intent;
import android.util.Log;

public class GpsGames implements InterfaceGameServices, GameHelper.GameHelperListener, PluginListener,
		RoomUpdateListener, RealTimeMessageReceivedListener/*, RoomStatusUpdateListener*/ {
	protected static final String TAG = "GpsGames";
	private Activity mContext = null;
	public static GameHelper mHelper;
	private boolean mIsDebug = true;
	private boolean mIsSignedIn = false;
	
	public GpsGames(Activity context) {
		mContext = context;
	}
	
	@Override
	public void setDebugMode(boolean isDebugMode) {
		mIsDebug = isDebugMode;
		
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				getGameHelper().enableDebugLog(mIsDebug);
			}
		});
	}
	
	public GameHelper getGameHelper() {
		if (mHelper == null) {
			mHelper = new GameHelper(mContext, GameHelper.CLIENT_GAMES);
			mHelper.enableDebugLog(mIsDebug);
		}
		return mHelper;
	}
	
	@Override
	public void startSession(final int forced) {
		Log.e(TAG, "start sess");
		
		PluginWrapper.addListener(this);
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				if (mHelper == null) {
					getGameHelper();
				}
				mHelper.setMaxAutoSignInAttempts(forced);
				mHelper.setup(GpsGames.this);
				mHelper.onStart(mContext);
			}
		});
	}
	
	@Override
	public void stopSession() {
		Log.e(TAG, "stop sess");
		
		PluginWrapper.removeListener(this);
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				mHelper.onStop();
			}
		});
	}
	
	@Override
	public void initiateSignIn() {
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				getGameHelper().beginUserInitiatedSignIn();
			}
		});
	}
	
	@Override
	public void publishScore(final String id, final int score) {
		if (!mIsSignedIn || id == null || id.length() == 0)
			return;
		
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				Games.Leaderboards.submitScore(getGameHelper().getApiClient(), id, score);
			}
		});
	}
	
	@Override
	public void showLeaderboard(final String id) {
		if (!mIsSignedIn || id == null || id.length() == 0)
			return;
		
		mContext.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(getGameHelper().getApiClient(), id), 0);
	}
	
	@Override
	public void showLeaderboards() {
		if (!mIsSignedIn)
			return;
		
		mContext.startActivityForResult(Games.Leaderboards.getAllLeaderboardsIntent(getGameHelper().getApiClient()), 0);
	}
	
	@Override
	public void unlockAchievement(final String id) {
		if (!mIsSignedIn || id == null || id.length() == 0)
			return;
		
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				Games.Achievements.unlock(getGameHelper().getApiClient(), id);
			}
		});
	}
	
	@Override
	public void showAchievements() {
		if (!mIsSignedIn)
			return;
		
		mContext.startActivityForResult(Games.Achievements.getAchievementsIntent(getGameHelper().getApiClient()), 0);
	}
	
	@Override
	public boolean isSignedIn() {
		return mIsSignedIn;
	}
	
	@Override
	public void signOut() {
		mIsSignedIn = false;
		
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				getGameHelper().signOut();
			}
		});
	}
	
	@Override
	public String getSDKVersion() {
		return mContext.getString(R.integer.google_play_services_version);
	}
	
	@Override
	public String getPluginVersion() {
		return "0.0.1";
	}
	
	@Override
	public void onSignInFailed() {
		Log.e(TAG, "signin failed");
		
		mIsSignedIn = false;
	}
	
	@Override
	public void onSignInSucceeded() {
		Log.e(TAG, "signin succeded");
		
		mIsSignedIn = true;
	}
	
	@Override
	public void onResume() {
		
	}
	
	@Override
	public void onPause() {
		
	}
	
	@Override
	public void onDestroy() {
		
	}
	
	@Override
	public boolean onActivityResult(int requestCode, int resultCode, Intent data) {
		if (mHelper != null)
			return mHelper.onActivityResult(requestCode, resultCode, data);
		
		return false;
	}
	
	//  --------- MULTIPLAYER ---------------
	
	private boolean mConnectedToRoom = false;
	private Room mRoom;
	private ConcurrentLinkedQueue<byte[]> mMessageQueue = new ConcurrentLinkedQueue<byte[]>();
	
	@Override
	public void joinAutoMatchMultiplayer() {
		if (!mIsSignedIn)
			return;
		
		Log.e("gamegame", "init create");
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				RoomConfig.Builder roomConfigBuilder = RoomConfig.builder(GpsGames.this);
				roomConfigBuilder.setMessageReceivedListener(GpsGames.this);
			//	roomConfigBuilder.setRoomStatusUpdateListener(GpsGames.this);
				roomConfigBuilder.setAutoMatchCriteria(RoomConfig.createAutoMatchCriteria(1, 1, 0));
				
				Games.RealTimeMultiplayer.create(GpsGames.mHelper.getApiClient(), roomConfigBuilder.build());
			}
		});
	}
	
	@Override
	public boolean isInMultiplayerRoom() {
		return mConnectedToRoom;
	}
	
	@Override
	public void sendMessage(byte[] buff, boolean reliable) {
		if (!mConnectedToRoom)
			return;
		
		Log.e("gamegame", "sending  id " + (int) buff[0] +"_" +(int) buff[1]+ ", command " + (int) buff[2] + ", data " + (int) buff[3] + " + " + (int) buff[4]);
		
		if (reliable) {
			for (Participant p : mRoom.getParticipants()) {
				if (!p.getParticipantId().equals(mRoom.getParticipantId(Games.Players.getCurrentPlayerId(GpsGames.mHelper.getApiClient())))) {
					Games.RealTimeMultiplayer.sendReliableMessage(GpsGames.mHelper.getApiClient(), null, buff, mRoom.getRoomId(), p.getParticipantId());
				}
			}
		} else {
			Games.RealTimeMultiplayer.sendUnreliableMessageToOthers(GpsGames.mHelper.getApiClient(), buff, mRoom.getRoomId());
		}
	}
	
	@Override
	public byte[] readMessage() {
		byte[] buff = mMessageQueue.poll();
		
		if (buff == null)
			buff = new byte[0];
		
		return buff;
	}
	
	@Override
	public void onRoomCreated(int statusCode, Room room) {
		Log.e("gamegame", "room created");
	}
	
	@Override
	public void onRoomConnected(int statusCode, Room room) {
		Log.e("gamegame", "room connected");
		
		if (statusCode != GamesStatusCodes.STATUS_OK) {
			return;
		}
		
		if (shouldStartGame(room)) {
			Log.e("gamegame", "game started!!!!!");
			
			mConnectedToRoom = true;
			mRoom = room;
		}
	}
	
	@Override
	public void onJoinedRoom(int statusCode, Room room) {
		Log.e("gamegame", "room joined");
	}
	
	@Override
	public void onLeftRoom(int statusCode, String roomId) {
		Log.e("gamegame", "room left");
	}
	/*
	@Override
	public void onConnectedToRoom(Room room) {
		Log.e("gamegame", "connected to room ..");
	}
	
	@Override
	public void onDisconnectedFromRoom(Room room) {
		Log.e("gamegame", "disconnected to room ..");
		
		Games.RealTimeMultiplayer.leave(GpsGames.mHelper.getApiClient(), null, room.getRoomId());
	}
	
	@Override
	public void onRoomAutoMatching(Room room) {
		Log.e("gamegame", "room automatching");
	}
	
	@Override
	public void onRoomConnecting(Room room) {
		Log.e("gamegame", "room connecting");
	}
	
	@Override
	public void onP2PConnected(String participantId) {
		Log.e("gamegame", "p2p connect");
	}
	
	@Override
	public void onP2PDisconnected(String participantId) {
		Log.e("gamegame", "p2p disconnect");
	}
	
	@Override
	public void onPeerDeclined(Room room, List<String> arg1) {
		Log.e("gamegame", "peer declined");
	}
	
	@Override
	public void onPeerInvitedToRoom(Room room, List<String> arg1) {
		Log.e("gamegame", "peer invite");
	}
	
	@Override
	public void onPeerJoined(Room room, List<String> arg1) {
		Log.e("gamegame", "peer joined");
	}
	
	@Override
	public void onPeerLeft(Room room, List<String> arg1) {
		Log.e("gamegame", "peer left");
		
		Games.RealTimeMultiplayer.leave(GpsGames.mHelper.getApiClient(), null, room.getRoomId());
	}
	
	@Override
	public void onPeersConnected(Room room, List<String> arg1) {
		Log.e("gamegame", "peers connected");
	}
	
	@Override
	public void onPeersDisconnected(Room room, List<String> arg1) {
		Log.e("gamegame", "peers disconnected");
		
		Games.RealTimeMultiplayer.leave(GpsGames.mHelper.getApiClient(), null, room.getRoomId());
	}
	*/
	
	@Override
	public void onRealTimeMessageReceived(RealTimeMessage message) {
		final byte[] buff = message.getMessageData();
		mMessageQueue.add(buff);
		Log.e("gamegame", "RTM received!! : id " + (int) buff[0] +"_" +(int) buff[1]+ ", command " + (int) buff[2] + ", data " + (int) buff[3] + " + " + (int) buff[4]);
		/*
		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				nativeOnMessageReceived("org/cocos2dx/plugin/GpsGames", buff);
			}
		});
		*/
	}
	
	// returns whether there are enough players to start the game
	boolean shouldStartGame(Room room) {
		int connectedPlayers = 0;
		for (Participant p : room.getParticipants()) {
			if (p.isConnectedToRoom()) ++connectedPlayers;
		}
		return connectedPlayers >= 2;
	}
	
//	private native void nativeOnMessageReceived(String className, byte[] msg);
}
