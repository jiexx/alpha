package com.wind.jap;

import java.io.IOException;
import java.io.OutputStream;

import javax.annotation.processing.Filer;
import javax.annotation.processing.FilerException;
import javax.lang.model.util.Elements;
import javax.tools.JavaFileObject;



import com.sun.codemodel.CodeWriter;
import com.sun.codemodel.JPackage;

public class SourceCodeWriter extends CodeWriter {

	private static final VoidOutputStream VOID_OUTPUT_STREAM = new VoidOutputStream();
	private final Filer filer;
	private Elements eltUtils;
	private String className; 

	private static class VoidOutputStream extends OutputStream {
		@Override
		public void write(int arg0) throws IOException {
			// Do nothing
		}
	}

	public SourceCodeWriter(Filer filer, Elements util) {
		this.filer = filer;
		this.eltUtils = util;
	}

	@Override
	public OutputStream openBinary(JPackage pkg, String fileName) throws IOException {
		String qualifiedClassName = toQualifiedClassName(pkg, fileName);

		try {
			JavaFileObject sourceFile;
			sourceFile = filer.createSourceFile(qualifiedClassName, eltUtils.getTypeElement(className));
			Logger.w("openBinary:" + qualifiedClassName +"  "+eltUtils.getTypeElement(className));
			return sourceFile.openOutputStream();
		} catch (FilerException e) {
			return VOID_OUTPUT_STREAM;
		}
	}

	private String toQualifiedClassName(JPackage pkg, String fileName) {
		int suffixPosition = fileName.lastIndexOf('.');
		className = fileName.substring(0, suffixPosition);

		String qualifiedClassName = pkg.name() + "." + className;
		return qualifiedClassName;
	}

	@Override
	public void close() throws IOException {
	}
	

}