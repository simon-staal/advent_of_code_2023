#include <iostream>
#include <string>
#include <cstdint>
#include <unordered_map>

#include "game.hpp"

std::unordered_map<Cube, uint32_t> getMinimumSet(const Game& g);
uint32_t getPower(const std::unordered_map<Cube, uint32_t> set);

int main() {
    std::string line;
    uint32_t sumOfPowers = 0;
    while (std::getline(std::cin, line)) {
        Game g {Game::FromString(line + ';')}; // Add a semi-colon to regularise input
        auto set {getMinimumSet(g)};
        sumOfPowers += getPower(set);
    }

    std::cout << sumOfPowers << std::endl;
}

std::unordered_map<Cube, uint32_t> getMinimumSet(const Game& g) {
    std::unordered_map<Cube, uint32_t> set;
    for (const auto& round : g.rounds) {
        for (const auto& [cube, count] : round) {
            set[cube] = std::max(set[cube], count);
        }
    }
    return set;
}

uint32_t getPower(const std::unordered_map<Cube, uint32_t> set) {
    uint32_t power = 1;
    for (const auto& [_, count] : set) {
        power *= count;
    }
    return power;
}