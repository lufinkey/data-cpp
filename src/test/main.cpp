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
	
	ArrayList<int>().maybeAt(0);
	ArrayList<int>().maybeRefAt(0);
	
	Optional<String> optStr = std::nullopt;
	Optional<String> optStr2 = "ayy";
	auto chainResult = chain<String>([&](){ return opt(optStr)->append(opt(optStr2).value()); });
	chain([&](){ return opt(optStr)->append(opt(optStr2).value()); });
	
	auto maybeTryResult = maybeTry([]() {
		return 5;
	});
	if(maybeTryResult) {
		printf("got maybeTryResult: %i\n", maybeTryResult.value());
	}
	
	printf("stringified: %s\n", stringify(ArrayList<int>()).c_str());
	
	return 0;
}
