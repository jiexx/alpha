package com.wind.jap;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.lang.model.element.AnnotationMirror;
import javax.lang.model.element.Element;
import javax.lang.model.element.TypeElement;

import com.sun.codemodel.writer.PrologCodeWriter;


public class CodeModel {
	/**
	 * Global var.
	 */
	private Map<String, ClazzModel> clazzModels = null;
	public CodeModel() {
		clazzModels = new HashMap<String, ClazzModel>();
	}
	public boolean checkIn(String topLevelClazzName, TypeElement annotaion, Element target) {
		if( !clazzModels.containsKey(topLevelClazzName) ){
			clazzModels.put(topLevelClazzName, new ClazzModel());
		}
		ClazzModel cm = clazzModels.get(topLevelClazzName);
		Logger.w("CodeModel checkIn: topLevelClazz " + topLevelClazzName + " target " +target.getSimpleName() );
		
		AnnotationMirror mirror = null;
		for( AnnotationMirror am : target.getAnnotationMirrors() ) {
			if( am.getAnnotationType() == annotaion.asType() ){
				mirror = am;
				break;
			}
		}
		if( mirror != null && cm != null ){
			Argument arg = new Argument(target, mirror);
			Logger.w("CodeModel checkIn: annotation " + mirror.getAnnotationType().toString() + " target " +target.getSimpleName() );
			return cm.checkIn(mirror.getAnnotationType().toString(), arg);
		}
		return false;
	}
	public String getPackageName( Class<?> clazz ) {
		return clazz.getClass().getPackage().getName();
	}
	public void generate(PrologCodeWriter proglogCodeWriter, SourceCodeWriter sourceCodeWriter) {
		for( Entry<String, ClazzModel> entry : clazzModels.entrySet() ) {
			entry.getValue().start(getPackageName(entry.getKey()), entry.getKey());
			entry.getValue().generate();
			Logger.w("CodeModel generate: " + getPackageName(entry.getKey()) );
			entry.getValue().build(proglogCodeWriter, sourceCodeWriter);
		}
	}
	
}