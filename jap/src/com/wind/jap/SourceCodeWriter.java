package com.wind.jap;

import java.io.IOException;
import java.io.OutputStream;
import java.io.Writer;

import javax.annotation.processing.Filer;
import javax.annotation.processing.FilerException;
import javax.lang.model.element.Element;
import javax.tools.JavaFileObject;



import com.sun.codemodel.CodeWriter;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JPackage;

public class SourceCodeWriter extends CodeWriter {

	private static final VoidOutputStream VOID_OUTPUT_STREAM = new VoidOutputStream();
	private final Filer filer;
	private final JCodeModel codeModel;

	private static class VoidOutputStream extends OutputStream {
		@Override
		public void write(int arg0) throws IOException {
			// Do nothing
		}
	}

	public SourceCodeWriter(Filer filer, JCodeModel cm) {
		this.filer = filer;
		this.codeModel = cm;
	}

	@Override
	public OutputStream openBinary(JPackage pkg, String fileName) throws IOException {
		String qualifiedClassName = toQualifiedClassName(pkg, fileName);

		try {
			JavaFileObject sourceFile;

			sourceFile = filer.createSourceFile(qualifiedClassName);
			Writer writer =  ((JavaFileObject)filer.createSourceFile(qualifiedClassName)).openWriter();
			
			JDefinedClass jdc;
			jdc.owner()._class(fullyqualifiedName);
			
			writer.write("public class " + generatedClassName + " {\n");

			writer.write("\tpublic static " + className	+ " process(String[] args) {\n");

			writer.write("\t\t" + className + " options = new " + className
					+ "();\n");
			writer.write("\t\tint idx = 0;\n");

			writer.write("\t\twhile (idx < args.length) {\n");

			for (String key : values.keySet()) {
				writer.write("\t\t\tif (args[idx].equals(\"" + key + "\")) {\n");
				writer.write("\t\t\t\toptions." + values.get(key)
						+ " = args[++idx];\n");
				writer.write("\t\t\t\tidx++;\n");
				writer.write("\t\t\t\tcontinue;\n");
				writer.write("\t\t\t}\n");
			}

			writer.write("\t\t\tSystem.err.println(\"Unknown option: \" + args[idx++]);\n");

			writer.write("\t\t}\n");

			writer.write("\t\treturn (options);\n");
			writer.write("\t}\n");

			writer.write("}");
			writer.flush();
			writer.close();
			
			return sourceFile.openOutputStream();
		} catch (FilerException e) {
			return VOID_OUTPUT_STREAM;
		}
	}

	private String toQualifiedClassName(JPackage pkg, String fileName) {
		int suffixPosition = fileName.lastIndexOf('.');
		String className = fileName.substring(0, suffixPosition);

		String qualifiedClassName = pkg.name() + "." + className;
		return qualifiedClassName;
	}

	@Override
	public void close() throws IOException {
	}
	

}