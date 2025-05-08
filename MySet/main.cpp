#include <chrono>
#include <iostream>
#include <set>
#include "MySet.h"


void benchmark(int cycles) {
	MyContainers::MySetV2<std::string> set;
	std::cout << "\nStarting MySet benchmark" << std::endl;

	const auto strings = std::vector<std::string>{ "a", "b", "c", "d", "e" };
	const int strings_size = strings.size(); // so i dont ask for the size every loop

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < cycles; i++) {
		// add all strings, try to add them all again, remove all strings
		// so we execute every method of the MySet class (add, contains and remove)
		for (int j = 1; j < strings_size; j++) {
			set.add(strings[j]);
		}
		for (int j = 1; j < strings_size; j++) {
			set.add(strings[j]);
		}
		for (int j = 1; j < strings_size; j++) {
			set.remove(strings[j]);
		}
	}
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsedMs = end - start;

	std::cout << "Elapsed milliseconds for " << cycles << " cycles: " << elapsedMs.count() << std::endl;
}

void benchmark_std_set(int cycles) {
	std::set<std::string> set;
	std::cout << "\nStarting std::set benchmark" << std::endl;

	const auto strings = std::vector<std::string>{ "a", "b", "c", "d", "e" };
	const int strings_size = strings.size(); // so i dont ask for the size every loop

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < cycles; i++) {
		for (int j = 0; j < strings_size; j++) {
			set.insert(strings[j]);
		}
		for (int j = 0; j < strings_size; j++) {
			set.insert(strings[j]);
		}
		for (int j = 0; j < strings_size; j++) {
			set.erase(strings[j]);
		}
	}
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsedMs = end - start;

	std::cout << "Elapsed milliseconds for " << cycles << " cycles: " << elapsedMs.count() << std::endl << std::endl;
}


int main()
{
	// create your set 
	MyContainers::MySetV2<std::string> set;
	
	// add some data 
	set.add(std::string("Some data"));
	std::cout << set;
	set.add(std::string("2"));
	std::cout << set;

	set.add(std::string("3"));
	std::cout << set;

	set.remove("3");
	std::cout << set;

	set.add(std::string("4"));
	set.add(std::string("5"));
	set.add(std::string("6"));
	set.add(std::string("7"));
	set.add(std::string("8"));
	set.add(std::string("9"));
	set.add(std::string("10"));
	set.add(std::string("11"));
	set.add(std::string("12"));
	set.add(std::string("13"));
	set.add(std::string("14"));

	std::cout << set;

	// check some data 
	std::cout << (set.contains(std::string("Some data")) ? "yes" : "no") << std::endl;
	std::cout << (set.contains(std::string("some data")) ? "yes" : "no") << std::endl;
	
	// remove some data 
	std::cout << (set.remove(std::string("Some data")) ? "contained" : "did not contain") << std::endl; 
	std::cout << (set.remove(std::string("some data")) ? "contained" : "did not contain") << std::endl; 
	
	std::cout << std::endl << set;

	MyContainers::MySetV2<std::string> set2 = set;

	std::cout << "set: " << std::endl << set;
	std::cout << "set 2: " << std::endl << set2;

	set.remove("5");
	set.remove("6");
	set.remove("7");
	set.remove("8");
	set.remove("9");
	set.remove("10");

	// moving
	MyContainers::MySetV2<std::string> set3 = std::move(set);
	//std::cout << std::endl << set; // Error
	std::cout << "set 3: " << std::endl << set3;

	std::cout << "set2 == set3: " << (set2 == set3) << std::endl;

	MyContainers::MySetV2<std::string> set4 = set3;
	std::cout << "set3 == set4: " << (set3 == set4) << std::endl;

	std::cout << "set 3: " << std::endl << set3;
	std::cout << "set 4: " << std::endl << set4;

	std::cout << "element at index 3: " << set4.at(3) << std::endl;
	
	{
		auto set5 = std::move(set3);
		std::cout << "Set 5:" << set5 << std::endl;
	}

	set4 = set4;
	std::cout << "set 4: " << std::endl << set4;

	// Benchmarking? ;p
	benchmark(1'000'000);
	benchmark_std_set(1'000'000);

	// check out of bounds
	//std::cout << "element at index 100" << set4.at(100) << std::endl;

	return 0;
}
