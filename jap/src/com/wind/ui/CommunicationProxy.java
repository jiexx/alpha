package com.wind.ui;

public abstract class CommunicationProxy {
	abstract void onReceive( CommunicationData cm );
	void post( CommunicationData cm ){
		//SEND(DATA, LISTENER) TO BE CUSTOMIZED
	};
	//CUSTOMIZED LISTENER TO BE IMPLEMENTED. RAW DATA SHOULD BE PARSED ON LISTENER.
}