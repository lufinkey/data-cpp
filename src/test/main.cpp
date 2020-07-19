//
//  main.cpp
//  DataCppTest
//
//  Created by Luis Finke on 8/15/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include <fgl/data.hpp>

using namespace fgl;

int main(int argc, char* argv[]) {
	BasicString<char> str = String::join(std::vector<String>{ String("hello"), String(" "), String("world") });
	
	auto result = LinkedList<int>() + ArrayList<int>();
	printf("%s\n", ArrayList<int>{ 1, 2, 5 }.toString().c_str());
	printf("%s\n", LinkedList<int>{ 1, 2, 5 }.toString().c_str());
	
	ArrayList<int>().maybeAt(0);
	ArrayList<int>().maybeRefAt(0);
	
	auto maybeTryResult = maybeTry([]() {
		return 5;
	});
	if(maybeTryResult) {
		printf("got maybeTryResult: %i\n", maybeTryResult.value());
	}
	auto maybeTryResult2 = maybeTry([]() -> void {
		printf("we in the maybeTry void\n");
	});
	if(maybeTryResult2) {
		printf("we got the maybeTry void\n");
	}
	
	printf("stringified: %s\n", stringify(ArrayList<int>()).c_str());
	
	Optional<int> opt1(5);
	Optional<int> opt2;
	printf("optional 5 == std::nullopt: %i\n", (int)(opt1 == opt2));
	printf("optional 5 >= std::nullopt: %i\n", (int)(opt1 >= opt2));
	printf("optional 5 <= std::nullopt: %i\n", (int)(opt1 <= opt2));
	
	auto testList = std::list<String>{"hello","world","whats","up"};
	
	printf("String::join: %s\n", String::join(testList, " ").c_str());
	printf("stringify(std::list{}): %s\n", stringify(testList).c_str());
	
	printf("enumerate:\n");
	for(auto [i, item] : enumerate(testList)) {
		printf("%s", String::join({
			"\t",
			stringify_type<decltype(i)>(),": ",stringify(i),", ",
			stringify_type<decltype(item)>(),": ",stringify(item),"\n"
		}).c_str());
		if(item == "whats") {
			item = "what the hecc is";
		}
	}
	printf("\nmodified list to:\n%s\n", stringify(testList).c_str());
	
	printf("range(6) (%s): ", stringify_type<decltype(*range(6).begin())>().c_str());
	for(auto i : range(6)) {
		printf("%i, ", (int)i);
	}
	printf("\n");
	printf("range(0, 5, 2) (%s): ", stringify_type<decltype(*range(0, 5, 2).begin())>().c_str());
	for(auto i : range(0, 5, 2)) {
		printf("%i, ", (int)i);
	}
	printf("\n");
	printf("range(0.0.3279875724, 5.6819, (2.0/3.0)) (%s): ", stringify_type<decltype(*range(0.3279875724, 5.6819, (1.0/3.0)).begin())>().c_str());
	for(auto i : range(0.3279875724, 5.6819, (1.0/3.0))) {
		printf("%f, ", (double)i);
	}
	printf("\n");
	
	return 0;
}
