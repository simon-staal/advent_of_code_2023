#include <limits>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <string_view>
#include <cassert>
#include <vector>
#include <ranges>
#include <algorithm>

using namespace std::literals;
using Network = std::unordered_map<std::string, std::pair<std::string, std::string>>;

std::pair<std::string, Network> parseInput();
uint32_t calculateStepsToEnd(const std::string& start, const std::string& directions, const Network& network);
uint64_t lcm(const std::vector<uint32_t>& numbers);

int main() {
	const auto [directions, network] = parseInput();

	std::vector<std::string> startNodes;
	for (const auto& [source, _] : network) {
		if (source.back() == 'A') {
			startNodes.push_back(source);
		}
	}

    std::vector<uint32_t> steps;
	
	for (const auto& start : startNodes) {
        steps.push_back(calculateStepsToEnd(start, directions, network));
    }

    // I'm not sure why this works, can't figure out from the question why we're guaranteed for this to be true...
	std::cout << lcm(steps) << std::endl;
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

uint32_t calculateStepsToEnd(const std::string& start, const std::string& directions, const Network& network) {
    std::string current = start;
    uint32_t steps = 0;
	for (; current.back() != 'Z'; steps++) {
		char direction = directions[steps % directions.size()];
		switch (direction) {
			case 'L':
				current = network.at(current).first;
				break;
			case 'R':
				current = network.at(current).second;
				break;
			default:
				assert(false);
		}
	}

    std::cerr << start << " -> " << current << ": " << steps << std::endl;
    return steps;
}

uint64_t lcm(const std::vector<uint32_t>& numbers) {
    auto gcd = [](uint64_t a, uint64_t b) {
        while (b != 0) {
            uint64_t temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    };

    auto lcm = [gcd](uint64_t a, uint64_t b) {
        assert(a != 0 && b != 0);
        return (a / gcd(a, b)) * b;
    };

    assert(!numbers.empty());

    uint64_t result = 1;
    for (uint32_t number : numbers) {
        result = lcm(result, number);
    }

    return result;
}