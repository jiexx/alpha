package com.wind.ui;

import com.wind.customizedata.CustomizedProxy;

public class CommunicationProxy {
	private CustomizedProxy cp = new CustomizedProxy();
	public void onReceive( CommunicationData cm ) {
		cp.onReceive(cm);
	}
	public void post( CommunicationData cm ){
		cp.post(cm);
	};
	//CUSTOMIZED LISTENER TO BE IMPLEMENTED. RAW DATA SHOULD BE PARSED ON LISTENER.
}