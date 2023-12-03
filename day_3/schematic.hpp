#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>

// There are several functions in part 1 and 2 that would've fit well into the Schematic class, but I tried keeping the "business logic" of each of the puzzles separate, only keeping generic utility functions in the Schematic class itself
class Schematic {
public:
    // Simply using a std::pair<size_t, size_t> could be better than a named struct, but I wanted it to abstract away the order of indexing from the user by explicitely naming the fields
    struct Position {
        size_t x;
        size_t y;

        auto operator<=>(const Position& other) const = default;
    };

    Schematic(const std::vector<std::string>& schematic) : mSchematic(schematic) {}
    Schematic(std::vector<std::string>&& schematic) : mSchematic(std::move(schematic)) {}

    Position size() const { return mSize; }
    char operator[](const Position& pos) const { return mSchematic[pos.y][pos.x]; }

    std::vector<Position> AdjacentPositions(const Position& pos) const;
    bool IsInBounds(const Position& pos) const;

private:
    const std::vector<std::string> mSchematic;
    const Position mSize{mSchematic[0].size(), mSchematic.size()};
};