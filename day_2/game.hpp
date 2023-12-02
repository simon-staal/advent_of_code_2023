#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <string>

enum class Cube {
    RED,
    GREEN,
    BLUE
};

struct Game {
    using Round = std::unordered_map<Cube, uint32_t>;

    const uint32_t id;
    const std::vector<Round> rounds;

    static Game FromString(const std::string& s);
};