package org.cocos2dx.plugin;

import org.cocos2dx.libgpsgames.R;

import com.google.android.gms.games.Games;
import com.google.games.basegameutils.GameHelper;

import android.app.Activity;
import android.content.Intent;
import android.util.Log;

public class GpsGames implements InterfaceGameServices, GameHelper.GameHelperListener, PluginListener {
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
}
