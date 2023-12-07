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
using SeedMappings = std::vector<std::map<uint32_t, int32_t>>;

std::pair<std::vector<uint32_t>, SeedMappings> parseInput();
uint32_t getSeedLocation(uint32_t seed, const SeedMappings& mappings);

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

std::pair<std::vector<uint32_t>, SeedMappings> parseInput() {
	std::vector<uint32_t> seeds;
	std::cin.ignore("seeds: "sv.size(), ' ');
	uint32_t seed;
	while (std::cin >> seed) {
		seeds.push_back(seed);
	}

	SeedMappings mappings;
	while (!std::cin.eof()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore map header

		std::map<uint32_t, int32_t> currentMapping;
		uint32_t destinationStart, sourceStart, length;
		while (std::cin >> destinationStart >> sourceStart >> length) {
			currentMapping.try_emplace(sourceStart, 0);
			currentMapping[sourceStart + length] = destinationStart - sourceStart;
		}
		currentMapping.emplace(std::numeric_limits<uint32_t>::max(), 0); // Ensure we always have an upper bound
		mappings.push_back(std::move(currentMapping));
	}

	return {std::move(seeds), std::move(mappings)};
}

uint32_t getSeedLocation(uint32_t seed, const SeedMappings& mappings) {
	for (const auto& map : mappings) {
		seed += map.upper_bound(seed)->second;
	}
	return seed;
}