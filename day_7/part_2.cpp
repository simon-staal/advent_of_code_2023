#include <iostream>
#include <string>
#include <cstdint>
#include <map>
#include <cassert>
#include <ranges>
#include <unordered_map>

bool isHandWorse(const std::string& hand, const std::string& other);

int main() {
	std::string hand;
	uint32_t bet;
	std::map<std::string, uint32_t, decltype(&isHandWorse)> hands{isHandWorse};

	while (std::cin >> hand >> bet) {
		assert(hand.size() == 5);
		hands.emplace(std::move(hand), bet);
	}

	uint32_t totalWinnings = 0;
	size_t rank = hands.size(); // Still waiting on std::views::enumerate ;(
	for (const auto& [_, bet] : hands | std::views::reverse) {
		totalWinnings += bet * rank;
		rank--;
	}

    std::cout << totalWinnings << std::endl;
}

enum class HandTypes {
	HighCard = 0,
	OnePair = 1,
	TwoPair = 2,
	ThreeOfAKind = 3,
	FullHouse = 4,
	FourOfAKind = 5,
	FiveOfAKind = 6
};

HandTypes getHandType(const std::string& hand) {
	std::unordered_map<char, uint32_t> cardCounts;
	for (char card : hand) {
		cardCounts[card]++;
	}

    auto jokersNode = cardCounts.extract('J');
    uint32_t jokers = jokersNode.empty() ? 0 : jokersNode.mapped();

    switch (cardCounts.size()) {
        case 0:
        case 1:
            return HandTypes::FiveOfAKind;
        case 2:
            if (std::views::filter(cardCounts, 
                    [jokers](const auto& pair) { return pair.second + jokers == 4; }).empty()) 
                return HandTypes::FullHouse;
            else
                return HandTypes::FourOfAKind;
        case 3:
            if (std::views::filter(cardCounts, 
                    [jokers](const auto& pair) { return pair.second + jokers == 3; }).empty()) 
                return HandTypes::TwoPair;
            else
                return HandTypes::ThreeOfAKind;
        case 4:
            return HandTypes::OnePair;
        case 5:
            return HandTypes::HighCard;
	}
    assert(false);
}

int32_t getCardValue(char card) {
	static const std::unordered_map<char, uint32_t> kPictureValues = {
		{'A', 14},
		{'K', 13},
		{'Q', 12},
		{'J', -1},
        {'T', 10}
	};

	if (std::isdigit(card)) {
		return card - '0';
	} else {
		return kPictureValues.at(card);
	}
}

bool isHandWorse(const std::string& hand, const std::string& other) {
	if (getHandType(hand) != getHandType(other)) {
		return getHandType(hand) < getHandType(other);
	}

	for (size_t i = 0; i < hand.size(); i++) {
		if (getCardValue(hand[i]) != getCardValue(other[i])) {
			return getCardValue(hand[i]) < getCardValue(other[i]);
		}
	}
	assert(false);
}