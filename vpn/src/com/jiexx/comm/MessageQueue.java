package com.jiexx.comm;

import java.util.ArrayList;
import java.util.List;

public class MessageQueue implements Runnable {
	private class Node {
		public long CreateTime;
		public long ReceiveTime;
		final public Message MSG;
		public Node( final Message msg ) {
			CreateTime = System.currentTimeMillis();
			MSG = msg;
		}
	}
	private List<Node> mSndQueue = new ArrayList<Node>();
	private List<Node> mRcvQueue = new ArrayList<Node>();
	private Dispatcher mDispatcher = new Dispatcher();
	public synchronized void enqueue( final Message msg ){
		mSndQueue.add( new Node(msg) );
	}
	public synchronized Message dequeue(){
		Node n = mSndQueue.get(0);
		mSndQueue.remove(0);
		mRcvQueue.add(n);
		return n.MSG;
	}
	public synchronized void loop() {
		dequeue().onRequest(mDispatcher);
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		for( Node n : mSndQueue ) {
			loop();
		}
	}
	
}