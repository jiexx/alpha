package com.jiexx.cmd;

import org.json.JSONObject;
import org.json.JSONTokener;

import com.jiexx.comm.Data;
import com.jiexx.comm.Utils;

public class Command {
	public final String toStr() throws Exception  {
		Data d = new Data(this);
		return Utils.encodeDES( d.serialize() );
	}
	public final static class Ack {
		public final static Data fromStr( String json ) throws Exception {
			String str = Utils.decodeDES(json.getBytes());
			JSONTokener parser = new JSONTokener( str );
			JSONObject obj = (JSONObject) parser.nextValue();
			if( obj != null ) {
				String cmd = obj.getString("name");
				String val = obj.getString("value");
				if( cmd != null && !cmd.equals("") && val != null && !val.equals("") ) {
					Class<?> clazz = Class.forName(cmd+"Ack");
					if( clazz != null ) {
						Object o = clazz.newInstance();
						Data d = new Data((Command) o);
						d.unserialize(val);
						return d;
					}
				}
			}
			return null;
		}
	}
}