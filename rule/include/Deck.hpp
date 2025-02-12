//
// Created by jintae on 25. 2. 12.
//

#ifndef DECK_HPP
#define DECK_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>
#include "Card.hpp"
#include "NormalCard.hpp"
#include "JokerCard.hpp"

class Deck {
private:
    std::vector<std::shared_ptr<Card>> cards;
public:
    Deck();

    void shuffle();
    std::shared_ptr<Card> draw();

    bool isEmpty() const;
    int getSize() const;
};

#endif // DECK_HPP

