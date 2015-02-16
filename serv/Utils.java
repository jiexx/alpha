package com.jiexx.serv;

import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;


import android.util.Base64;


import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONStringer;
import org.json.JSONTokener;


public class Utils {
	private static SecretKeySpec key; 
	private static byte[] iv = {1,2,3,4,5,6,7,8}; 
	private static IvParameterSpec ips = new IvParameterSpec(iv);
	private static Cipher cipher = null;
	
	static void initDES( String k ) throws Exception {
		initKey( k );
		if( cipher == null )
			cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
	}
	
	static void initKey( String k ) throws Exception {
		key = new SecretKeySpec(k.getBytes(), "DES"); 
	}
	
	public static String encodeDES(String d) throws Exception {
		cipher.init(Cipher.ENCRYPT_MODE, key, ips); 
		return Base64.encodeToString(cipher.doFinal(d.getBytes()), Base64.NO_WRAP);
	}
	
	public static String decodeDES(byte[] d) throws Exception {
		cipher.init(Cipher.DECRYPT_MODE, key, ips);
		return new String(cipher.doFinal(Base64.decode(d, Base64.NO_WRAP)));
	}
	private final static Object extractComFields( Class<?> clazz, JSONObject jobj ) {
		try {
			Object obj = clazz.newInstance();
			if ( obj == null )
				return null;
			Iterator<String> key = jobj.keys();
			String name;
			Object value, o;
			Field field;
			Class<?> type;
			Constructor<?> c;
			while(  key.hasNext() ) {
				name = key.next();
				value = jobj.get( name );
				field = clazz.getDeclaredField(name);
				field.setAccessible(true);
				type = field.getType();
				if( isConstMatched( type, value ) ) {
					c = type.getConstructor(type);
					o = c.newInstance(value);
					field.set(obj, o);
				}else if( isObjectMatched( type, value ) ) {
					o = extractComFields( type, (JSONObject)value );
					field.set(obj, o);
				}else if( isArrayMatched( type, value ) ) {
					o = extractArrFields( type, (JSONArray)value );
					field.set(obj, o);
				}
			}
			return obj;
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	private final static Object[] extractArrFields( Class<?> clazz, JSONArray jobj ) {
		try {
			
			Constructor<?>[] ca = clazz.getConstructors();
			if ( ca.length == jobj.length() )  {
				Object[] obj = (Object[]) Array.newInstance(clazz, ca.length);
				for( int i = 0 ; i < ca.length ; i ++ ) {
					Object o = extractComFields( ca.getClass(), jobj.getJSONObject(i) );
					obj[i] = o;
				}
				return obj;
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	public Object unserialize( Class<?> clazz, JSONObject jobj ) {
		return extractComFields( clazz, jobj );		
	}
	private final static void serialize( JSONStringer js, Object obj ) {
		try {
			Class<?> c = obj.getClass();
			Class<?> clazz;
			for( Field field : c.getDeclaredFields() ) {
				clazz = field.getClass();
				if( isConstMatched( clazz ) ) {
					js.key(field.getName());
					js.value(field.get(obj));	
				} else if ( isObjectMatched( c ) ) {
					js.key(field.getName());
					js.object();
					serialize(js,field.get(obj));
					js.endObject();
				} else if ( isArrayMatched( c ) ) {
					js.key(field.getName());
					js.array();
					for( int i = 0; i < Array.getLength(obj) ; i ++ ) {
						serialize( js,Array.get(field.get(obj), i) );
					}
					js.endArray();
				}
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public String serialzie( Object obj ) {
		try {
			JSONStringer js = new JSONStringer();
			serialize( js, obj );
			return js.toString();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public final static Object fromStr( String json ) throws Exception {
		String str = Utils.decodeDES(json.getBytes());
		JSONTokener parser = new JSONTokener( str );
		JSONObject obj = (JSONObject) parser.nextValue();
		if( obj != null ) {
			String cmd = obj.getString("name");
			String val = obj.getString("value");
			if( cmd != null && !cmd.equals("") && val != null && !val.equals("") ) {
				Class<?> clazz = Class.forName(cmd);
				if( clazz != null ) {
					return unserialize( clazz, val);;
				}
			}
		}
		return null;
	}
	public final String toStr( Object o ) throws Exception  {
		return Utils.encodeDES( serialize(o) );
	}
}