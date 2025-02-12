//
// Created by jintae on 25. 2. 12.
//

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include "Card.hpp"
#include "Deck.hpp"
#include "Player.hpp"

class Game {
private:
    Deck deck;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Card> dummyCard;
    bool reverseOrder = false;
    int attackStack = 0;
    std::vector<std::shared_ptr<Card>> usedAttackCards;  // 이미 사용된 공격 카드 추적

public:
    Game();
    void start();
};

#endif // GAME_HPP

