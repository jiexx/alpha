package com.jiexx.comm;

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



public class Data<T> {
	public enum Type {
		String, Integer, Array, Data;
		public static Type check( Object o ) {
			if( o instanceof String )
				return Type.String;
			else if( o instanceof Integer )
				return Type.Integer;
			else if( o instanceof List<?> )
				return Type.Array;
			else if( o instanceof Data )
				return Type.Data;
			return null;
		}
		public static void toJSON( JSONStringer json, String name, Value v ) {
			try {
				json.object();
				if( v.type() == Type.String || v.type() == Type.Integer ) {
					json.key(name);
					json.value(v.obj());
				}else if ( v.type() == Type.Data ) {
					toJSON( json, name, v );
				}else if ( v.type() == Type.Array ) {
					List<Value> list = (List<Value>) v.obj();
					for( int i = 0 ; i < list.size() ; i ++ ) {
						toJSON( json, name, list.get(i) );
					}
				}
				json.endObject();
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	};
	private class Value {
		private Type mTyp;
		private Object mObj;
		public Value(Type t, Object o) {
			// TODO Auto-generated constructor stub
			mTyp = t;
			mObj = o;
		}
		public Object obj() {
			return mObj;
		}
		public Type type() {
			return mTyp;
		}
		public void set(Object o) {
			mObj = o;
		}
	}
	private Class<T> mCache;
	public void define( String def ) {
		
	}
	private void construct( String k, Data d, JSONObject jobj ) {
		try {
			Iterator<String> keys = jobj.keys();  
			String key;
			Object obj;
			Value v;
			while(keys.hasNext()){  
				key = keys.next();
				obj = jobj.get(key);
				if( obj instanceof String || obj instanceof Integer ) {
					d.put( key, obj );
				}else if( obj instanceof JSONObject ) {
					v = d.add(Type.Data, k);
					construct( key, (Data)v.obj(), (JSONObject)obj );
				}
			}
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
	public Data( String des ) {
		try {
			JSONTokener json = new JSONTokener( Utils.decodeDES(des.getBytes()) );
			JSONObject jobj = (JSONObject) json.nextValue();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public Data( Class<T> d ) {
		mCache = d;
	}
	
	public Value add( Type t, String name ) {
		return mCache.put(name, new Value(t, null));
	}
	public Type getType( String name ){
		return mCache.get(name).type();
	}
	public void put( String name, Object value ) throws Exception {
		if( Type.check(value) == getType(name) ) {
			mCache.get(name).set(value);
		}else {
			throw new Exception(Log.Error.ERR_DATA_TYPE);
		}
	}
	public String getStr( String name ) {
		return (String) mCache.get(name).obj();
	}
	public Integer getInt( String name ) {
		return (Integer) mCache.get(name).obj();
	}
	public List<Data> getArray( String name ) {
		return (List<Data>) mCache.get(name).obj();
	}
	public Data getData( String name ) {
		return (Data) mCache.get(name).obj();
	}
	public String getBase64Str() throws Exception {
		JSONStringer json = new JSONStringer();
		
		Iterator iter = mCache.entrySet().iterator();
		while (iter.hasNext()) {
			HashMap.Entry entry = (HashMap.Entry) iter.next();
			Type.toJSON(json, (String)entry.getKey(), (Value)entry.getKey() );
		}
		
		return Utils.encodeDES( json.toString() );
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
		if( o instanceof JSONObject && t.isMemberClass() ) {
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
	public <T> T unserialize( Class<T> clazz, JSONObject jobj ) {
		try {
			if ( mCache == null )
				return null;
			//T obj = (T) mCache.newInstance();
			if( List.class.isAssignableFrom(mCache) ) {
				
			}else {
				Iterator<String> key = jobj.keys();
				while(  key.hasNext() ) {
					String name = key.next();
					Object value = jobj.get( name );
					Field fld = mCache.getDeclaredField(name);
					Class<?> typ = fld.getType();
					if( isConstMatched( typ, value ) ) {
						Constructor<?> c = typ.getConstructor(typ);
						Object o = c.newInstance(value);
						fld.set(obj, o);
					}else if( isObjectMatched( typ, value ) ) {
						unserialize( (JSONObject)value );
					}
				}
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	private static void setField(Object obj, Method fieldSetMethod,Field f, JSONObject jo) { 
		
	}
}