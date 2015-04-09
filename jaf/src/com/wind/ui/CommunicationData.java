package com.wind.ui;

import java.io.Serializable;

import android.os.Bundle;

public class CommunicationData {
	public CommunicationData( String url ){
		//TO BE CUSTOMIZED
	}
	public CommunicationData( Bundle b ){
		//TO BE CUSTOMIZED
	}
	public void setString(String url) {
		
	}
	public class Data implements Serializable {
		private static final long serialVersionUID = 1L;
	}
	public Serializable getSerializable() {
		//TO BE CUSTOMIZED
		return new Data();
	}
	public Bundle getBundle() {
		Bundle b = new Bundle();
		b.putSerializable("Data", getSerializable());
		return b;
	}
	//OTHER TRANSFORMATIONTO BE CUSTOMIZED. I.E. OBJECT TO THIS. 
}