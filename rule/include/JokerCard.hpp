//
// Created by jintae on 25. 2. 12.
//

#ifndef JOKERCARD_HPP
#define JOKERCARD_HPP

#include "Card.hpp"

class JokerCard : public Card {
private:
    std::string type;
public:
    JokerCard(std::string t);
    void display() const override;
    std::string getType() const override;
    std::string getValue() const override;
    std::string getShape() const override;
    std::string getFullInfo() const override;
    int getAttackPower() const override;
    bool canPlayJoker(const std::string& currentShape) const override;
};

#endif //JOKERCARD_HPP
