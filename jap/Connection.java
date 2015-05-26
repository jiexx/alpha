package com.qmh.bookshare.tool;

import java.util.concurrent.locks.ReentrantLock;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

import com.qmh.bookshare.util.SPUtils;
import com.wind.customizedata.Launcher;

public enum Connection {
	INSTANCE;
	
	private Context context = null;
	private ConnectivityManager connectivityManager;
    private NetworkInfo info;
    
    enum OP {
    	AUTH,
    	REG,
    }
    private boolean isRegistered = false;
    public boolean isRegistered() {
    	return isRegistered;
    }
    
    public void registered(int userID, String sessionID) {
    	isRegistered = true;
    	session.userID = userID;
    	session.sessionID = sessionID;
    	if( session.loginCallBack != null ) {
    		session.loginCallBack.handle();
    	}
    }

    public void unregistered() {
    	SPUtils.put(context, "mobile", "");
		
    	isRegistered = false;
    	session.userID = 0;
    	session.sessionID = "";
    	if( session.loginCallBack != null ) {
    		session.loginCallBack.handle();
    	}
    }
    
    public JAPSession session = null;

    private BroadcastReceiver mReceiver = new BroadcastReceiver() {

        @Override
        public void onReceive(Context context, Intent intent) {
//            String action = intent.getAction();
//            if (action.equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
//                connectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
//                info = connectivityManager.getActiveNetworkInfo();  
//                if(info != null && info.isAvailable()) {
//                    Launcher.INSTANCE.LOG("network name "+ info.getTypeName() + " state"+info.getState());
//                    if( info.getState() == NetworkInfo.State.CONNECTED ) {
//                    	if( session.mobile != "" && session.mobile != null )
//                    		login(session.mobile,session.autoCallBack);
//                    }
//                } else {
//                	if( session != null )
//                		session.broken();
//                	Launcher.INSTANCE.LOG("network disconnnected.");
//                }
//            }
        }
    };
    
    private boolean active = false;
    private ReentrantLock lock = new ReentrantLock();
    private volatile OP op;
    
    public void init(Context con, Session.CallBack cb) {
    	IntentFilter mFilter = new IntentFilter();
        mFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        con.registerReceiver(mReceiver, mFilter);
        context = con;
        String mobile = (String) SPUtils.get(context, "mobile", "");
    	if( !mobile.equals("") ) {
    		isRegistered = true;
    	}else {
    		isRegistered = false;
    	}
    	active = true;
    	
    	
    	if( session == null ) {
    		session = new JAPSession();
    	}
    	session.con = con;
    	session.mobile = mobile;
    	session.reset();
    	
    	session.loginCallBack = cb;
    }
    private final int MAX = 2;
    private int counter = MAX;
    public void authorize(Context con, String mobile, Session.CallBack cb ) {
    	
        context = con;
        
        if( session == null ) {
    		session = new JAPSession();
    	}
        session.con = con;
    	session.mobile = mobile;
    	session.reset();
    	
    	session.authCallBack = cb;
        
        Thread thread = new Thread( new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				if( lock.tryLock() ) {
					try {
						counter = MAX;
				    	while( active  && counter-- > 0 && session.status != Session.State.OPENED ) {
				    		session.open();
				    		Launcher.INSTANCE.LOG(session.status.toString());
				    		Thread.sleep(2000);
				    	}
				    	counter = MAX;
				    	while( active && counter-- > 0 && session.status != Session.State.PREPARED ) {
				    		session.prepare();
				    		Launcher.INSTANCE.LOG(session.status.toString());
				    		Thread.sleep(2000);
				    	}
			    	}catch(Exception e){
						Launcher.INSTANCE.LOG(e.getMessage());
						e.printStackTrace();		
					}
					lock.unlock();
				}
			}
        });
        thread.start();
    }

    public void destroy() {
    	context.unregisterReceiver(mReceiver);
    	Launcher.INSTANCE.LOG("destroy");
    	active = false;
    }
    
    public void handle() {
    	if( lock.tryLock() ) {
			try {
				
				counter = MAX;
		    	while( active && counter-- > 0 && session.status != Session.State.OPENED ) {
		    		session.open();
		    		Launcher.INSTANCE.LOG("open. " +counter);
		    		Thread.sleep(2000);
		    	}
		    	counter = MAX;
		    	while( active && counter-- > 0 && session.status != Session.State.PREPARED ) {
		    		session.prepare();
		    		Launcher.INSTANCE.LOG("prepare. "+counter);
		    		Thread.sleep(2000);
		    	}
		    	counter = MAX;
		    	while( active && counter-- > 0 && session.status != Session.State.LOGINED ) {
		    		session.trylogin();
		    		Launcher.INSTANCE.LOG("trylogin. "+counter);
		    		Thread.sleep(2000);
		    	}
			    	
			}catch(Exception e){
				Launcher.INSTANCE.LOG(e.getMessage());
				e.printStackTrace();		
			}
			lock.unlock();
		}
    }
    
    private Thread trylogin;
    
    public void login(Context con, String mobile, Session.CallBack cb ) {
    	if( session == null ) {
    		session = new JAPSession();
    	}
    	session.con = con;
    	session.mobile = mobile;
    	session.reset();
    	
    	session.loginCallBack = cb;
    	
    	if( trylogin != null && trylogin.isAlive() ) {
    		trylogin.interrupt();
    		Thread.yield();
    	}
    	
    	trylogin = new Thread( new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				handle();
				
			}
        	
        });
    	trylogin.start();
    }
    


}

