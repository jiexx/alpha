package com.wind.customizedata;

import com.wind.customizedata.CustomizedDataManager.Info;
import com.wind.ui.CommunicationData;
import com.wind.ui.CommunicationProxy;



public class CustomizedProxy {
	private CommunicationProxy cp;
	public CustomizedProxy(CommunicationProxy c){
		cp = c;
	}
	public void onReceive( CommunicationData cm ){
		cp.onReceive(cm);
	}
	public void post( CommunicationData cm ){
		Info info = CustomizedDataManager.INSTANCE.queryClass(cm);
		
		CustomizedImpl impl = info.impl;
		
		impl.post(cm, this);
	};
}
