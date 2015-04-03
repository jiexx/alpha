package com.wind.jap;

import com.sun.codemodel.ClassType;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JFieldVar;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JSwitch;


public class CodeModel {
	private JDefinedClass dc = null;
	private JCodeModel cm = null;
	private String pkg;
	private ClassType cls;
	private JSwitch  hdl_swc = null;
	private JFieldVar hdl= null;
	public CodeModel(String p, ClassType t) {
		cm = new JCodeModel();
		pkg = p;
		cls = t;
	}
	public JCodeModel self() {
		if( cm == null )
			cm = new JCodeModel();
		return cm;
	}
	public JDefinedClass clazz() {
		try {
			if( dc == null )
				dc = cm._class(JMod.PUBLIC, pkg+cls.getClass().getName(), cls);
		} catch (JClassAlreadyExistsException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return dc;
	}
	public void add(Generator g) {
		
	}
	public JCodeModel generate(){
		return cm; 
	}
	public int countOfPOST() {
		return 0;
	}
	public String nameOfPOST() {
		return "";
	}
	public JFieldVar getHANDLERVAR(){
		try {
			if( hdl == null )
				hdl = dc.field(JMod.PRIVATE + JMod.FINAL, cm.parseType("Handler"), "evtHandler", JExpr._new(cm.parseType("uiHandler")));
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return hdl;
	}
	public JSwitch  getHANDLERSWC(){
		try {
			if( hdl_swc == null ) {
				JDefinedClass sc = dc._class(JMod.PRIVATE, "uiHandler", ClassType.CLASS)._extends(cm.parseType("Handler").getClass());
				JMethod handleMessage = sc.method(JMod.PUBLIC, cm.parseType("void"), "handleMessage");
				handleMessage.param(cm.parseType("Message"), "msg");
				hdl_swc = handleMessage.body()._switch(JExpr.ref("msg").ref("what"));
			}
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (JClassAlreadyExistsException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return hdl_swc;
	}
}