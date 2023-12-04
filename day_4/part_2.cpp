#include <cstdint>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>

using namespace std::literals;

struct Scorecard {
	const std::vector<uint32_t> ourNumbers;
	const std::unordered_set<uint32_t> winningNumbers;
};

std::vector<Scorecard> parseScorecards();

int main() {
	const std::vector<Scorecard> scorecards {parseScorecards()};
	std::vector<uint32_t> scorecardsToProcess(scorecards.size(), 1);

	uint32_t processedScorecards = 0;

	// std::views::enumerate only available in C++23 ;(
	for (size_t scorecardId = 0; scorecardId < scorecards.size(); ++scorecardId) {
		uint32_t count = scorecardsToProcess.at(scorecardId);
		const Scorecard& scorecard = scorecards.at(scorecardId);
		uint32_t nextId = scorecardId + 1;
		for (uint32_t number : scorecard.ourNumbers) {
			if (scorecard.winningNumbers.contains(number)) {
				scorecardsToProcess.at(nextId++) += count;
			}
		}
		processedScorecards += count;
	}

	std::cout << processedScorecards << std::endl;
}

std::vector<Scorecard> parseScorecards() {
	std::string line;
	std::vector<Scorecard> scorecards;
	while (std::getline(std::cin, line)) {
		std::stringstream ss(std::move(line));
		ss.ignore("Game XXX: "sv.size(), ':');
		std::vector<uint32_t> ourNumbers;
		uint32_t number;
		ss.ignore(": "sv.size(), ' ');
		while (ss >> number) {
			ourNumbers.push_back(number);
		}
		ss.clear();
		ss.ignore("| "sv.size(), ' ');
		std::unordered_set<uint32_t> winningNumbers;
		while (ss >> number) {
			winningNumbers.insert(number);
		}
		scorecards.emplace_back(std::move(ourNumbers), std::move(winningNumbers));
	}
	return scorecards;
}