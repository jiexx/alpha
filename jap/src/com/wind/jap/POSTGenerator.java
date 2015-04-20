package com.wind.jap;


import java.util.LinkedList;
import java.util.List;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.String;

import com.sun.codemodel.ClassType;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JExpression;
import com.sun.codemodel.JFieldRef;
import com.sun.codemodel.JFieldVar;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JType;
import com.sun.codemodel.JVar;
import com.wind.ui.CommunicationData;
import com.wind.ui.CommunicationProxy;


public class POSTGenerator implements Generator {
	List<Argument> arguments = null;
	final public static java.lang.String STATIC_EVNET_ID_PRIFIX = "EVT_";
	final private int EVT_ID( int i ) {
		return 0x1000+i;
	}
	final private JFieldRef EVT_ID_ref( int i ) {
		String simpleName = arguments.get(i).valueOfAnnotationParams(0).toString();
		return JExpr.ref(STATIC_EVNET_ID_PRIFIX+simpleName.toUpperCase());
	}
	final private JFieldVar FIELD_EVNET_ID_DECL( JCodeModel codeMode, JDefinedClass defClazz, int index ){
		String simpleName = arguments.get(index).valueOfAnnotationParams(0).toString();
		try {
			return defClazz.field(JMod.PRIVATE + JMod.FINAL, codeMode.INT, STATIC_EVNET_ID_PRIFIX+simpleName.toUpperCase(), JExpr.lit(EVT_ID(index)));
		}catch(Exception e) {
			return null;
		}
	}
	final private JDefinedClass METHOD_POST_PROXY_DECL( JCodeModel codeMode, JDefinedClass defClazz, int index, JFieldVar handler ) throws ClassNotFoundException, JClassAlreadyExistsException {
		JDefinedClass proxy = defClazz._class(JMod.PRIVATE, 
				arguments.get(index).nameOfTarget()+CommunicationProxy.class.getSimpleName(), ClassType.CLASS)._extends(CommunicationProxy.class);
		JMethod onReceive = proxy.method(JMod.PUBLIC, void.class, "onReceive");
		onReceive.param(CommunicationData.class, "cd");
		JVar msg = onReceive.body().decl(codeMode.parseType("android.os.Message"), "msg");
		onReceive.body().assign(msg, JExpr._new(codeMode.parseType("android.os.Message")));
		onReceive.body().assign(msg.ref("what"), EVT_ID_ref(index));
		onReceive.body().invoke(msg, "setData").arg(JExpr.ref("cd").invoke("getResponseData"));
		onReceive.body().invoke(handler, "sendMessage").arg(msg);
		
		defClazz.field(JMod.PRIVATE + JMod.FINAL, proxy, "proxy", JExpr._new(proxy));
		return proxy;
	}
	final private JMethod METHOD_POST_IMPL_DECL( JCodeModel codeMode, JDefinedClass defClazz, int index, JDefinedClass proxy ) throws ClassNotFoundException{
		JType typeTarget = codeMode.parseType("com.wind.ui.CommunicationData");
		JMethod post = defClazz.method(JMod.PUBLIC + JMod.FINAL, typeTarget, arguments.get(index).nameOfTarget());
		Argument arg = arguments.get(index);
		for( int i = 0 ; i < arg.countOfTargetParams() ; i ++ ) {
			post.param(codeMode.parseType(arg.clazzStringOfTargetParams(i)), arg.nameOfTargetParams(i));
		}
		
		JVar url = post.body().decl(typeTarget, "url");
		JInvocation superPost = JExpr._super().invoke(arguments.get(index).nameOfTarget());
		for( int i = 0 ; i < arg.countOfTargetParams() ; i ++ ) {
			superPost.arg(JExpr.ref(arg.nameOfTargetParams(i)));
		}
		post.body().assign(url, superPost);
		
		JFieldRef pxy = JExpr.ref("proxy");

		JInvocation  communicationData = JExpr._new(typeTarget);
		communicationData.arg(url);
		post.body().invoke(pxy, "post").arg(communicationData);
		post.body()._return(url);
		return post;
	}

	@Override
	public void generate(ClazzModel cm) {
		// TODO Auto-generated method stub
		Logger.w("				POSTGenerator generate: arguments.size(): "+arguments.size());
		List<JDefinedClass> proxies = new LinkedList<JDefinedClass>();
		try {	
			for( int i = 0 ; arguments != null && i < arguments.size() ; i ++ ){
				FIELD_EVNET_ID_DECL( cm.self(), cm.clazz(), i );
			}
			for( int i = 0 ; i < arguments.size() ; i ++ ){
				JDefinedClass proxy = METHOD_POST_PROXY_DECL( cm.self(), cm.clazz(), i, cm.HANDLERVAR() );
				proxies.add(proxy);
			}
			for( int i = 0 ; i < arguments.size() ; i ++ ){
				METHOD_POST_IMPL_DECL( cm.self(), cm.clazz(), i, proxies.get(i) );
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
		} finally {
			proxies = null;
		}
	}
	@Override
	public boolean checkIn(Argument args) {
		// TODO Auto-generated method stub
		Logger.w("				POSTGenerator checkIn: countOfAnnotationParams: "+args.countOfAnnotationParams()+" valueOfAnnotationParams(0) "+args.valueOfAnnotationParams(0).getClass().equals(String.class));
		if( args.countOfAnnotationParams() == 1 && args.valueOfAnnotationParams(0).getClass().equals(String.class) ){
			if( arguments == null )
				arguments = new LinkedList<Argument>();
			arguments.add(args);
			return true;
		}
		return false;
	}
}