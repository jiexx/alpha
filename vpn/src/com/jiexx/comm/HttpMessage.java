package com.jiexx.comm;

public class HttpMessage implements Message {
	public void onResponse( Data d ){
		
	}

	@Override
	public void onRequest(Dispatcher d) {
		// TODO Auto-generated method stub
		d.send(this);
	}
}