package com.jiexx.comm;

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

import com.jiexx.cmd.Command;



public class Data {

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
	public String serialize() {
		try {
			JSONStringer js = new JSONStringer();
			serialize( js, mShadow );
			return js.toString();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	public Data() {
		mShadow = null;
	}
	public Data( Command t ) {
		mShadow = t;
	}
	private Command mShadow;
	private static boolean isConstMatched( Class<?> t ) {
		if( Integer.class.isAssignableFrom(t) 
		||  String.class.isAssignableFrom(t)
		||  Float.class.isAssignableFrom(t) ){
			return true;
		}
		return false;
	}
	private final static boolean isObjectMatched( Class<?> t ) {
		if( t.isMemberClass() || t.isLocalClass() ) {
			return true;
		}
		return false;
	}
	private final static boolean isArrayMatched( Class<?> t ) {
		if( t.isArray() ) {
			return true;
		}
		return false;
	}
	private static boolean isConstMatched( Class<?> t, Object o ) {
		if( o instanceof Integer && Integer.class.isAssignableFrom(t) 
		||  o instanceof String && String.class.isAssignableFrom(t)
		||  o instanceof Float && Float.class.isAssignableFrom(t) ){
			return true;
		}
		return false;
	}
	private final static boolean isObjectMatched( Class<?> t, Object o ) {
		if( o instanceof JSONObject && ( t.isMemberClass() || t.isLocalClass() ) ) {
			return true;
		}
		return false;
	}
	private final static boolean isArrayMatched( Class<?> t, Object o ) {
		if( o instanceof JSONArray && t.isArray() ) {
			return true;
		}
		return false;
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
	public Object unserialize( Class<?> clazz, JSONObject jobj ) {
		return extractComFields( clazz, jobj );		
	}
	public void unserialize( String json ) {
		try {
			JSONTokener parser = new JSONTokener( json );
			mShadow = (Command) unserialize( mShadow.getClass(), (JSONObject)parser.nextValue() );  
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public Command get() {
		return mShadow;
	}
}