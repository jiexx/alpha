package com.wind.jap;

import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.Set;

import javax.annotation.processing.AbstractProcessor;
import javax.annotation.processing.Filer;
import javax.annotation.processing.Messager;
import javax.annotation.processing.ProcessingEnvironment;
import javax.annotation.processing.RoundEnvironment;
import javax.lang.model.SourceVersion;
import javax.lang.model.element.Element;
import javax.lang.model.element.TypeElement;
import javax.tools.Diagnostic.Kind;

import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.writer.PrologCodeWriter;
import com.wind.ui.ACTIVITY;



public class Processor extends AbstractProcessor {
	private Filer filer;
	private Messager messager;
	private String className;
	@Override
	public synchronized void init(ProcessingEnvironment processingEnv) {
		super.init(processingEnv);
		filer = processingEnv.getFiler();
		messager = processingEnv.getMessager();
	}

	@Override
	public Set<String> getSupportedAnnotationTypes() {
		Set<String> annotataions = new LinkedHashSet<String>();
		annotataions.add(ACTIVITY.class.getCanonicalName());
		return annotataions;
	}

	@Override
	public SourceVersion getSupportedSourceVersion() {
		return SourceVersion.latestSupported();
	}
	@Override
	public boolean process(Set<? extends TypeElement> annotations, RoundEnvironment roundEnv) {
		// TODO Auto-generated method stub

		HashMap<String, String> values = new HashMap<String, String>();

		for (TypeElement te : annotations) {
			for (Element e : roundEnv.getElementsAnnotatedWith(te))
				processAnnotation(e, values, messager);
		}
		if (values.size() > 0)
			try {
				generateOptionProcessor(filer, values);
			} catch (Exception e) {
				messager.printMessage(Kind.ERROR, e.getMessage());
			}

		return (true);
	}
	
	private void processAnnotation(Element element,	HashMap<String, String> values, Messager msg) {

		ACTIVITY opt = element.getAnnotation(ACTIVITY.class);

		className = element.getEnclosingElement().toString();

		if (!element.asType().toString().equals(String.class.getName())) {
			msg.printMessage(Kind.WARNING, element.asType() + " not supported. " + opt.getClass().getName() + " not processed");
			return;
		}

		values.put(opt.getClass().getName(), element.getSimpleName().toString());
	}
	
	private void generateOptionProcessor(Filer filer, HashMap<String, String> values) throws Exception {
		
		JCodeModel cm = new JCodeModel();
		
		SourceCodeWriter sourceCodeWriter = new SourceCodeWriter(filer, cm);
		
		PrologCodeWriter prologCodeWriter = new PrologCodeWriter(sourceCodeWriter, "AUTO generate by jaf, DON'T modify.\n");
		
		cm.build(prologCodeWriter);
	}   

}