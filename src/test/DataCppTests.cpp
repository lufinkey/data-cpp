//
//  main.cpp
//  DataCppTest
//
//  Created by Luis Finke on 8/15/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include "DataCppTests.hpp"

#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace fgl_data_cpp_tests {
	using namespace fgl;

	void println(const String& str) {
		#ifdef __ANDROID__
			__android_log_print(ANDROID_LOG_DEBUG, "DataCppTests", "%s\n", str.c_str());
		#else
			printf("%s\n", str.c_str());
		#endif
	}

	void runTests() {
		BasicString<char> str = String::join(std::vector<String>{ String("hello"), String(" "), String("world") });

		Map<size_t,String> testMap;
		auto testNewMap = testMap.mapValues([](auto key, auto& value) -> int {
			return 5;
		});
		println((String)"mapped to " + stringify_type<decltype(testNewMap)>());

		auto testNewMap2 = testMap.map([](auto key, auto& value) {
			return make_pair( (int)5, String("what's up yoooo") );
		});
		println((String)"mapped to " + stringify_type<decltype(testNewMap2)>());

		auto result = LinkedList<int>() + ArrayList<int>();
		println(ArrayList<int>{ 1, 2, 5 }.toString());
		println(LinkedList<int>{ 1, 2, 5 }.toString());

		ArrayList<int>().maybeAt(0);
		ArrayList<int>().maybeRefAt(0);

		auto maybeTryResult = maybeTry([]() {
			return 5;
		});
		if(maybeTryResult) {
			println((String)"got maybeTryResult: " + maybeTryResult.value());
		}
		auto maybeTryResult2 = maybeTry([]() -> void {
			println("we in the maybeTry void");
		});
		if(maybeTryResult2) {
			println("we got the maybeTry void");
		}

		println("stringified: " + stringify(ArrayList<int>()));

		Optional<int> opt1(5);
		Optional<int> opt2;
		println((String)"optional 5 == std::nullopt: " + (opt1 == opt2));
		println((String)"optional 5 >= std::nullopt: " + (opt1 >= opt2));
		println((String)"optional 5 <= std::nullopt: " + (opt1 <= opt2));

		auto testList = std::list<String>{"hello","world","whats","up"};

		println("String::join: " + String::join(testList, " "));
		println("stringify(std::list{}): " + stringify(testList));

		println("reversed:");
		String logText;
		for(auto& item : reversed(testList)) {
			logText += item;
			logText += " ";
		}
		println(logText);

		println("enumerate: " + stringify_type<decltype(enumerate(testList))>());
		for(auto [i, item] : enumerate(testList)) {
			println(String::join({
				"\t",
				stringify_type<decltype(i)>(),": ",stringify(i),", ",
				stringify_type<decltype(item)>(),": ",stringify(item)
			}));
			// replace item
			if(item == "whats") {
				item = "what the hecc is";
			}
		}
		println("modified list to: " + stringify(testList));

		println("reversed enumerate: " + stringify_type<decltype(reversed(enumerate(testList)))>());
		for(auto [i, item] : reversed(enumerate(testList))) {
			println((String)"\t " + (int)i + " " + item);
		}
		println("done enumerating in reverse");

		logText = "range(6) (" + stringify_type<decltype(*range(6).begin())>() + "): ";
		for(auto i : range(6)) {
			logText += (String)"" + (int)i + ", ";
		}
		println(logText);

		logText = "range(0, 5, 2) ("
				+ stringify_type<decltype(*range(0, 5, 2).begin())>() + "): ";
		for(auto i : range(0, 5, 2)) {
			logText += (String)"" + (int)i + ", ";
		}
		println(logText);

		logText = "range(0.0.3279875724, 5.6819, (2.0/3.0)) ("
				+ stringify_type<decltype(*range(0.3279875724, 5.6819, (1.0/3.0)).begin())>()
				+ "): ";
		for(auto i : range(0.3279875724, 5.6819, (1.0/3.0))) {
			logText += (String)"" + (double)i + ", ";
		}
		println(logText);
		
		auto optStr = Optional<String>("hello world");
		auto mappedOptInt = optStr.map([](const auto& str) -> int {
			println("mapping from "+str);
			return 5;
		});
		println((String)"mapped value: "+stringify(mappedOptInt));

		println("Finished running data-cpp tests");
	}
}
