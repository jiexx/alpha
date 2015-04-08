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
import com.sun.codemodel.JFieldVar;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JVar;
import com.wind.ui.CommunicationProxy;


public class POSTGenerator implements Generator {
	List<Argument> arguments = null;
	final public static java.lang.String STATIC_EVNET_ID_PRIFIX = "EVT_";
	final private int EVT_ID( int i ) {
		return 0x1000+i;
	}
	final private JFieldVar FIELD_EVNET_ID_DECL( JCodeModel codeMode, JDefinedClass defClazz, int index ){
		String simpleName = arguments.get(index).valueOfAnnotationParams(0).toString();
		Logger.w("				POSTGenerator FIELD_EVNET_ID_DECL "+STATIC_EVNET_ID_PRIFIX+simpleName.toUpperCase()+"  "+index+" "+defClazz.fullName());
		try {
			return defClazz.field(JMod.PRIVATE + JMod.FINAL, codeMode.INT, STATIC_EVNET_ID_PRIFIX+simpleName.toUpperCase()+index, JExpr.lit(EVT_ID(index)));
		}catch(Exception e) {
			Logger.w("				POSTGenerator "+e.getMessage());
			return null;
		}
	}
	final private JDefinedClass METHOD_POST_PROXY_DECL( JCodeModel codeMode, JDefinedClass defClazz, int index, JFieldVar handler ) throws ClassNotFoundException, JClassAlreadyExistsException {
		JDefinedClass proxy = defClazz._class(JMod.PRIVATE, 
				arguments.get(index).nameOfTarget()+CommunicationProxy.class.getSimpleName(), ClassType.CLASS)._extends(CommunicationProxy.class);
		JMethod onReceive = proxy.method(JMod.PUBLIC, void.class, "onReceive");
		onReceive.param(CommunicationProxy.class, "cd");
		JVar msg = onReceive.body().decl(codeMode.parseType("Message"), "msg");
		onReceive.body().assign(msg, JExpr._new(codeMode.parseType("Message")));
		onReceive.body().assign(msg.ref("what"), JExpr.lit(EVT_ID(index)));
		onReceive.body().invoke(msg, "setData").arg(JExpr.ref("cd"));
		onReceive.body().invoke(handler, "sendMessage").arg(msg);
		return proxy;
	}
	final private JMethod METHOD_POST_IMPL_DECL( JCodeModel codeMode, JDefinedClass defClazz, int index, JDefinedClass proxy ) throws ClassNotFoundException{
		JMethod post = defClazz.method(JMod.PRIVATE + JMod.FINAL, void.class, arguments.get(index).nameOfTarget());
		Argument arg = arguments.get(index);
		for( int i = 0 ; i < arg.countOfTargetParams() ; i ++ ) {
			post.param(codeMode.ref(arg.clazzStringOfTargetParams(i)), arg.nameOfTargetParams(i));
		}
		
		JVar url = post.body().decl(codeMode.ref("java.lang.String"), "url");
		
		JInvocation superPost = JExpr._super().invoke(arguments.get(index).nameOfTarget());
		for( int i = 0 ; i < arg.countOfTargetParams() ; i ++ ) {
			superPost.arg(JExpr.ref(arg.nameOfTargetParams(i)));
		}
		url.assign(superPost);
		
		JVar pxy = post.body().decl(proxy, "pxy");
		pxy.assign(JExpr._new(proxy));
		
		post.body().invoke(pxy, "post").arg(JExpr._new(codeMode.parseType("CommunicationData")).cor(url));
		return post;
	}
	public void test() {
		Logger.w("				POSTGenerator test");
	}
	@Override
	public void generate(ClazzModel cm) {
		// TODO Auto-generated method stub
		Logger.w("				POSTGenerator generate: arguments.size(): "+arguments.size());
		List<JDefinedClass> proxies = new LinkedList<JDefinedClass>();
		try {	
			for( int i = 0 ; arguments != null && i < arguments.size() ; i ++ ){
				Logger.w("				FIELD_EVNET_ID_DECL start "+i+" "+arguments.size());
				FIELD_EVNET_ID_DECL( cm.self(), cm.clazz(), i );
				Logger.w("				FIELD_EVNET_ID_DECL end "+i+" "+arguments.size());
			}
			for( int i = 0 ; i < arguments.size() ; i ++ ){
				JDefinedClass proxy = METHOD_POST_PROXY_DECL( cm.self(), cm.clazz(), i, cm.HANDLERVAR() );
				proxies.add(proxy);
				Logger.w("				POSTGenerator generate: proxy: "+proxy.toString());
			}
			for( int i = 0 ; i < arguments.size() ; i ++ ){
				METHOD_POST_IMPL_DECL( cm.self(), cm.clazz(), i, proxies.get(i) );
			}
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			Logger.w("				POSTGenerator "+e.getMessage());
			Logger.e(e.getMessage());
		} catch (JClassAlreadyExistsException e) {
			// TODO Auto-generated catch block
			Logger.w("				POSTGenerator "+e.getMessage());
			Logger.e(e.getMessage());
		} catch (Exception e) {
			StringWriter sw = new StringWriter();
	        PrintWriter pw = new PrintWriter(sw, true);
	        e.printStackTrace(pw);
	        pw.flush();
	        sw.flush();
			Logger.w("				POSTGenerator "+sw.toString());
		} finally {
			proxies = null;
			Logger.w("				POSTGenerator final ");
		}
		Logger.w("				POSTGenerator final 2 ");
	}
	@Override
	public boolean checkIn(Argument args) {
		// TODO Auto-generated method stub
		Logger.w("				POSTGenerator checkIn: countOfAnnotationParams: "+args.countOfAnnotationParams()+" valueOfAnnotationParams(0) "+args.valueOfAnnotationParams(0).getClass().equals(String.class));
		if( args.countOfAnnotationParams() == 1 && args.valueOfAnnotationParams(0).getClass().equals(String.class) ){
			if( arguments == null )
				arguments = new LinkedList<Argument>();
			Logger.w("				POSTGenerator checkIn:"+arguments.size());
			arguments.add(args);
			return true;
		}
		return false;
	}
}