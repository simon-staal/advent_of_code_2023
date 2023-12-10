#include <limits>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <string_view>
#include <cassert>

using namespace std::literals;
using Network = std::unordered_map<std::string, std::pair<std::string, std::string>>;

std::pair<std::string, Network> parseInput();

int main() {
	auto [directions, network] = parseInput();

	uint32_t steps = 0;
	std::string current = "AAA";
	for (; current != "ZZZ"; steps++) {
		char direction = directions[steps % directions.size()];
		switch (direction) {
			case 'L':
				current = network[current].first;
				break;
			case 'R':
				current = network[current].second;
				break;
			default:
				assert(false);
		}
	}
	std::cout << steps << '\n';
}

std::pair<std::string, Network> parseInput() {
	std::string directions;
	std::cin >> directions;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	Network network;
	std::string line;
	while(std::getline(std::cin, line)) {
		std::stringstream ss(std::move(line));
		std::string source, left, right;
		ss >> source;
		ss.ignore(" = ("sv.size(), '(');
		ss >> left;
		assert(left.back() == ',');
		left.pop_back();
		ss >> right;
		assert(right.back() == ')');
		right.pop_back();
		network.emplace(std::move(source), std::make_pair(std::move(left), std::move(right)));
	}

	return std::make_pair(std::move(directions), std::move(network));
}