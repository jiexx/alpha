package com.wind.customizedata;

import android.util.Log;

import com.google.gson.Gson;
import com.iwindnet.client.ISkyMsgDelegate;
import com.iwindnet.client.SkyAgentWrapper;
import com.iwindnet.launch.base.BaseJsonRequest;
import com.iwindnet.launch.base.BaseJsonRequest.MessageDelegate;
import com.iwindnet.launch.base.BaseJsonRequest.OnResponseListener;
import com.iwindnet.message.PacketStream;
import com.iwindnet.message.SkyMessage;
import com.wind.customizedata.CustomizedDataManager.Info;
import com.wind.ui.CommunicationData;
import com.wind.ui.CommunicationProxy;



public class CustomizedJSONImpl implements CustomizedImpl{
	private CustomizedProxy proxy;
	public void post( CommunicationData cm, CustomizedProxy pxy ){
		proxy = pxy;
		
		Info info = CustomizedDataManager.INSTANCE.queryClass(cm);
		
		listener l = new listener(info.response);
		
		BaseJsonRequest brj = new BaseJsonRequest(info.appClass, info.cmdId, cm.getRequestData() );
		
		Launcher.INSTANCE.LOG("POST", cm.getRequestData().getClass().getSimpleName()+"  appClass:"+info.appClass+"  cmdId:"+info.cmdId);
		SkyAgentWrapper.getInstance().postMessage(brj,l);
	};
	
	public class listener extends SkyMessage implements ISkyMsgDelegate{
		private String result;
		private Class<?> clazz;
		public listener(Class<?> cls) {
			clazz = cls;
		}
		@Override
		public void onSkyMessageReceive(SkyMessage sky) {
			if(sky != null){
				if(this.deserialize(sky.getSerializedData(), sky.getLength())){
					if(result != null){
						Launcher.INSTANCE.LOG("RECV "+clazz.getSimpleName(), result);
						proxy.onReceive( new CommunicationData(clazz, result) );
					}else{
						Launcher.INSTANCE.LOG("RECV "+clazz.getSimpleName(), "nothing");
						proxy.onReceive( new CommunicationData(0));
					}
				}
			}
		}
		
		@Override
		public boolean deserializeBody(byte[] buf, int offset, int size) {
			PacketStream stream = new PacketStream(buf, offset, size, false);
			try {
				result = stream.readString(stream.readShort());
			    return true;
			} catch (Exception e) {
				// #ifdef DEBUG
				e.printStackTrace();
				// #endif
			} finally {
				stream.close();
			}
			return false;
		}
	}
	
}
