#include "schematic.hpp"

std::vector<Schematic::Position> Schematic::AdjacentPositions(const Position& pos) const {
    std::vector<Position> adjacentPositions;
    for (int32_t y = -1; y <= 1; ++y) {
        for (int32_t x = -1; x <= 1; ++x) {
            Position candidate{pos.x + x, pos.y + y};
            if (IsInBounds(candidate) && candidate != pos) {
                adjacentPositions.push_back(std::move(candidate));
            }
        }
    }
    return adjacentPositions;
}

bool Schematic::IsInBounds(const Position& pos) const {
    return pos.x < mSize.x && pos.y < mSize.y; 
}