package com.wind.jap;


public abstract interface Generator {
	public abstract void generate(ClazzModel cm);
	public abstract boolean checkIn(Argument args);
}