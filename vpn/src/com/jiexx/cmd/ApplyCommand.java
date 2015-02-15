package com.jiexx.cmd;

import com.jiexx.comm.Data;

public class ApplyCommand extends Command {
	public String username; 
	public int expiretime;
	
	protected class ApplyCommandAck extends Command {
		public Integer result;
		public String key;
		public ApplyCommandAck() {
			super.name = CommandName.APPLYACK;
		}
	}
	public ApplyCommand() {
		super.name = CommandName.APPLYCOMMAND;
	}
	
}