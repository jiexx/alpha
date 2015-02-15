package com.jiexx.comm;

public abstract interface Message {
	public void onRequest( Dispatcher d ) ;
	public void onResponse( Dispatcher d ) ;
	public void onData( Data d ) ;
	public void onFailed( String info ) ;
}