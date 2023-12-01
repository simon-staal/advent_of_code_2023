#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <ranges>
#include <unordered_map>
#include <optional>

int32_t getCalibrationValue(const std::string& line);
std::optional<int32_t> tryGetDigit(const std::string& line, size_t i);

static std::unordered_map<std::string, int32_t> kWordToDigit {
	{"one", 1},
	{"two", 2},
	{"three", 3},
	{"four", 4},
	{"five", 5},
	{"six", 6},
	{"seven", 7},
	{"eight", 8},
	{"nine", 9}
};

int main() {
	std::string line;
	int32_t total = 0;
	while (std::getline(std::cin, line)) {
		total += getCalibrationValue(line);
	}

	std::cout << total << std::endl;
}

int32_t getCalibrationValue(const std::string& line) {
	int32_t calibrationValue = 0;
	for (size_t i = 0; i < line.size(); ++i) {
		if (auto digit = tryGetDigit(line, i); digit) {
			calibrationValue += digit.value() * 10;
			break;
		}
	}

	for (size_t i = line.size(); i > 0; --i) {
		if (auto digit = tryGetDigit(line, i - 1); digit) {
			calibrationValue += digit.value();
			break;
		}
	}

	return calibrationValue;
}

std::optional<int32_t> tryGetDigit(const std::string& line, size_t i)
{
	if (i >= line.size()) {
		throw std::out_of_range("Index out of bounds");
	}

	if (isdigit(line[i])) {
		return (line[i] - '0');
	}

	for (const auto& [word, digit] : kWordToDigit) {
		if (line.substr(i, word.size()) == word) {
			return digit;
		}
	}

	return {};
}