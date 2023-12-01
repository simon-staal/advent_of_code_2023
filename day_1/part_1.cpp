#include <cstdint>
#include <iostream>
#include <string>
#include <ranges>

int32_t getCalibrationValue(const std::string& line);

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
	for (char c : line) {
		if (isdigit(c)) {
			calibrationValue += (c - '0') * 10;
			break;
		}
	}
	for (char c : line | std::views::reverse) {
		if (isdigit(c)) {
			calibrationValue += c - '0';
			break;
		}
	}
	return calibrationValue;
}