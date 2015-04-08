package com.wind.jap;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.LinkedList;
import java.util.List;

import javax.lang.model.element.ElementKind;

import com.sun.codemodel.JCase;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JExpression;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JSwitch;
import com.wind.ui.CommunicationData;


public class UITHREADGenerator implements Generator {
	List<Argument> arguments = null;
	private final void METHOD_UITHREAD_SWITCH_DEFAULT( JCodeModel codeMode, JDefinedClass defClazz, int index, JSwitch swc ) throws ClassNotFoundException{
		Logger.w("				UITHREADGenerator 1 "+defClazz);
		Argument arg = arguments.get(index);
		Logger.w("				UITHREADGenerator 2 "+arg.toString());
		JCase c = swc._default();
		JInvocation def = c.body().invoke(arg.nameOfTarget());
		for( int i = 0 ; i < 1 ; i ++ ) {
			JExpression data = JExpr.ref("msg").invoke("getData");
			def.arg(data);
		}
		c.body()._break();
	}
	@Override
	public void generate(ClazzModel cm) {
		// TODO Auto-generated method stub
		try {
			for( int i = 0 ; arguments != null && i < arguments.size() ; i ++ ){
				METHOD_UITHREAD_SWITCH_DEFAULT( cm.self(), cm.clazz(), i, cm.HANDLERSWC() );
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
			Logger.w("				UITHREADGenerator "+sw.toString());
		}
	}
	@Override
	public boolean checkIn(Argument args) {
		// TODO Auto-generated method stub
		if( args.countOfAnnotationParams() == 0 && args.kindOfTarget() == ElementKind.METHOD && args.countOfTargetParams() == 1 && args.clazzStringOfTargetParams(0).contains("CommunicationData")){
			if( arguments == null )
				arguments = new LinkedList<Argument>();
			arguments.add(args);
			return true;
		}
		return false;
	}
}