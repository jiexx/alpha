package com.jiexx.comm;

import com.jiexx.cmd.Command;
import com.jiexx.cmd.iMessage;

public class ImMessage implements Message {
	public long CreateTime;
	public long ReceiveTime;
	public String str;
	public ImMessage( Command d ) {
		CreateTime = System.currentTimeMillis();
	}
	
	public void onResponse( Data d ){
		
	}

	@Override
	public void onRequest(Dispatcher d) {
		// TODO Auto-generated method stub
		d.receive(this);
	}

	@Override
	public void onFailed(String info) {
		// TODO Auto-generated method stub
		
	}
	
	public void onReceive( Data d ){
		
	}

	@Override
	public void onResponse(Dispatcher d) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onData(Data d) {
		// TODO Auto-generated method stub
		
	}
}