#include <iostream>
#include <list>
#include <functional>
#include "Utils.hpp"

inline bool SameLetterDifferentCase(char letter1, char letter2) {
	return std::abs(letter1-letter2) == 32;
}

inline bool SameLetter(char letter1, char letter2) {
	return letter1 == letter2 || SameLetterDifferentCase(letter1, letter2); 
}

bool DoReactionPass(std::list<char>& list) {
	auto did_change = true;
	while (did_change) {
		did_change = false;

		auto it = list.begin();
		while (it != list.end()) {
			auto next = std::next(it, 1);
			if (next == list.end()) {
				break;
			}
			
			if (SameLetterDifferentCase(*it, *next)) {
				list.erase(it);
				it = std::next(next, 1);
				list.erase(next);
				did_change = true;
			} else {
				++it;
			}
		}
	}

	return did_change;
}

inline void FullyReact(std::list<char>& list) {
	while (DoReactionPass(list)) ;
}

std::list<char> StringToLinkedList(const std::string& line) {
	auto result = std::list<char>();

	for(auto& letter : line) {
		result.push_back(letter);
	}

	return result;
}

std::string LinkedListToString(const std::list<char> list) {
	auto result = std::string();

	for (auto& letter : list) {
		result += letter;
	}

	return result;
}

int SolvePart1(const std::string& s) {
	auto ll = StringToLinkedList(s);

	FullyReact(ll);

	return LinkedListToString(ll).size();
}

void for_each_letter(std::function<void(char)> letter_callback) {
	for (auto letter = 97; letter <= 122; ++letter) {
		letter_callback(letter);
	}
}

std::list<char> ListWithoutLetter(const std::string& input, char letter) {
	auto result = std::list<char>();
	for (auto& input_letter : input) {
		if (!SameLetter(input_letter, letter)) {
			result.push_back(input_letter);
		}
	}

	return result;
}

int SolvePart2(const std::string& s) {
	auto shortest_length = s.size();

	for_each_letter([&](char letter) {
		auto list_without_letter = ListWithoutLetter(s, letter);
		FullyReact(list_without_letter);
		auto result = LinkedListToString(list_without_letter);

		if (result.size() < shortest_length) {
			shortest_length = result.size();
		}
	});

	return shortest_length;
}

int main() {
	auto input = aoc::ReadLine("Day5.txt");

	auto solution1 = SolvePart1(input);
	auto solution2 = SolvePart2(input);
	
	std::cout << "Solution Part 1:" << solution1 << std::endl;
	std::cout << "Solution Part 2:" << solution2 << std::endl;
    return 0;
}
