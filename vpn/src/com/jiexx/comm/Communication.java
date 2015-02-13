package com.jiexx.comm;

import com.jiexx.cmd.Command;

public class Communication {
	private class ComHttpMessage extends HttpMessage {

		public ComHttpMessage(Command d) {
			super(d);
			// TODO Auto-generated constructor stub
		}
		public void onResponse( Data d ){
			onHttpMessage(d);
		}
		
	}
	public void postMessage( Command cmd ) {
		ComHttpMessage chm = new ComHttpMessage(cmd);
		Dispatcher.self().inject(chm);
	}
	public void postImMessage( String who, String msg ) {
		
	}
	public void onImMessage( String who, String msg ) {
		
	}
}