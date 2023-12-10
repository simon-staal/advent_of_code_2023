#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <sstream>
#include <ranges>

// This solution is a "hybrid" which could be used for both part 1 and 2 by just switching the PredictionType passed to predictNextValue()
enum class PredictionType {
    Past,
    Future
};

std::vector<std::vector<int32_t>> parseInput();
int32_t predictNextValue(const std::vector<int32_t>& history, PredictionType type);

int main() {
    auto report = parseInput();
    int32_t sumOfPredictions = 0;
    for (const auto& history : report) {
        sumOfPredictions += predictNextValue(history, PredictionType::Past);
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

int32_t predictNextValue(const std::vector<int32_t>& history, PredictionType type) {
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
        switch (type) {
            case PredictionType::Past:
                prediction += differences.front();
                break;
            case PredictionType::Future:
                prediction = differences.back() - prediction;
                break;
        }
    }

    return prediction;
}