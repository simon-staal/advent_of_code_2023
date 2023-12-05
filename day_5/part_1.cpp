#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include <cstdint>
#include <string_view>
#include <limits>
#include <ranges>
#include <algorithm>

using namespace std::literals;

std::pair<std::vector<uint32_t>, std::vector<std::map<uint32_t, int32_t>>> parseInput();
uint32_t getSeedLocation(uint32_t seed, const std::vector<std::map<uint32_t, int32_t>>& mappings);

int main() {
	const auto [seeds, seedMappings] = parseInput();

	auto seedLocations = seeds | std::views::transform(
		[&seedMappings](uint32_t seed) {
			return getSeedLocation(seed, seedMappings);
		}
	);

	uint32_t minLocation = *std::ranges::min_element(seedLocations);
	std:: cout << minLocation << std::endl;
}

std::pair<std::vector<uint32_t>, std::vector<std::map<uint32_t, int32_t>>> parseInput() {
	std::vector<uint32_t> seeds;
	std::cin.ignore("seeds: "sv.size(), ' ');
	uint32_t seed;
	while (std::cin >> seed) {
		seeds.push_back(seed);
	}

	std::vector<std::map<uint32_t, int32_t>> seedMappings;
	while (!std::cin.eof()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore map header

		std::map<uint32_t, int32_t> mapping;
		uint32_t destinationStart;
		while (std::cin >> destinationStart) {
			uint32_t sourceStart;
			uint32_t length;
			std::cin >> sourceStart >> length;
			
			mapping.emplace(sourceStart, 0);
			mapping[sourceStart + length] = destinationStart - sourceStart;
		}
		mapping.emplace(std::numeric_limits<uint32_t>::max(), 0); // Ensure we always have an upper bound
		seedMappings.push_back(std::move(mapping));
	}

	return {std::move(seeds), std::move(seedMappings)};
}

uint32_t getSeedLocation(uint32_t seed, const std::vector<std::map<uint32_t, int32_t>>& mappings) {
	for (const auto& map : mappings) {
		seed += map.upper_bound(seed)->second;
	}
	return seed;
}