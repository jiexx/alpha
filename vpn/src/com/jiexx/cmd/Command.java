package com.jiexx.cmd;

import org.json.JSONObject;
import org.json.JSONTokener;

import com.jiexx.comm.Data;
import com.jiexx.comm.Utils;

public class Command {
	public enum CommandName {
		LOGINCOMMAND,LOGINACK,
		APPLYCOMMAND,APPLYACK,
		IMESSAGE;
		public static CommandName toName( Class<?> clazz ) {
			if( clazz.getName().equals("LoginCommand") ){
				return LOGINCOMMAND;
			}else if ( clazz.getName().equals("LoginCommandAck") ) {
				return LOGINACK;
			}else if ( clazz.getName().equals("ApplyCommand") ) {
				return APPLYCOMMAND;
			}else if ( clazz.getName().equals("ApplyCommandAck") ) {
				return APPLYACK;
			}else if ( clazz.getName().equals("iMessage") ) {
				return IMESSAGE;
			}
			return null;
		}
	}
	public CommandName name;
	public final String toStr() throws Exception  {
		Data d = new Data(this);
		return Utils.encodeDES( d.serialize() );
	}
	public final static class ACK {
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
	public final static class STR {
		public final static Data fromStr( String json ) throws Exception {
			String str = Utils.decodeDES(json.getBytes());
			JSONTokener parser = new JSONTokener( str );
			JSONObject obj = (JSONObject) parser.nextValue();
			if( obj != null ) {
				String cmd = obj.getString("name");
				String val = obj.getString("value");
				if( cmd != null && !cmd.equals("") && val != null && !val.equals("") ) {
					Class<?> clazz = Class.forName(cmd);
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