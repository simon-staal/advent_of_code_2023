#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <sstream>
#include <ranges>

std::vector<std::vector<int32_t>> parseInput();
int32_t predictNextValue(const std::vector<int32_t>& history);

int main() {
    auto report = parseInput();
    int32_t sumOfPredictions = 0;
    for (const auto& history : report) {
        sumOfPredictions += predictNextValue(history);
    }

    std::cout << sumOfPredictions << std::endl;
}

std::vector<std::vector<int32_t>> parseInput() {
    std::string line;
    std::vector<std::vector<int32_t>> report;
    while (std::getline(std::cin, line)) {
        std::stringstream ss{std::move(line)};
        int32_t val;
        std::vector<int32_t> history;
        while (ss >> val) {
            history.push_back(val);
        }
        report.push_back(std::move(history));
    }
    return report;
}

// I feel as though there's some nice mathematical way to find a closed-form solution to this problem (it kinda looks like a rotated Pascal's triangle), but I cba figuring it out
int32_t predictNextValue(const std::vector<int32_t>& history) {
    std::vector<std::vector<int32_t>> stackOfDifferences {history};
    auto isNonZero = [](int32_t x) { return x != 0; };

    while (!std::views::filter(stackOfDifferences.back(), isNonZero).empty()) {
        const auto& prevDifference = stackOfDifferences.back();
        std::vector<int32_t> differences;
        for (size_t i = 1; i < prevDifference.size(); i++) {
            int32_t prev = prevDifference[i - 1];
            int32_t curr = prevDifference[i];
            differences.push_back(prev - curr);
        }
        stackOfDifferences.push_back(std::move(differences));
    }

    int32_t prediction = 0;
    for (const auto& differences : stackOfDifferences | std::views::reverse) {
        prediction = differences.back() - prediction;
    }

    return prediction;
}