#include "game.hpp"

#include <sstream>
#include <string_view>

using namespace std::literals;

static const std::unordered_map<std::string, Cube> kStringToCube {
    {"red", Cube::RED},
    {"green", Cube::GREEN},
    {"blue", Cube::BLUE}
};

Game Game::FromString(const std::string& s) {
    std::stringstream ss{s};
    ss.ignore("Game "sv.size(), ' ');
    uint32_t id;
    ss >> id;
    ss.ignore(": "sv.size(), ' ');

    std::vector<Round> rounds;
    while (!ss.eof()) {
        Round round;
        for (bool endOfRound = false; !endOfRound;) {
            uint32_t n;
            std::string cube;
            ss >> n >> cube;
            endOfRound = cube.back() == ';';
            cube.pop_back();
            auto [_, res] = round.emplace(kStringToCube.at(cube), n);
            if (!res) {
                throw std::runtime_error("Duplicate cube colour processed within a single round (should never happen)");
            }
        }
        rounds.push_back(std::move(round));
    }

    return {id, std::move(rounds)};
}