package com.jiexx.comm;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;

import javax.crypto.NoSuchPaddingException;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.util.EntityUtils;

public class Dispatcher {
	DefaultHttpClient mHttpClient;
	MessageQueue mSndQueue;
	MessageQueue mRcvQueue;
	static Dispatcher mThis;
	
	public Dispatcher() {
		HttpParams httpParams = new BasicHttpParams();  
        HttpConnectionParams.setConnectionTimeout(httpParams, 20000);  
        HttpConnectionParams.setSoTimeout(httpParams, 20000);  
        HttpConnectionParams.setSocketBufferSize(httpParams, 8192);  
  
        mHttpClient = new DefaultHttpClient(httpParams); 
        
        try {
			Utils.initDES("123");
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        mSndQueue = new MessageQueue( this );
        mRcvQueue = new MessageQueue( this );
        mThis = this;
	}
	
	public static Dispatcher self() {
		return mThis;
	}
	
	public void send( HttpMessage msg ){
		HttpPost httpPost = new HttpPost( Profile.HOST );
		List<NameValuePair> params = new ArrayList<NameValuePair>();  
		try {
			BasicNameValuePair bnvp = new BasicNameValuePair("args", msg.toStr());
			params.add(bnvp);
			httpPost.setEntity(new UrlEncodedFormEntity(params, "UTF-8"));
			HttpResponse httpResponse = mHttpClient.execute(httpPost);
			int code = httpResponse.getStatusLine().getStatusCode();  
			if( code > 199 && code < 400 ) {
				String result = EntityUtils.toString(httpResponse.getEntity());
				msg.onResponse( new Data(result) );
			}else {
				msg.onFailed( Log.Error.ERR_NETWORK );
			}
			mSndQueue.dequeue();
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ClientProtocolException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
	
	public void send( ImMessage msg ){
		msg.onResponse( msg.getData() );
	}
	
	public void receive( Message msg ) throws Exception {
		mRcvQueue.enqueue(msg);
	}
}