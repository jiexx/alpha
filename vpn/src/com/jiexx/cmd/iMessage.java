package com.jiexx.cmd;

import com.jiexx.cmd.Command.CommandName;
import com.jiexx.comm.Data;

public class iMessage extends Command {
	public String content; 
	
	public iMessage() {
		super.name = CommandName.IMESSAGE;
	}
}