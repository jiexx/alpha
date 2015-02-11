package com.jiexx.comm;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import org.json.JSONException;
import org.json.JSONStringer;



public class Data {
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
	private HashMap<String,Value> mCache = new HashMap<String,Value>();
	public void define( String def ) {
		
	}
	public Data( String base64str ) {
		
	}
	
	public void add( Type t, String name ) {
		mCache.put(name, new Value(t, null));
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
}