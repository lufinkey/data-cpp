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
	
	Optional<String> optStr = std::nullopt;
	Optional<String> optStr2 = "ayy";
	auto chainResult = chain<String>([&](){ return opt(optStr)->append(opt(optStr2).value()); });
	chain([&](){ return opt(optStr)->append(opt(optStr2).value()); });
	
	return 0;
}
