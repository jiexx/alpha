package com.wind.jap;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.lang.model.element.AnnotationMirror;
import javax.lang.model.element.AnnotationValue;
import javax.lang.model.element.Element;
import javax.lang.model.element.ElementKind;
import javax.lang.model.element.ExecutableElement;
import javax.lang.model.element.VariableElement;
import javax.lang.model.util.ElementFilter;

/**
 * i.e 
 * @POST("login")
 * public String userlogin(String name){
 * ...
 * }
 * nameOfTarget = "userlogin"
 * typeOfTarget = String
 * argumentsOfTarget = <"name", "String">
 * nameOfAnnotation = "POST"
 * argumentsOfAnnotation = <value, "login">
 * 
 * note: Argument hold only one annotation
 * 
 * @author jluo
 *
 */
public class Argument {
	private Element target;
	private AnnotationMirror mirror;
	public Argument(Element t, AnnotationMirror am){
		target = t;
		mirror = am;
	}
	public String nameOfTarget() {
		return target.getSimpleName().toString();
	}
	public String typeOfTarget() {
		return target.asType().toString();
	}
	public ElementKind kindOfTarget() {
		return target.getKind();
	}
	/** Map<parameter name, parameter type> 
	 * @return
	 */
	public Map<String,String> argumentsOfTarget(){
		Map<String,String> r = new HashMap<String,String>();
		if(target.getKind() == ElementKind.METHOD) {
			ExecutableElement method = (ExecutableElement) target;
			List<? extends VariableElement> parameters = method.getParameters();
			for(VariableElement param : parameters) {
				r.put(param.getSimpleName().toString(), param.asType().toString());
			}
		}
		return r;
	}
	private List<String> targetParamNames = null;
	private List<Class<?>> targetParamTypes = null;
	public int countOfTargetParams(){
		int count = 0;
		if( targetParamNames == null && target.getKind() == ElementKind.METHOD ) {
			targetParamNames = new LinkedList<String>();
			targetParamTypes = new LinkedList<Class<?>>();
			ExecutableElement method = (ExecutableElement) target;
			for( VariableElement param : method.getParameters() ) {
				targetParamNames.add(param.getSimpleName().toString());
				targetParamTypes.add(param.getClass());
			}
			count = targetParamNames.size();
		}
		return count;
	}
	public String nameOfTargetParams( int index ) {
		countOfTargetParams();
		return targetParamNames.get(index);
	}
	public Class<?> clazzOfTargetParams( int index ) {
		countOfTargetParams();
		return targetParamTypes.get(index);
	}
	public String nameOfAnnotation() {
		return mirror.getAnnotationType().getClass().getSimpleName();
	}
	/** Map<parameter , value> 
	 * @return 
	 * @return
	 */
	public Map<? extends ExecutableElement, ? extends AnnotationValue> argumentsOfAnnotation() {
		return mirror.getElementValues();
	}
	private List<String> annotationParamNames = null;
	private List<String> annotationParamTypes = null;
	private List<Object> annotationParamValues = null;
	public int countOfAnnotationParams(){
		if( annotationParamNames == null ) {
			annotationParamNames = new LinkedList<String>();
			annotationParamTypes = new LinkedList<String>();
			annotationParamValues = new LinkedList<Object>();
			for( Entry<? extends ExecutableElement, ? extends AnnotationValue> entry : argumentsOfAnnotation().entrySet() ) {
				annotationParamNames.add( entry.getKey().getSimpleName().toString() );
				annotationParamTypes.add( entry.getKey().asType().toString());
				annotationParamValues.add( entry.getValue().getValue() );
			}
		}
		return annotationParamNames.size();
	}
	public String nameOfAnnotationParams( int index ) {
		countOfAnnotationParams();
		return annotationParamNames.get(index);
	}
	public Object valueOfAnnotationParams( int index ) {
		countOfAnnotationParams();
		return annotationParamNames.get(index);
	}

}