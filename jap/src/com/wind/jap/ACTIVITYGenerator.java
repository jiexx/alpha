package com.wind.jap;

import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;


public class ACTIVITYGenerator implements Generator {
	private String id;
	public void setLayout( String li ){
		id = li;
	}
	@Override
	public void generate(CodeModel cm) {
		// TODO Auto-generated method stub
		try {
			JDefinedClass dc = cm.clazz();
			JMethod onCreate = dc.method(JMod.PUBLIC, cm.self().parseType("void"), "onCreate");
			onCreate.param(cm.self().parseType("Bundle"), "savedInstanceState"); 
			onCreate.body().invoke(JExpr._super(), "onCreate").arg(JExpr.ref("savedInstanceState"));
			onCreate.body().invoke("setContentView").arg(id);
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
}