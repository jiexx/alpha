package com.wind.jap;

import com.sun.codemodel.ClassType;
import com.sun.codemodel.JCase;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JFieldVar;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JVar;


public class POSTGenerator implements Generator {
	private String name;
	public void setName( String a ){
		name = a;
	}
	@Override
	public void generate(CodeModel cm) {
		// TODO Auto-generated method stub
		try {
			JDefinedClass dc = cm.clazz();
			JFieldVar id = dc.field(JMod.PRIVATE + JMod.FINAL, cm.self().INT, "EVT_"+name.toUpperCase(), JExpr.lit(cm.countOfPOST()));
			JDefinedClass delegator = dc._class(JMod.PRIVATE, cm.nameOfPOST()+"Delegator", ClassType.CLASS)._extends(cm.self().parseType("TEST").getClass());
			JMethod onReceive = delegator.method(JMod.PUBLIC, cm.self().parseType("void"), "onReceive");
			onReceive.param(cm.self().parseType("char[]"), "buf");
			JVar msg = onReceive.body().decl(cm.self().parseType("Message"), "msg");
			onReceive.body().assign(msg, JExpr._new(cm.self().parseType("Message")));
			onReceive.body().assign(msg.ref("what"), id);
			onReceive.body().assign(msg.ref("data"), JExpr.ref("buf"));
			onReceive.body().invoke(cm.getHANDLERVAR(), "sendMessage").arg(msg);
			
			JCase c = cm.getHANDLERSWC()._case(id);
			
			c.body()._break();
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (JClassAlreadyExistsException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
}