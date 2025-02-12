//
// Created by jintae on 25. 2. 12.
//

#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

class Card {
public:
    virtual void display() const = 0;
    virtual std::string getType() const = 0;
    virtual std::string getValue() const = 0;
    virtual std::string getShape() const = 0;
    virtual std::string getFullInfo() const = 0;
    virtual int getAttackPower() const;
    virtual bool canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const;
    virtual bool canPlayJoker(const std::string& currentShape) const;
    virtual ~Card() {}
};

#endif // CARD_HPP

