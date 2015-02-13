package com.jiexx.comm;

import com.jiexx.cmd.Command;

public class HttpMessage implements Message {
	public long CreateTime;
	public long ReceiveTime;
	public Command cmd;
	public HttpMessage( Command d ) {
		CreateTime = System.currentTimeMillis();
		cmd = d;
	}
	
	public void onResponse( Data d ){
		
	}

	@Override
	public void onRequest(Dispatcher d) {
		// TODO Auto-generated method stub
		d.send(this);
	}

	@Override
	public void onFailed(String info) {
		// TODO Auto-generated method stub
		
	}
}