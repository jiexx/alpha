package com.jiexx.comm;

public class Communication {
	private class ComHttpMessage extends HttpMessage {

		public ComHttpMessage(Data d) {
			super(d);
			// TODO Auto-generated constructor stub
		}
		public void onResponse( Data d ){
			onHttpMessage(d);
		}
		
	}
	public void postHttpMessage( String cmd, Data d ) {
		Dispatcher.self().
	}
	public void onHttpMessage( Data d ) {
		
	}
	public void postImMessage( String who, String msg ) {
		
	}
	public void onImMessage( String who, String msg ) {
		
	}
}