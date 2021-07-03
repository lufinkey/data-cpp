package com.lufinkey;

public class DataCpp {
	static {
		loadLibraries();
	}

	public static void loadLibraries() {
		System.loadLibrary("DataCpp");
	}
}
