#include <iostream>
#include <string>
#include <cstdint>
#include <unordered_map>

#include "game.hpp"

bool isGamePossible(const Game& g);

int main() {
    std::string line;
    uint32_t sumOfImpossibleIds = 0;
    while (std::getline(std::cin, line)) {
        Game g {Game::FromString(line + ';')}; // Add a semi-colon to regularise input
        sumOfImpossibleIds += g.id * isGamePossible(g);
    }

    std::cout << sumOfImpossibleIds << std::endl;
}

bool isGamePossible(const Game& g) {
    static const std::unordered_map<Cube, uint32_t> bag {
        {Cube::RED, 12},
        {Cube::GREEN, 13},
        {Cube::BLUE, 14}
    };

    for (const auto& round : g.rounds) {
        for (const auto& [cube, count] : round) {
            if (count > bag.at(cube))
                return false;
        }
    }

    return true;
}