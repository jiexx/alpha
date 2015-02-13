package com.jiexx.cmd;

import com.jiexx.comm.Data;

public class ApplyCommand extends Command {
	public String username; 
	public int expiretime;
	
	protected class ApplyAck {
		public Integer result;
		public String key;
	}
}