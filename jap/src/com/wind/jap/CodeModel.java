package com.wind.jap;

import java.io.IOException;
import java.lang.annotation.Annotation;
import java.lang.annotation.ElementType;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import javax.annotation.processing.RoundEnvironment;
import javax.lang.model.element.AnnotationMirror;
import javax.lang.model.element.Element;
import javax.lang.model.element.TypeElement;
import javax.lang.model.type.TypeMirror;
import javax.lang.model.util.Elements;

import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JFieldVar;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JSwitch;
import com.sun.codemodel.writer.PrologCodeWriter;


public class CodeModel {
	/**
	 * Global var.
	 */
	private Map<Class<?>, ClazzModel> clazzModels = null;
	public CodeModel() {
		clazzModels = new HashMap<Class<?>, ClazzModel>();
	}
	public boolean checkIn(Class<?> topLevelClazz, TypeElement annotaion, Element target) {
		if( !clazzModels.containsKey(topLevelClazz) ){
			clazzModels.put(topLevelClazz, new ClazzModel());
		}
		ClazzModel cm = clazzModels.get(topLevelClazz);
		
		AnnotationMirror mirror = null;
		for( AnnotationMirror am : target.getAnnotationMirrors() ) {
			if( am.getAnnotationType() == annotaion.asType() ){
				mirror = am;
				break;
			}
		}
		if( mirror != null && cm != null ){
			Argument arg = new Argument(target, mirror);
			return cm.checkIn(mirror.getAnnotationType().toString(), arg);
		}
		return false;
	}
	public String getPackageName( Class<?> clazz ) {
		return clazz.getClass().getPackage().getName();
	}
	public void generate(PrologCodeWriter proglogCodeWriter, SourceCodeWriter sourceCodeWriter) {
		for( Entry<Class<?>, ClazzModel> entry : clazzModels.entrySet() ) {
			entry.getValue().start(getPackageName(entry.getKey()), entry.getKey());
			entry.getValue().generate();
			entry.getValue().build(proglogCodeWriter, sourceCodeWriter);
		}
	}
	
}