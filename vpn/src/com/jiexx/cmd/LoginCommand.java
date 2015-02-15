package com.jiexx.cmd;

import com.jiexx.cmd.Command.CommandName;
import com.jiexx.comm.Data;

public class LoginCommand extends Command {
	public String username; 
	
	protected class LoginCommandAck extends Command {
		public Integer result;
		public String key;
		public LoginCommandAck() {
			super.name = CommandName.APPLYACK;
		}
	}
	
	public LoginCommand() {
		super.name = CommandName.LOGINCOMMAND;
	}
}