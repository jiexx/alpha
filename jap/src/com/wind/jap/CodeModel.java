package com.wind.jap;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.lang.model.element.AnnotationMirror;
import javax.lang.model.element.Element;
import javax.lang.model.element.TypeElement;

import com.sun.codemodel.JPackage;
import com.sun.codemodel.writer.PrologCodeWriter;


public class CodeModel {
	/**
	 * Global var.
	 */
	private Map<String, ClazzModel> clazzModels = null;
	public final static String toSimpleName(String fullName) {
		int suffixPosition = fullName.lastIndexOf('.')+1;
		if( suffixPosition > 0 )
			return fullName.substring(suffixPosition);
		return null;
	}
	public CodeModel() {
		clazzModels = new HashMap<String, ClazzModel>();
	}
	public boolean checkIn(String packageName, String topLevelClazzName, TypeElement annotaion, Element target) {
		if( !clazzModels.containsKey(topLevelClazzName) ){
			clazzModels.put(topLevelClazzName, new ClazzModel(packageName));
		}
		ClazzModel cm = clazzModels.get(topLevelClazzName);
		Logger.w("CodeModel checkIn: Package: "+packageName+" topLevelClazz " + topLevelClazzName + " target " +target.getSimpleName() );
		
		AnnotationMirror mirror = null;
		for( AnnotationMirror am : target.getAnnotationMirrors() ) {
			if( am.getAnnotationType().equals(annotaion.asType()) ){
				mirror = am;
				break;
			}
		}
		if( mirror != null && cm != null ){
			Argument arg = new Argument(target, mirror);
			Logger.w("CodeModel checkIn:"+toSimpleName(mirror.getAnnotationType().toString()));
			return cm.checkIn(toSimpleName(mirror.getAnnotationType().toString()), arg);
		}
		return false;
	}
	public String getPackageName( Class<?> clazz ) {
		return clazz.getClass().getPackage().getName();
	}
	public void generate(PrologCodeWriter proglogCodeWriter, SourceCodeWriter sourceCodeWriter) {
		Logger.w("CodeModel generate: clazzModels.size(): "+clazzModels.size());
		for( Entry<String, ClazzModel> entry : clazzModels.entrySet() ) {
			Logger.w("CodeModel generate: entry.getKey(): "+entry.getKey());
			entry.getValue().start(entry.getKey());
			entry.getValue().generate();
			entry.getValue().build(proglogCodeWriter, sourceCodeWriter);
		}
	}
	
}