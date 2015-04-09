package com.wind.jap;

import javax.lang.model.element.ElementKind;

import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.String;
import java.util.LinkedList;

public class ACTIVITYGenerator implements Generator {
	Argument arguments;
	final private void METHOD_ACTIVITY_ONCREATE( JCodeModel codeMode, JDefinedClass defClazz, int index ) throws ClassNotFoundException{
		JMethod onCreate = defClazz.method(JMod.PUBLIC, void.class, "onCreate");
		onCreate.param(codeMode.parseType("android.os.Bundle"), "savedInstanceState"); 
		onCreate.body().invoke(JExpr._super(), "onCreate").arg(JExpr.ref("savedInstanceState"));
		//onCreate.body().invoke("setContentView").arg(JExpr.ref("test")/*arguments.valueOfAnnotationParams(0).toString()*/);
	}
	@Override
	public void generate(ClazzModel cm) {
		// TODO Auto-generated method stub
		try {
			METHOD_ACTIVITY_ONCREATE( cm.self(), cm.clazz(), 0 );
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
	}
	@Override
	public boolean checkIn(Argument args) {
		// TODO Auto-generated method stub
		if( args.countOfAnnotationParams() == 1 
				&& args.valueOfAnnotationParams(0).getClass().equals(String.class) && args.kindOfTarget() == ElementKind.CLASS ){
			arguments = args;
			return true;
		}
		return false;
	}
}