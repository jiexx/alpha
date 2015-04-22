package com.wind.customizedata;

import android.content.Context;
import android.os.Parcelable;
import android.util.Log;

import com.google.gson.Gson;
import com.iwindnet.client.SkyAgentWrapper;
import com.iwindnet.im.base.UserManager;
import com.iwindnet.launch.WindnetLaunch;
import com.iwindnet.message.SkyLoginRequest;
import com.iwindnet.message.SkyLoginResponse;
import com.iwindnet.message.SkyMessage;
import com.wind.ui.CommunicationData;

public enum Launcher {
	INSTANCE;
	Launcher() {
	}
	public void start(Context ctx) {
		WindnetLaunch.Instance().launch(ctx);
	}
	public void deamon(String code, String session, int type, String impwd ) {
		LOG("DEAMON", " code:"+code+" session:"+session+" type:"+type+" impwd:"+impwd);
		
		SkyLoginRequest skyLoginRequest = new SkyLoginRequest(session);

		SkyMessage messageResponse = (SkyMessage) SkyAgentWrapper.getInstance().sendMessage(skyLoginRequest, 6000);

		if (messageResponse != null) {
			SkyLoginResponse resp = new SkyLoginResponse();

			if (!resp.deserialize(
					messageResponse.getSerializedData(),
					messageResponse.getLength())) {
				return;
			}

			int retCode = resp.getReturnCode();
			if (retCode == 0) {
				UserManager.Instance().setUserId(resp.getUserId());
				UserManager.Instance().setImPasswd(impwd);
			}
			
			
			SkyAgentWrapper.getInstance().getSkyAgent().setUserId(resp.getUserId());
			SkyAgentWrapper.getInstance().getSkyAgent().setUserType(type);
			SkyAgentWrapper.getInstance().getSkyAgent().setSessionId(session);
			SkyAgentWrapper.getInstance().setLoginStatus(true);
			SkyAgentWrapper.getInstance().ResumeAllThreads();
		}
	}
	private int debug = 1;
	public void setDebug( int enable ) {
		debug = enable;
	}
	public void LOG( String head, String str ) {
		if( debug > 0 ) {
			Log.d("JAP debug", "-------------------begin "+head+"-------------------");
			Log.d("JAP debug", str);
			Log.d("JAP debug", "-------------------end   "+head+"-------------------");
		}
	}
	public void LOG( Parcelable pa ) {
		if( debug > 0 ){
			Gson gson = new Gson();
			String str = gson.toJson(pa);
			Log.i("JAP debug", "-------------------begin "+pa.getClass().getSimpleName()+"-------------------");
			Log.i("JAP debug", str);
			Log.i("JAP debug", "-------------------end   "+pa.getClass().getSimpleName()+"-------------------");
		}
	}
}
