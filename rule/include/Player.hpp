//
// Created by jintae on 25. 2. 12.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <vector>
#include <memory>
#include "Card.hpp"
#include "Deck.hpp"

class Player {
protected:
    std::vector<std::shared_ptr<Card>> hand;
    std::string name;
public:
    Player(std::string n);

    void drawCard(std::shared_ptr<Card> card);
    void showHand();

    std::string getName();
    int getHandSize();
    std::vector<std::shared_ptr<Card>> getHand() const;

    void playCard(std::shared_ptr<Card> playedCard);

    bool hasBothJokers();
    bool hasMore15();
    bool hasNoCards();

    bool canPlayCard(const std::shared_ptr<Card>& card, const std::shared_ptr<Card>& dummyCard, bool isFirstCard);
    bool checkGameOver(std::shared_ptr<Player> currentPlayer, std::vector<std::shared_ptr<Player>>& players, int currentPlayerIndex, Deck& deck);
};

#endif // PLAYER_HPP

