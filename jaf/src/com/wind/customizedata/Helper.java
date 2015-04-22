package com.wind.customizedata;

import java.io.IOException;
import java.lang.reflect.Field;
import java.lang.reflect.Type;
import java.util.HashMap;
import java.util.Map;

import android.os.Parcelable;

import com.iwindnet.message.PacketStream;

public class Helper {
	PacketStream stream = new PacketStream();
	public interface Handler {
		public  void accept(Object obj)  throws IOException;
	}
	public class IntegerHandler implements Handler {
		public  void accept(Object obj)  throws IOException{  
	        stream.writeInt((Integer) obj);
	    } 
	}
	public class LongHandler implements Handler {
		public  void accept(Object obj)  throws IOException{  
			stream.writeLong((Long) obj);
	    } 
	}
	public class ByteArrayHandler implements Handler {
		public  void accept(Object obj)  throws IOException{  
			stream.write((byte[]) obj);
	    } 
	}
	private Map<Type, Handler> map = new HashMap<Type, Handler>();
	public Helper() {
		map.put(Integer.class,			new IntegerHandler());
		map.put(Long.class,				new LongHandler());
		map.put(Byte[].class,			new ByteArrayHandler());
	}
	public byte[] get( Parcelable par ) {
		
		byte[] b;
		try {
			Class<?> clazz = par.getClass();
			for( Field f : clazz.getDeclaredFields() ) {
				f.setAccessible(true);
				map.get(f.getGenericType()).accept( f.get(par) );
			}
		} catch (IOException e) {

		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} finally {
			b = stream.getByte().clone();
			stream.close();
		}
		return b;
	}
}
