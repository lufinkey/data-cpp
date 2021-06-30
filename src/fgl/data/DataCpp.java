package com.lufinkey.libdatacpp;

public class DataCpp {
	static {
		loadLibraries();
	}

	public static void loadLibraries() {
		System.loadLibrary("DataCpp");
	}
}
