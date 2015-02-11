package com.jiexx.comm;

public abstract interface Message {
	public void onRequest( Dispatcher d ) ;
	public void onResponse( Data d ) ;
	public void onFailed( String info ) ;
}