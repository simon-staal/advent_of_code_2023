#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

#include "schematic.hpp"

// Using std::map instead of std::unordered_map as I cba defining a hash for Position (and we're dealing with a very small set of parts)
std::map<Schematic::Position, uint32_t> getAdjacentParts(const Schematic& s, const Schematic::Position& pos);
uint32_t getGearRatio(const std::map<Schematic::Position, uint32_t>& parts);

int main() {
    std::vector<std::string> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.push_back(std::move(line));
    }

    const Schematic schematic(std::move(input));
    uint32_t sumOfGearRatios = 0;
    for (size_t y = 0; y < schematic.size().y; ++y) {
        for (size_t x = 0; x < schematic.size().x; ++x) {
            Schematic::Position pos{x, y};
            if (schematic[pos] == '*') {
                auto parts = getAdjacentParts(schematic, pos);
                sumOfGearRatios += getGearRatio(parts) * (parts.size() == 2);
            }
        }
    }

    std::cout << sumOfGearRatios << std::endl;
}

std::map<Schematic::Position, uint32_t> getAdjacentParts(const Schematic& s, const Schematic::Position& pos) {
    auto getLeadingDigit = [&s](Schematic::Position pos) {
        while (s.IsInBounds(pos) && std::isdigit(s[pos])) { 
            pos = {pos.x - 1, pos.y};
        }
        return Schematic::Position{pos.x + 1, pos.y};
    };

    auto getPartNumber = [&s](const Schematic::Position& start) {
        uint32_t partNumber = 0;
        auto pos = start;
        while (s.IsInBounds(pos) && std::isdigit(s[pos])) {
            partNumber *= 10;
            partNumber += s[pos] - '0';
            pos = {pos.x + 1, pos.y};
        }
        return partNumber;
    };

    std::map<Schematic::Position, uint32_t> parts;
    for (const auto& adjacent : s.AdjacentPositions(pos)) {
        if (std::isdigit(s[adjacent])) {
            auto startOfPart = getLeadingDigit(adjacent);
            parts.emplace(startOfPart, getPartNumber(startOfPart));
        }
    }

    return parts;
}

uint32_t getGearRatio(const std::map<Schematic::Position, uint32_t>& parts) {
    uint32_t gearRatio = 1;
    for (const auto& [_, partNumber] : parts) {
        gearRatio *= partNumber;
    }
    return gearRatio;
}