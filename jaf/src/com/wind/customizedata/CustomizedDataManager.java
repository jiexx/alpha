package com.wind.customizedata;

import java.util.HashMap;
import java.util.Map;
import android.os.Parcelable;

import com.wind.ui.CommunicationData;
import com.wind.ui.NameParcel;

public enum CustomizedDataManager {
	INSTANCE;  
	public class Info {
		public int appClass;
		public int cmdId;
		public Class<?> response;
		public Info( int a, int c, Class<?> r ) {
			appClass = a;
			cmdId = c;
			response = r;
		}
	}
	private Map<Class<?>, Info> table = new HashMap<Class<?>, Info>();;
	CustomizedDataManager() {
		table.put(LoginRequest.class, new Info(1005, 1003, LoginData.class) );
	}
	public NameParcel createRequest(Class<?> clazz, Object obj){
		NameParcel cd = null;
		if( table.containsKey(clazz) ) {
			cd = new NameParcel(clazz, (Parcelable)obj);
		}
		return cd;
	}
	public boolean checkResponse(Class<?> clazz) {
		for( Map.Entry<Class<?>, Info> e : table.entrySet() ) {
			if( e.getValue().response.equals(clazz) ) {
				return true;
			}
		}
		return false;
	}
	public NameParcel createResponse(Class<?> clazz, Object obj){
		NameParcel cd = null;
		if( checkResponse( clazz ) ) {
			cd = new NameParcel(clazz, (Parcelable)obj);
		}
		return cd;
	}
	public Info queryClass(CommunicationData cm){
		Class<?> name = cm.name();
		return table.get(name);
	}
	
}
