package com.jiexx.comm;

import java.util.ArrayList;
import java.util.List;

public class RcvQueue extends MessageQueue {
	public RcvQueue(Dispatcher d) {
		super(d);
		// TODO Auto-generated constructor stub
	}

	public void loop() {
		synchronized(mQueue) {
			try {
				if( !mQueue.isEmpty() ) {
					Message msg = mQueue.get(0);
					msg.onResponse( mDispatcher );
				}else {
					mQueue.wait();				
				}
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}