package com.wind.jap;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import com.sun.codemodel.ClassType;
import com.sun.codemodel.JClass;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JFieldVar;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JSwitch;
import com.sun.codemodel.writer.PrologCodeWriter;
import com.wind.ui.ACTIVITY;
import com.wind.ui.POST;
import com.wind.ui.RECV;
import com.wind.ui.UITHREAD;

public class ClazzModel {
	/**
	 * Global var.
	 */
	private JDefinedClass dc = null;
	private JCodeModel cm = null;
	private String pkg;
	private JClass cls;
	private JSwitch  hdl_swc = null;
	private JFieldVar hdl = null;
	private JFieldVar delegator = null;
	public ClazzModel(String packageName) {
		pkg = packageName;
		generators = new HashMap<String, Generator>();
		generators.put( "ACTIVITY", new ACTIVITYGenerator() );
		generators.put( "POST", 	new POSTGenerator() );
		generators.put( "RECV", 	new RECVGenerator() );
		generators.put( "UITHREAD", new UITHREADGenerator() );
	}
	public static Set<String> getSupportedAnnotationTypes() {
		Set<String> annotataions = new LinkedHashSet<String>();
		annotataions.add(ACTIVITY.class.getCanonicalName());
		annotataions.add(POST.class.getCanonicalName());
		annotataions.add(RECV.class.getCanonicalName());
		annotataions.add(UITHREAD.class.getCanonicalName());
		return annotataions;
	}
	public void start(String clazzName) {
		cm = new JCodeModel();
		cls = cm.ref(clazzName);
		cm._package("android.os.Message");
		cm._package("android.os.Bundle");
		Logger.w("ClazzModel start: " + cm._package("android.os.Message").getPackage().name() +" " + cm._package("android.os.Bundle").getPackage().name() );
	}
	public JCodeModel self() {
		if( cm == null )
			cm = new JCodeModel();
		return cm;
	}
	public JDefinedClass clazz() throws JClassAlreadyExistsException {
		if( dc == null ){
			dc = cm._class(JMod.PUBLIC, pkg+".JAP"+cls.fullName(), ClassType.CLASS);
			dc._extends(cls);
		}
		return dc;
	}
	public int incCountOfPOST() {
		return 0;
	}
	public String getNameOfRECVByPOST( String name ) {
		return "";
	}
	public JFieldVar HANDLERVAR() throws ClassNotFoundException{
		if( hdl == null ) {
			hdl = dc.field(JMod.PRIVATE + JMod.FINAL, cm.parseType("android.os.Handler"), "evtHandler", JExpr._new(cm.ref("uiHandler")));
			delegator = dc.field(JMod.PRIVATE + JMod.FINAL, cls, "delegator", JExpr._this());
		}
		return hdl;
	}
	public JFieldVar DELEGATORVAR() throws ClassNotFoundException{
		if( delegator == null ) {
			hdl = dc.field(JMod.PRIVATE + JMod.FINAL, cm.parseType("android.os.Handler"), "evtHandler", JExpr._new(cm.ref("uiHandler")));
			delegator = dc.field(JMod.PRIVATE + JMod.FINAL, cls, "delegator", JExpr._this());
		}
		return delegator;
	}
	/**
	 * only one handler
	 * @return
	 */
	public JSwitch  HANDLERSWC(){
		try {
			if( hdl_swc == null ) {
				JDefinedClass sc = dc._class(JMod.PRIVATE, "uiHandler", ClassType.CLASS)._extends(cm.parseType("android.os.Handler").boxify());
				
				JMethod handleMessage = sc.method(JMod.PUBLIC, cm.parseType("void"), "handleMessage");
				handleMessage.param(cm.parseType("android.os.Message"), "msg");
				hdl_swc = handleMessage.body()._switch(JExpr.ref("msg").ref("what"));
				
				Logger.w("				ClazzModel HANDLERSWC: "+cm.parseType("android.os.Handler").boxify().toString());
			}
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			Logger.e(e.getMessage());
		} catch (JClassAlreadyExistsException e) {
			// TODO Auto-generated catch block
			Logger.e(e.getMessage());
		} catch (Exception e) {
			StringWriter sw = new StringWriter();
	        PrintWriter pw = new PrintWriter(sw, true);
	        e.printStackTrace(pw);
	        pw.flush();
	        sw.flush();
		}
		return hdl_swc;
	}
	private Map<String, Generator> generators = null;
	public boolean checkIn( String annotationName, Argument args ) {
		Generator g = generators.get(annotationName);
		Logger.w("ClazzModel checkIn:" + annotationName +" " +generators.size());
		if( g != null )
			return g.checkIn(args);
		return false;
	}
	public void generate() {
		Logger.w("ClazzModel generate:"+generators.size());
		for( Entry<String, Generator> entry : generators.entrySet() ) {
			Logger.w("ClazzModel generate: key:"+entry.getKey()+" value:"+entry.getValue().getClass().toString());
			entry.getValue().generate(this);
		}
	}
	public void build(PrologCodeWriter proglogCodeWriter, SourceCodeWriter sourceCodeWriter) {
		// TODO Auto-generated method stub
		try {
			if( cm != null )
				cm.build(proglogCodeWriter, sourceCodeWriter);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Logger.e(e.getMessage());
		}
	}
	
}