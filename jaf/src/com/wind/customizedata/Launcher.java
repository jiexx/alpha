package com.wind.customizedata;

import android.content.Context;
import com.iwindnet.launch.WindnetLaunch;

public enum Launcher {
	INSTANCE;
	Launcher() {
	}
	public void start(Context ctx) {
		WindnetLaunch.Instance().launch(ctx);
	}
	public void login(String phone, String code) {
		WindnetLaunch.Instance().login(phone, code);
	}
}
