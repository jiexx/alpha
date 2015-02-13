package com.jiexx.cmd;

import com.jiexx.comm.Data;

public class LoginCommand extends Command {
	public String username; 
	
	protected class LoginCommandAck {
		public Integer result;
		public String key;
	}
}