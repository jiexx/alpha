package com.jiexx.comm;

import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;


import android.util.Base64;


public class Utils {
	private static SecretKeySpec key; 
	private static byte[] iv = {1,2,3,4,5,6,7,8}; 
	private static IvParameterSpec ips = new IvParameterSpec(iv);
	private static Cipher cipher = null;
	
	static void initDES( String k ) throws Exception {
		key = new SecretKeySpec(k.getBytes(), "DES"); 
		if( cipher == null )
			cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
	}
	
	static String encodeDES(String d) throws Exception {
		cipher.init(Cipher.ENCRYPT_MODE, key, ips); 
		return Base64.encodeToString(cipher.doFinal(d.getBytes()), Base64.NO_WRAP);
	}
	
	static String decodeDES(byte[] d) throws Exception {
		cipher.init(Cipher.DECRYPT_MODE, key, ips);
		return new String(cipher.doFinal(Base64.decode(d, Base64.NO_WRAP)));
	}
}