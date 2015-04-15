package com.wind.ui;

import android.os.Parcelable;


/**
 * internal class
 * @author jluo
 *
 */
public class NameParcel {
	public Class<?> name;
	public Parcelable data;
	public NameParcel(Class<?> clazz, Parcelable d){
		name = clazz;
		data = d;
	}
	public NameParcel(NameParcel np){
		name = np.name;
		data = np.data;
	}
}