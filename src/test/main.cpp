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
	
	printf("String::join: %s\n", String::join({"hello","world","whats","up"}, " ").c_str());
	printf("stringify(std::list{}): %s\n", stringify(std::list<String>{"hello","world","whats","up"}).c_str());
	
	return 0;
}
