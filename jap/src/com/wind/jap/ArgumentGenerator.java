package com.wind.jap;

public class ArgumentGenerator {
	public String generatorName;
	public Generator generator;
	public ArgumentGenerator( String name, Generator g ) {
		generatorName = name;
		generator = g;
	}
}
