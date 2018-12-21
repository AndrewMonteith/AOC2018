#include "Utils.hpp"
#include <functional>
#include <numeric>

std::vector<int> ParseInput(const std::string& line) {
	auto ss = std::stringstream(line);
	auto input = std::vector<int>();

	int n;
	while (ss >> n) {
		input.push_back(n);
	}

	return input;
}

struct TreeNode {
	std::vector<TreeNode> children;
	std::vector<int> metadata;
};

TreeNode _ParseInputTree(const std::vector<int>& input, int& index) {
	auto num_of_children = input[index++];
	auto num_of_metadata = input[index++];
		
	auto children = std::vector<TreeNode>();
	for (auto i = 0; i < num_of_children; ++i) {
		children.emplace_back(_ParseInputTree(input, index));
	}

	auto metadata = std::vector<int>();
	for (auto i = 0; i < num_of_metadata; ++index, ++i) {
		metadata.push_back(input[index]);
	}

	return TreeNode{std::move(children), 
		std::move(metadata)};
}

TreeNode ParseInputTree(const std::vector<int>& input) {
	int i = 0;
	return _ParseInputTree(input, i);
}

void WalkTree(const TreeNode& node, std::function<void(const TreeNode&)> callback) {
	callback(node);

	for (auto& child : node.children) {
		WalkTree(child, callback);
	}
}

int SolvePart1(const std::vector<int>& input) {
	auto root = ParseInputTree(input);
	auto metadata_sum = 0;

	WalkTree(root, [&](const TreeNode& node) {
		metadata_sum += std::accumulate(node.metadata.begin(), node.metadata.end(), 0);
	});

	return metadata_sum;
}

int GetValueOfNode(const TreeNode& node) {
	if (node.children.size() == 0) {
		return std::accumulate(node.metadata.begin(), node.metadata.end(), 0);
	} else {
		auto sum = 0;

		for (auto& reference : node.metadata) {
			if (reference == 0 || reference > node.children.size()) { continue; }

			sum += GetValueOfNode(node.children[reference-1]);
		}
		
		return sum;
	}
}

int SolvePart2(const std::vector<int>& input) {
	auto root = ParseInputTree(input);

	return GetValueOfNode(root);
}

int main() {
	auto raw_input = aoc::ReadLine("Day8");
	auto input = ParseInput(raw_input);

	auto solution1 = SolvePart1(input);
	auto solution2 = SolvePart2(input);

	std::cout << "Part 1 Solution:" << solution1 << std::endl;
	std::cout << "Part 2 Solution:" << solution2 << std::endl;
	return 0;
}
