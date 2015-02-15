package com.jiexx.comm;

import com.jiexx.cmd.Command;

public class HttpMessage implements Message {
	public long CreateTime;
	public long ReceiveTime;
	public Command cmd;
	public int code;
	public String result;
	public HttpMessage( Command d ) {
		CreateTime = System.currentTimeMillis();
		cmd = d;
	}
	
	public void onResponse(Dispatcher d){
		// TODO Auto-generated method stub
		d.receive(this);
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

	@Override
	public void onData(Data d) {
		// TODO Auto-generated method stub
		
	}
}