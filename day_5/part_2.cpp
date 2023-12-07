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
using SeedMappings = std::vector<std::map<uint64_t, int64_t>>;
using SeedRanges = std::vector<std::pair<uint64_t, uint64_t>>;

std::pair<SeedRanges, SeedMappings> parseInput();
std::map<uint64_t, int64_t> combineSeedMappings(const SeedMappings& seedMappings);

int main() {
	const auto [seeds, seedMappings] = parseInput();
	std::map<uint64_t, int64_t> seedLocationOffsets = combineSeedMappings(seedMappings);

	uint64_t minLocation = std::numeric_limits<uint64_t>::max();
	for (const auto& [seed, range] : seeds) {
		uint64_t seedStart = seed;
		auto offsetIt = seedLocationOffsets.upper_bound(seedStart);
		while (offsetIt->first <= seed + range) {
            const auto [nextBound, offset] = *offsetIt;
            // Only need to check first seed in range as seeds are monotonically increasing within range
			minLocation = std::min(minLocation, seedStart + offset);
			seedStart = nextBound;
			offsetIt++;
		}
	}

	std::cout << minLocation << std::endl;
}

std::pair<SeedRanges, SeedMappings> parseInput() {
	SeedRanges seeds;
	std::cin.ignore("seeds: "sv.size(), ' ');
	uint64_t seed, range;
	while (std::cin >> seed >> range) {
		seeds.emplace_back(seed, range);
	}

	SeedMappings mappings;
	while (!std::cin.eof()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore map header

		std::map<uint64_t, int64_t> currentMapping;
		uint64_t destinationStart, sourceStart, length;
		while (std::cin >> destinationStart >> sourceStart >> length) {			
			currentMapping.try_emplace(sourceStart, 0);
			currentMapping[sourceStart + length] = destinationStart - sourceStart;
		}
		currentMapping.emplace(std::numeric_limits<uint64_t>::max(), 0); // Ensure we always have an upper bound
		mappings.push_back(std::move(currentMapping));
	}

	return {std::move(seeds), std::move(mappings)};
}

std::map<uint64_t, int64_t> combineSeedMappings(const SeedMappings& seedMappings) {
	std::map<uint64_t, int64_t> combinedMappings {
        {0, 0},
        {std::numeric_limits<uint64_t>::max(), 0}
    };

	for (const auto& newMapping : seedMappings) {
        std::map<uint64_t, int64_t> joinedMap {{0, 0}};
        for (auto currIt = combinedMappings.cbegin(); currIt != combinedMappings.cend();) {
            auto prevIt = currIt;
            currIt++;
            if (currIt == combinedMappings.cend())
                break;
            
            const auto [rangeStart, _] = *prevIt;
            const auto [rangeEnd, cumulativeOffset] = *currIt;

            const uint64_t mappedRangeStart = rangeStart + cumulativeOffset;
            const uint64_t mappedRangeEnd = rangeEnd + cumulativeOffset;

            auto newOffsetIt = newMapping.upper_bound(mappedRangeStart);
            while (newOffsetIt->first < mappedRangeEnd) {
                const auto [newBound, newOffset] = *newOffsetIt;
                const uint64_t length = newBound - mappedRangeStart;
                joinedMap[rangeStart + length] = cumulativeOffset + newOffset;
                newOffsetIt++;
            }
            joinedMap[rangeEnd] = cumulativeOffset + newOffsetIt->second;
        }
        combinedMappings = std::move(joinedMap);
	}

	return combinedMappings;
}