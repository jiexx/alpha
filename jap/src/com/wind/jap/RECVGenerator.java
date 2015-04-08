package com.wind.jap;


import java.util.LinkedList;
import java.util.List;

import com.sun.codemodel.JCase;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JExpression;
import com.sun.codemodel.JSwitch;

import com.wind.ui.CommunicationData;


public class RECVGenerator implements Generator {
	List<Argument> arguments = null;
	private final void METHOD_UITHREAD_SWITCH_CASE( JCodeModel codeMode, JDefinedClass defClazz, int index, JSwitch swc ) throws ClassNotFoundException{
		JCase c = swc._case(JExpr.ref(POSTGenerator.STATIC_EVNET_ID_PRIFIX+arguments.get(index).nameOfAnnotation().toUpperCase()));
		JExpression data = JExpr.ref("msg").invoke("getData");
		c.body().invoke(JExpr._super(), arguments.get(index).nameOfTarget()).arg(JExpr.cast(codeMode.parseType("CommunicationData"), data));
		c.body()._break();
	}
	@Override
	public void generate(ClazzModel cm) {
		// TODO Auto-generated method stub
		try {		
			for( int i = 0 ; arguments != null &&  i < arguments.size() ; i ++ ){
				METHOD_UITHREAD_SWITCH_CASE( cm.self(), cm.clazz(), i, cm.HANDLERSWC() );
			}
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			Logger.e(e.getMessage());
		} catch (JClassAlreadyExistsException e) {
			// TODO Auto-generated catch block
			Logger.e(e.getMessage());
		} 
	}
	@Override
	public boolean checkIn(Argument args) {
		// TODO Auto-generated method stub
		Logger.w("				POSTGenerator checkIn: countOfAnnotationParams: "+args.countOfAnnotationParams()+" countOfTargetParams "+args.countOfTargetParams()+" clazzOfTargetParams(0) "+args.clazzStringOfTargetParams(0));
		if( args.countOfAnnotationParams() == 1 
				&& args.valueOfAnnotationParams(0).getClass().equals(String.class) && args.countOfTargetParams() == 1 && args.clazzStringOfTargetParams(0).contains("CommunicationData") ){
			if( arguments == null )
				arguments = new LinkedList<Argument>();
			arguments.add(args);
			return true;
		}
		return false;
	}
}