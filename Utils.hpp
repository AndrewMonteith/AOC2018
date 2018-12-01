#include <string>
#include <vector>
#include <fstream>
#include <iostream>

namespace AOC {
	std::vector<std::string> ReadLines(std::string filename) {
		auto file = std::ifstream(filename);
		auto result = std::vector<std::string>();
		
		auto line = std::string();
		while (file >> line) {
			result.emplace_back(line);
		}


		return result; 
	}
}