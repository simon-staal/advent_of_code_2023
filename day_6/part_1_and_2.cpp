#include <iostream>
#include <vector>
#include <string_view>
#include <cstdint>
#include <cassert>
#include <cmath>

using namespace std::literals;

// Had to upgrade to 64 bits for enough size / precision for part 2
std::pair<std::vector<uint64_t>, std::vector<uint64_t>> parseInput();
std::pair<double, double> solveQuadratic(double a, double b, double c);

int main() {
	auto [times, distances] = parseInput();
	uint64_t productOfMargin = 1;

	// std::views::zip only available in C++23 ;(
	for (size_t i = 0; i < times.size(); ++i) {
		int64_t time = times[i], distance = distances[i];

		// Generalise problem to solve x(t-x) > d
		auto [lower, upper] = solveQuadratic(-1, time, -distance);

        auto isNear = [](double a, double b) {
            return std::abs(a - b) < 1e-6;
        };

        // Ensure x != d
        if (isNear(lower, std::ceil(lower)))
            lower++;
        if (isNear(upper, std::floor(upper)))
            upper--;

        uint64_t winningRange = static_cast<uint64_t>(std::floor(upper)) 
            - static_cast<uint64_t>(std::ceil(lower)) + 1;
		productOfMargin *= winningRange;
	}

	std::cout << productOfMargin << std::endl;
}

std::pair<std::vector<uint64_t>, std::vector<uint64_t>> parseInput() {
	std::cin.ignore("Time: "sv.size(), ' ');
	std::vector<uint64_t> times;
	uint64_t time;
	while (std::cin >> time) {
		times.push_back(time);
	}
	std::cin.clear();
	std::cin.ignore("Distance: "sv.size(), ' ');
	
	std::vector<uint64_t> distances;
	uint64_t distance;
	while (std::cin >> distance) {
		distances.push_back(distance);
	}

	assert(times.size() == distances.size());

	return {std::move(times), std::move(distances)};
}

std::pair<double, double> solveQuadratic(double a, double b, double c) {
	double discriminant = b * b - 4 * a * c;
	assert(discriminant > 0);

	double root = std::sqrt(discriminant);
	double xLo = (-b + root) / (2. * a);
	double xHi = (-b - root) / (2. * a);

	if (xLo > xHi) 
		std::swap(xLo, xHi);

	return {xLo, xHi};
}