#include <cstdint>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <ranges>

using namespace std::literals;

uint32_t getScore(const std::vector<uint32_t>& ourNumbers, const std::unordered_set<uint32_t>& winningNumbers);

int main() {
	uint32_t sumOfPoints = 0;
	std::string line;
	while (std::getline(std::cin, line)) {
		std::stringstream ss(std::move(line));
		ss.ignore("Game XXX: "sv.size(), ':');

		std::vector<uint32_t> ourNumbers;
		uint32_t number;
		while (ss >> number) {
			ourNumbers.push_back(number);
		}

		ss.clear();
		ss.ignore("| "sv.size(), ' ');
		std::unordered_set<uint32_t> winningNumbers;
		while (ss >> number) {
			winningNumbers.insert(number);
		}

		sumOfPoints += getScore(ourNumbers, winningNumbers);
	}
    
	std::cout << sumOfPoints << std::endl;
}

uint32_t getScore(const std::vector<uint32_t>& ourNumbers, const std::unordered_set<uint32_t>& winningNumbers) {
    auto isWinner = [&winningNumbers](uint32_t number){
        return winningNumbers.contains(number);
    };

	uint32_t score = 0;
	for ([[maybe_unused]] uint32_t _ : ourNumbers | std::views::filter(isWinner)) {
        score = score ? score * 2 : 1;
	}
	return score;
}