package com.wind.ui;

import com.google.gson.Gson;
import com.wind.customizedata.CustomizedDataManager;

import android.os.Bundle;
import android.os.Parcelable;

public class CommunicationData {
	public CommunicationData( int err ){
		switch( err ) {
		case 0:
			request = null;
			response = null;
			break;
		default:
			request = null;
			response = null;
			break;
		}
	}
	
	/**
	 * for user, i.e
	 * CommunicationData login(){
	 *     LoginRequest lr;
	 *     lr.username = ..;
	 *     return new CommunicationData(lr);
	 * @param <T>
	 * 
	 */
	public <T> CommunicationData( T b ) {
		request = CustomizedDataManager.INSTANCE.createRequest( b.getClass(), b );
		response = null;
	}
	/**
	 * for user, i.e
	 * void onLogin(CommunicationData cd){
	 *     LoginData lr = cd.get();
	 *     setText(lr.username ..;
	 *     ..
	 * }
	 * @param <T>
	 * @param b
	 */
	public <T> T get() {
		return  (T) response.data;
	}
	
	/**
	 * internal class
	 * @return
	 */
	private NameParcel request = null;
	private NameParcel response = null;
	public CommunicationData( CommunicationData cd ){
		request = cd.request;
		response = cd.response;
	}
	public CommunicationData( Class<?> resp, String cd ){
		//TO BE CUSTOMIZED
		Gson gson = new Gson();
		Object obj = gson.fromJson(cd, resp);
		response = CustomizedDataManager.INSTANCE.createResponse( resp, obj );
	}
	public CommunicationData( Bundle b ) {
		request = null;
		response.data = b.getParcelable("ptr");
	}
	public Bundle getResponseData() {
		if( response == null )
			return null;
		Bundle b = new Bundle();
		b.putParcelable("ptr", response.data);
		return b;
	}
	public Parcelable getRequestData() {
		return request.data;
	}
	public Class<?> name() {
		if( request != null)
			return request.name;
		return null;
	}
	
	//OTHER TRANSFORMATIONTO BE CUSTOMIZED. I.E. OBJECT TO THIS. 
}