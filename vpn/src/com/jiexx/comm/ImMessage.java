package com.jiexx.comm;

public class ImMessage implements Message {
	private Data mData;
	
	public ImMessage( Data d ) {
		mData = d;
	}
	
	public Data getData() {
		return mData;
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