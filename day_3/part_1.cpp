#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include "schematic.hpp"

uint32_t isSymbolAdjacent(const Schematic& schematic, const Schematic::Position& pos);

int main() {
    std::vector<std::string> input;
    std::string line;
    while (std::getline(std::cin, line)) {
        input.push_back(std::move(line));
    }

    const Schematic schematic(std::move(input));
    uint32_t sumOfEngineParts = 0;
    for (size_t y = 0; y < schematic.size().y; ++y) {
        uint32_t enginePart = 0;
        bool hasAdjacentSymbol = false;
        auto processEnginePart = [&]() {
            sumOfEngineParts += enginePart * hasAdjacentSymbol;
            enginePart = 0;
            hasAdjacentSymbol = false;
        };
        for (size_t x = 0; x < schematic.size().x; ++x) {
            Schematic::Position pos{x, y};
            if (std::isdigit(schematic[pos])) {
                enginePart *= 10;
                enginePart += schematic[pos] - '0';
                if (isSymbolAdjacent(schematic, pos)) {
                    hasAdjacentSymbol = true;
                }
            }
            else {
                processEnginePart();
            }
        }
        processEnginePart();
    }

    std::cout << sumOfEngineParts << std::endl;
}

uint32_t isSymbolAdjacent(const Schematic& schematic, const Schematic::Position& pos) {
    auto isSymbol = [](char c) { return c != '.' && !std::isdigit(c); };
    for (const auto& adjacentPos : schematic.AdjacentPositions(pos)) {
        if (isSymbol(schematic[adjacentPos])) {
            return true;
        }
    }
    return false;
}