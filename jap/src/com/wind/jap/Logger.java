package com.wind.jap;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import javax.annotation.processing.Filer;
import javax.annotation.processing.Messager;
import javax.annotation.processing.ProcessingEnvironment;
import javax.tools.FileObject;
import javax.tools.StandardLocation;
import javax.tools.Diagnostic.Kind;




public class Logger {
	static class FileHolder {
		String dummySourceFilePath;
		File sourcesGenerationFolder;
		File projectRoot;

		public FileHolder(String dummySourceFilePath, File sourcesGenerationFolder, File projectRoot) {
			this.dummySourceFilePath = dummySourceFilePath;
			this.sourcesGenerationFolder = sourcesGenerationFolder;
			this.projectRoot = projectRoot;
		}
	}
	public static Option<FileHolder> findRootProjectHolder(ProcessingEnvironment processingEnv) {
		Filer filer = processingEnv.getFiler();

		FileObject dummySourceFile;
		try {
			dummySourceFile = filer.createResource(StandardLocation.SOURCE_OUTPUT, "", "dummy" + System.currentTimeMillis());
		} catch (IOException ignored) {
			return Option.absent();
		}
		String dummySourceFilePath = dummySourceFile.toUri().toString();

		if (dummySourceFilePath.startsWith("file:")) {
			if (!dummySourceFilePath.startsWith("file://")) {
				dummySourceFilePath = "file://" + dummySourceFilePath.substring("file:".length());
			}
		} else {
			dummySourceFilePath = "file://" + dummySourceFilePath;
		}

		URI cleanURI;
		try {
			cleanURI = new URI(dummySourceFilePath);
		} catch (URISyntaxException e) {
			return Option.absent();
		}

		File dummyFile = new File(cleanURI);
		File sourcesGenerationFolder = dummyFile.getParentFile();
		File projectRoot = sourcesGenerationFolder.getParentFile();

		return Option.of(new FileHolder(dummySourceFilePath, sourcesGenerationFolder, projectRoot));
	}
	public static File resolveOutputDirectory(ProcessingEnvironment processingEnv) {
		Option<File> rootProjectOption = findRootProject(processingEnv);
		if (rootProjectOption.isAbsent()) {
			// Execution root folder
			return null;
		}

		File rootProject = rootProjectOption.get();

		// Target folder - Maven
		File targetFolder = new File(rootProject, "target");
		if (targetFolder.isDirectory() && targetFolder.canWrite()) {
			return targetFolder;
		}

		// Build folder - Gradle
		File buildFolder = new File(rootProject, "build");
		if (buildFolder.isDirectory() && buildFolder.canWrite()) {
			return buildFolder;
		}

		// Bin folder - Eclipse
		File binFolder = new File(rootProject, "bin");
		if (binFolder.isDirectory() && binFolder.canWrite()) {
			return binFolder;
		}

		// Fallback to projet root folder
		return rootProject;
	}

	private static Messager messager = null;
	static FileWriter fileWriter = null;  
	public static Option<File> findRootProject(ProcessingEnvironment processingEnv) {
		Option<FileHolder> rootProjectHolder = findRootProjectHolder(processingEnv);
		if (rootProjectHolder.isAbsent()) {
			return Option.absent();
		}

		return Option.of(rootProjectHolder.get().projectRoot);
	}
	public static void init(ProcessingEnvironment env) {
		if( messager == null )
			messager = env.getMessager();
		if( fileWriter == null )
			try {
				File dir = resolveOutputDirectory(env);
				File file = new File(dir, "japdebug.txt");
				fileWriter = new FileWriter(file);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				w(e.getMessage());
			} 
	}
	public static void w(String log) {
		try {
			fileWriter.write(log+"\r\n");
			fileWriter.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			w(e.getMessage());
		}
		messager.printMessage(Kind.WARNING, log);
	}
	public static void e(String log) {
		try {
			fileWriter.write(log+"\r\n");
			fileWriter.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			w(e.getMessage());
		}
		messager.printMessage(Kind.ERROR, log);
	}
}