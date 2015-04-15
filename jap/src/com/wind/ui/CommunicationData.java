package com.wind.ui;

import java.io.Serializable;

import android.os.Bundle;

public abstract class CommunicationData {
	public CommunicationData( CommunicationData cd ){
		//TO BE CUSTOMIZED
	}
	public abstract Serializable getSerializable();
	
	public Bundle toBundle() {
		Bundle b = new Bundle();
		b.putSerializable("Data", getSerializable());
		return b;
	}
	//OTHER TRANSFORMATIONTO BE CUSTOMIZED. I.E. OBJECT TO THIS. 
}