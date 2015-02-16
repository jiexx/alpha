package com.jiexx.serv;

public class LoginCommand{
	public String username; 
	
	protected class LoginCommandAck{
		public Integer result;
		public String key;
	}
}