package com.qmh.bookshare.tool;

import java.util.concurrent.locks.ReentrantLock;

import android.content.Context;

import com.qmh.bookshare.util.LogUtils;
import com.qmh.bookshare.util.StringMD5;
import com.wind.customizedata.Launcher;
import com.wind.login.LoginData;
import com.wind.login.LoginRequest;
import com.wind.login.PhoneAuthenticode;
import com.wind.login.PhoneAuthenticodeRequest;
import com.wind.ui.CommunicationData;
import com.wind.ui.POST;
import com.wind.ui.RECV;

public class Session {

	public Context con;
	
	public String autoCode = null;
	public String verifyCode = null;
	public String mobile = null;
	public String authMD5 = null;
	public long userID;
	public String sessionID;
	
	enum State {
		INITIALIZED,
		OPENED,
		PREPARED,
		LOGINED,
	}
	private ReentrantLock lock = new ReentrantLock();
	public volatile State status = State.INITIALIZED;

	public Session() {
		reset();
	}
	
	public void reset() {
		if( lock.tryLock() ){
			status = State.INITIALIZED;
			lock.unlock();
		}
	}
	// when only network is available, revoke this, or must be NOTHING to do.
	public void open() {
		if( status == State.INITIALIZED && lock.tryLock()) {
			if( Launcher.INSTANCE.start(con) ) 
				status = State.OPENED;
			lock.unlock();
		}
	}
	
	public void prepare() {
		if( status == State.OPENED && lock.tryLock() ) {
			auth(mobile, 0, 3);
			lock.unlock();
		}
	}
	
	
	public void trylogin() {
		if( status == State.PREPARED && lock.tryLock()) {
			login(mobile, authMD5, 3);
			lock.unlock();
		}
	}
	
	@POST("Auth")
	public CommunicationData auth(String phone, int smsFlag, int appType) {
		PhoneAuthenticodeRequest par = new PhoneAuthenticodeRequest();
		par.params.phoneNumber = phone;
		par.params.smsFlag = smsFlag;// 1短信，0debug接口
		par.appType = appType;// android
		return new CommunicationData(par);
	}

	@RECV("Auth")
	public void onAuth(CommunicationData cd) {
		PhoneAuthenticode lr = cd.get();
		if( lr != null && lr.errCode == 0 ) {
			verifyCode = lr.results.verifyCode;
			authMD5 = StringMD5.MD5(lr.results.code);
			autoCode = lr.results.code; 
			if( lock.tryLock() ) {
				status = State.PREPARED;
				lock.unlock();
			}
			if( authCallBack != null ) {
				authCallBack.handle();
	    	}
		}
		//login(mobile, StringMD5.MD5(lr.results.code), 3);
	}

	@POST("Login")
	public CommunicationData login(String phone, String codeMD5, int appType) {
		LoginRequest lr = new LoginRequest();
		lr.appType = appType;
		lr.params.phoneNumber = phone;
		lr.params.verifyCodeMD5 = codeMD5;
		return new CommunicationData(lr);
	}

	@RECV("Login")
	public void onLogin(CommunicationData cd) {
		LoginData ld = cd.get();
		// userID = ld.results.userInfo.userID;
		if (ld == null) {
			LogUtils.e("登录失败！！！");
			return;
		}
		if (ld != null && ld.errCode == 0) {
			userID = ld.results.userInfo.userID;
			sessionID = ld.results.sessionID;
			if( lock.tryLock() ) {
				status = State.LOGINED;
				lock.unlock();
			}
			
			try {
			Launcher.INSTANCE.deamon(verifyCode, ld.results.sessionID,
					ld.results.userInfo.userType, ld.results.IMUserAccount, ld.results.IMPassword);
			} catch( Exception e ) {
				e.printStackTrace();
			}
		}
	
		if( loginCallBack != null ) {
			loginCallBack.handle();
    	}
		Launcher.INSTANCE.LOG(ld);
	}
	
    public interface CallBack {
    	void handle();
    }
    
    public CallBack loginCallBack = null;
    public CallBack authCallBack = null;
	

}
