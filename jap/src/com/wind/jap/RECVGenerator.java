package com.wind.jap;


import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.LinkedList;
import java.util.List;

import com.sun.codemodel.JCase;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JExpression;
import com.sun.codemodel.JFieldRef;
import com.sun.codemodel.JFieldVar;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JSwitch;



public class RECVGenerator implements Generator {
	List<Argument> arguments = null;
	final private JFieldRef EVT_ID_ref( int i ) {
		String simpleName = arguments.get(i).valueOfAnnotationParams(0).toString();
		return JExpr.ref(POSTGenerator.STATIC_EVNET_ID_PRIFIX+simpleName.toUpperCase());
	}
	private final void METHOD_UITHREAD_SWITCH_CASE( JCodeModel codeMode, JDefinedClass defClazz, int index, JSwitch swc, JFieldVar delegator ) throws ClassNotFoundException{
		Logger.w("				RECVGenerator generate: "+EVT_ID_ref(index));
		JCase c = swc._case(EVT_ID_ref(index));
		JExpression data = JExpr.ref("msg").invoke("getData");
		JInvocation cd = JExpr._new(codeMode.parseType("com.wind.ui.CommunicationData"));
		cd.arg(data);
		c.body().invoke(delegator, arguments.get(index).nameOfTarget()).arg(cd);
		c.body()._break();
	}
	@Override
	public void generate(ClazzModel cm) {
		// TODO Auto-generated method stub
		try {
			Logger.w("				RECVGenerator generate: "+arguments.size());
			for( int i = 0 ; arguments != null &&  i < arguments.size() ; i ++ ){
				METHOD_UITHREAD_SWITCH_CASE( cm.self(), cm.clazz(), i, cm.HANDLERSWC(), cm.DELEGATORVAR() );
			}
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			Logger.e(e.getMessage());
		} catch (JClassAlreadyExistsException e) {
			// TODO Auto-generated catch block
			Logger.e(e.getMessage());
		}  catch (Exception e) {
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
		Logger.w("				RECVGenerator checkIn: countOfAnnotationParams: "+args.countOfAnnotationParams()+" countOfTargetParams "+args.countOfTargetParams()+" clazzOfTargetParams(0) "+args.clazzStringOfTargetParams(0));
		if( args.countOfAnnotationParams() == 1 
				&& args.valueOfAnnotationParams(0).getClass().equals(String.class) && args.countOfTargetParams() == 1 && args.clazzStringOfTargetParams(0).contains("CommunicationData") ){
			Logger.w("RECVGenerator generate: ok");
			if( arguments == null )
				arguments = new LinkedList<Argument>();
			arguments.add(args);
			return true;
		}
		return false;
	}
}