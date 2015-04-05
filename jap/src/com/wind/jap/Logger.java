package com.wind.jap;

import javax.annotation.processing.Messager;
import javax.annotation.processing.ProcessingEnvironment;
import javax.tools.Diagnostic.Kind;



public class Logger {
	private static Messager messager = null;
	public static void init(ProcessingEnvironment env) {
		if( messager == null )
			messager = env.getMessager();
	}
	public static void w(String log) {
		messager.printMessage(Kind.WARNING, log);
	}
	public static void e(String log) {
		messager.printMessage(Kind.ERROR, log);
	}
}