package com.wind.jap;

import javax.lang.model.element.ElementKind;

import android.view.View.OnClickListener;
import android.view.View;

import com.sun.codemodel.JClass;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JFieldRef;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JPackage;
import com.sun.codemodel.JType;
import com.sun.codemodel.JVar;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.String;
import java.util.LinkedList;
import java.util.List;

public class BUTTONGenerator implements Generator {
	View.OnClickListener vo = new View.OnClickListener(){

		@Override
		public void onClick(View arg0) {
			// TODO Auto-generated method stub
			
		}
		
	};
	List<Argument> arguments = null;
	final private void METHOD_BUTTON_CLICK( JCodeModel codeMode, JDefinedClass defClazz, int index, JMethod oncreate ) throws ClassNotFoundException, JClassAlreadyExistsException{
		
		String target = arguments.get(index).nameOfTarget();
		target = target.replaceFirst(target.substring(0, 1),target.substring(0, 1).toUpperCase()) ;
		oncreate.body().decl(codeMode.parseType("android.view.View"), "vw"+target, 
				JExpr.invoke("findViewById").arg(JExpr.lit((Integer)arguments.get(index).valueOfAnnotationParams(0))));
		JDefinedClass anonymous = codeMode.anonymousClass(View.OnClickListener.class);
		
		JMethod on = anonymous.method(JMod.PUBLIC, void.class, "onClick");
		on.param(codeMode.ref(View.class), "v");
		on.body().invoke(arguments.get(index).nameOfTarget());
		
		JFieldRef btn = JExpr.ref("vw"+target);
		oncreate.body().invoke(btn, "setOnClickListener").arg(JExpr._new(anonymous));
	}
	@Override
	public void generate(ClazzModel cm) {
		// TODO Auto-generated method stub
		try {
			Logger.e("				BUTTONGenerator---------------");
			for( int i = 0 ; arguments != null &&  i < arguments.size() ; i ++ ){
				METHOD_BUTTON_CLICK( cm.self(), cm.clazz(), i, cm.ONCREATE() );
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
	        Logger.w(sw.getBuffer().toString());
		}
	}
	@Override
	public boolean checkIn(Argument args) {
		// TODO Auto-generated method stub
		Logger.w("				BUTTONGenerator checkIn: countOfAnnotationParams: "+args.countOfAnnotationParams()+" countOfTargetParams "+args.countOfTargetParams()+ "  args.valueOfAnnotationParams(0).getClass()" +args.valueOfAnnotationParams(0).getClass());
		if( args.countOfAnnotationParams() == 1 
				&& args.valueOfAnnotationParams(0).getClass().equals(Integer.class) && args.kindOfTarget() == ElementKind.METHOD && args.countOfTargetParams() == 0 ){
			if( arguments == null )
				arguments = new LinkedList<Argument>();
			arguments.add(args);
			return true;
		}
		return false;
	}
}