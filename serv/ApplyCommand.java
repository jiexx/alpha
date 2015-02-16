package com.jiexx.serv;

public class ApplyCommand {
	public String username; 
	public int expiretime;
	
	protected class ApplyCommandAck  {
		public Integer result;
		public String key;
	}
}