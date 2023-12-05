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

// Uppgraded to uint64_t because overflow shit on me

std::pair<std::vector<std::pair<uint64_t, uint64_t>>, std::vector<std::map<uint64_t, int64_t>>> parseInput();
std::map<uint64_t, int64_t> flattenSeedMappings(const std::vector<std::map<uint64_t, int64_t>>& seedMappings);

int main() {
	const auto [seeds, seedMappings] = parseInput();
	std::map<uint64_t, int64_t> seedLocationOffsets = flattenSeedMappings(seedMappings);

	uint64_t minLocation = std::numeric_limits<uint64_t>::max();
	for (const auto& [seed, range] : seeds) {
		uint64_t seedStart = seed;
		auto offsetIt = seedLocationOffsets.upper_bound(seedStart);
		while (offsetIt->first <= seed + range) {
			minLocation = std::min(minLocation, seedStart + offsetIt->second);
			seedStart = offsetIt->first;
			offsetIt++;
		}
	}

	std::cout << minLocation << std::endl;
}

std::pair<std::vector<std::pair<uint64_t, uint64_t>>, std::vector<std::map<uint64_t, int64_t>>> parseInput() {
	std::vector<std::pair<uint64_t, uint64_t>> seeds;
	std::cin.ignore("seeds: "sv.size(), ' ');
	uint64_t seed;
	while (std::cin >> seed) {
		uint64_t range;
		std::cin >> range;
		seeds.emplace_back(seed, range);
	}

	std::vector<std::map<uint64_t, int64_t>> seedMappings;
	while (!std::cin.eof()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore map header

		std::map<uint64_t, int64_t> mapping;
		uint64_t destinationStart;
		while (std::cin >> destinationStart) {
			uint64_t sourceStart;
			uint64_t length;
			std::cin >> sourceStart >> length;
			
			mapping.try_emplace(sourceStart, 0);
			mapping[sourceStart + length] = destinationStart - sourceStart;
		}
        // Ensure we always have lower + upper bounds
        mapping.try_emplace(std::numeric_limits<uint64_t>::min(), 0);
		mapping.try_emplace(std::numeric_limits<uint64_t>::max(), 0); 

		seedMappings.push_back(std::move(mapping));
	}

	return {std::move(seeds), std::move(seedMappings)};
}

std::map<uint64_t, int64_t> flattenSeedMappings(const std::vector<std::map<uint64_t, int64_t>>& seedMappings) {
	std::map<uint64_t, int64_t> flattenedMappings = seedMappings[0];

	for (const auto& mapping : seedMappings | std::views::drop(1)) {
        std::map<uint64_t, int64_t> joinedMap {{0, 0}};
        for (auto it = flattenedMappings.cbegin(); it != flattenedMappings.cend();) {
               auto prev = it;
            it++;
            if (it == flattenedMappings.cend())
                break;
            auto [prevBound, _] = *prev;
            auto [currBound, offset] = *it;
            uint64_t startOfRange = prevBound + offset;
            uint64_t endOfRange = currBound + offset;
            auto offsetIt = mapping.upper_bound(startOfRange);
            while (offsetIt->first < endOfRange) {
                joinedMap[prevBound + offsetIt->first - startOfRange] = offset + offsetIt->second;
                offsetIt++;
            }
            joinedMap[currBound] = offset + offsetIt->second;
        }
        flattenedMappings = std::move(joinedMap);
	}

	return flattenedMappings;
}