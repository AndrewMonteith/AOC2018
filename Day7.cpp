#include "Utils.hpp"
#include <sstream>
#include <list>
#include <numeric>
#include <algorithm>

using Graph = std::map<char, std::vector<char>>;

std::pair<char, Graph> ParseInput(const std::vector<std::string>& lines) {
	auto result = Graph();
	for (auto letter = 'A'; letter <= 'Z'; ++letter) {
		result[letter] = std::vector<char>();
	}

	auto letters = std::list<char>(26);
	std::iota(letters.begin(), letters.end(), 'A');

	auto add_edge = [&](char from, char to) {
		letters.remove(to);
		result[from].push_back(to);
	};

	for (auto& line : lines) {
		std::stringstream ss(line);

		char from, to;

		ss.ignore(5);
		ss >> from;
		ss.ignore(30);
		ss >> to;

		add_edge(from, to);
	}

	return {letters.front(), result};
}

std::map<char, std::vector<char>> GetVisitDependencies(const Graph& graph) {
	auto result = std::map<char, std::vector<char>>();
	
	for (auto letter = 'A'; letter <= 'Z'; ++letter) {
		auto neighbours = graph.at(letter);
		for (auto& neighbour : neighbours) { // Can get from letter -> neighbour
			result[neighbour].push_back(letter); // So to get to neighbour, you need to visit letter.
		}
	}

	return result;
}

std::vector<char> GetVisitable(const std::set<char> visited, 
									const std::map<char, std::vector<char>>& visit_dependecies) {
	auto result = std::vector<char>();

	for (auto& [letter, dependencies] : visit_dependecies) {
		auto visited_all_dependencies = std::all_of(dependencies.begin(), dependencies.end(),
				[&](const char letter) { return visited.find(letter) != visited.end(); });

		if (visited_all_dependencies && visited.find(letter) == visited.end()) {
			result.push_back(letter);
		}
	}

	return result;
}

std::string SolvePart1(const Graph& graph, char root_node) {
	auto result = std::string();

	auto avaliable = std::set<char> {root_node};

	auto visited = std::set<char>();
	auto visit_dependencies = GetVisitDependencies(graph);

	while (!avaliable.empty()) {
		auto next_to_visit = *avaliable.begin();
		avaliable.erase(avaliable.begin());

		result += next_to_visit;
		visited.insert(next_to_visit);

		for (auto& newly_visitable : GetVisitable(visited, visit_dependencies)) {
			avaliable.insert(newly_visitable);
		}
	}
	
	return result;
}

struct Worker {
	bool working;
	char job;
	int time_remaining;
};

void MakeWorkersWork(std::vector<Worker>& workers, std::set<char>& visitable, std::set<char>& completed) {
	for (auto& worker : workers) {
		if (!worker.working) { continue; }
		
		--worker.time_remaining;
		if (worker.time_remaining == 0) {
			worker.working = false;
			completed.insert(worker.job);
		}
	}
}

inline bool IsJobActive(const std::vector<Worker>& workers, char job) {
	return std::any_of(workers.begin(), workers.end(),
			[&](const Worker& worker) { return worker.job == job && worker.working; });
}


void AssignNewJobs(std::vector<Worker>& workers, std::set<char>& visitable) {
	const int ASCII_TO_JOB_COST_OFFSET = 4;
	for (auto& worker : workers) {
		if (worker.working || visitable.empty()) { continue; }
		auto next_job = *visitable.begin();
		visitable.erase(visitable.begin());
		
		worker.working = true;
		worker.job = next_job;
		worker.time_remaining = next_job-ASCII_TO_JOB_COST_OFFSET;
	}
}

int SolvePart2(const Graph& graph, char root_node, const int num_of_workers) {
	auto result = 0;
	auto workers = std::vector<Worker>(num_of_workers);

	auto visitable = std::set<char> { root_node }, completed = std::set<char> {};
	auto visit_dependencies = GetVisitDependencies(graph);

	AssignNewJobs(workers, visitable);

	do {
		result += 1;

		MakeWorkersWork(workers, visitable, completed);
		
		for (auto& visitable_node : GetVisitable(completed, visit_dependencies)) {
			if (!IsJobActive(workers, visitable_node)) {
				visitable.insert(visitable_node);
			}
		}

		AssignNewJobs(workers, visitable);
	} while (std::any_of(workers.begin(), workers.end(), 
				[](const Worker& worker) { return worker.working; }));

	return result;
}

int main() {
	auto lines = aoc::ReadLines("Day7");
	auto [root_node, graph] = ParseInput(lines);

	auto solution1 = SolvePart1(graph, root_node);
	auto solution2 = SolvePart2(graph, root_node, 5);
	std::cout << "Solution 1:" << solution1 << std::endl;
	std::cout << "Solution 2:" << solution2 << std::endl;
	return 0;
}
