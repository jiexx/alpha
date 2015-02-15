package com.jiexx.comm;

import java.util.ArrayList;
import java.util.List;

public class MessageQueue implements Runnable {
	private static final int QUEUE_MAX_SIZE = 20;
	protected List<Message> mQueue = new ArrayList<Message>();
	protected Dispatcher mDispatcher;
	public MessageQueue( Dispatcher d ) {
		mDispatcher = d;
	}
	public Message dequeue() {
		synchronized(mQueue) {
			Message msg = mQueue.get(0);
			mQueue.remove(0);
			return msg;
		}
	}
	public void enqueue( final Message msg ) throws Exception{
		synchronized(mQueue) {
			if( mQueue.size() < QUEUE_MAX_SIZE ) {
				mQueue.add( msg );
				mQueue.notify();
			} else {
				mQueue.notify();
				throw new Exception(Log.Error.ERR_MAX_QUEUE);
			}
		}
	}
	public void loop() {
		synchronized(mQueue) {
			try {
				if( !mQueue.isEmpty() ) {
					Message msg = mQueue.get(0);
					msg.onRequest( mDispatcher );
				}else {
					mQueue.wait();				
				}
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		while( true ) {
			loop();
		}
	}
	
}