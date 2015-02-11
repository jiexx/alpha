package com.jiexx.comm;

public class HttpMessage implements Message {
	private Data mData;
	public long CreateTime;
	public long ReceiveTime;
	public HttpMessage( Data d ) {
		CreateTime = System.currentTimeMillis();
		mData = d;
	}
	
	public String toStr() throws Exception {
		return mData.getBase64Str();
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