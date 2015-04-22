package com.wind.customizedata;

import java.io.File;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

import android.util.Log;

import com.google.gson.Gson;
import com.iwindnet.client.ISkyMsgDelegate;
import com.iwindnet.client.SkyAgentWrapper;
import com.iwindnet.launch.base.BaseJsonRequest;
import com.iwindnet.launch.base.BaseJsonRequest.MessageDelegate;
import com.iwindnet.launch.base.BaseJsonRequest.OnResponseListener;
import com.iwindnet.message.MessageHeader;
import com.iwindnet.message.PacketStream;
import com.iwindnet.message.SkyMessage;
import com.iwindnet.message.MessageHeader.SignDealType;
import com.wind.customizedata.CustomizedDataManager.Info;
import com.wind.login.ChangeAvatarRequest;
import com.wind.ui.CommunicationData;
import com.wind.ui.CommunicationProxy;



public class CustomizedBinaryImpl implements CustomizedImpl {
	private CustomizedProxy proxy; 
	public class Binary extends SkyMessage {
		int length;
		String json;
		byte[] buff;
		public Binary( int app, int cmd, byte[] b, String j ) {
			json = j;
			try {
				length = json.getBytes("UTF_8").length +2 + b.length + 4;
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			buff = b;
			getHeader().setSignDealType(SignDealType.interGroup);
			setCommand((app << 20)+cmd);
		}
		public void serialize() {
			int bodySize = length;
			this.mMsgHeader.setMessageLen(MessageHeader.MESSAGE_HEADER_LEN + this.mSkyHeader.MESSAGE_HEADER_LEN + bodySize);
			this.createBuffer(this.mMsgHeader.getMessageLen());
			this.mMsgHeader.serialize(this.mDataBuffer);
			
			this.mSkyHeader.serialize(this.mDataBuffer, MessageHeader.MESSAGE_HEADER_LEN);
			if (bodySize > 0)  {
				PacketStream stream = new PacketStream(mDataBuffer, 
						MessageHeader.MESSAGE_HEADER_LEN + mSkyHeader.MESSAGE_HEADER_LEN, bodySize, true);
				try {
					stream.writeString(json);
					stream.writeInt(length);
					stream.write(buff);
					stream.writeFinish();
				} catch (IOException e) {
				} finally {
					stream.close();
				}
			}		
		}
	}
	public void post( CommunicationData cm, CustomizedProxy pxy ){
		proxy = pxy;
		ChangeAvatarRequest car = (ChangeAvatarRequest)cm.getRequestData();
		Info info = CustomizedDataManager.INSTANCE.queryClass(cm);
	
		listener l = new listener(info.response);
	
		String json = "{uid:\""+car.userId+"\",imgLen: \""+car.imgLen+"\"}";
	
		Binary bin = new Binary( info.appClass, info.cmdId, car.imgcontent, json );
	
		Launcher.INSTANCE.LOG("POST", cm.getRequestData().getClass().getSimpleName()+"  appClass:"+info.appClass+"  cmdId:"+info.cmdId);
		SkyAgentWrapper.getInstance().postMessage(bin,l);
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
