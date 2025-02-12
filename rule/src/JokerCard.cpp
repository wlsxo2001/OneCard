//
// Created by jintae on 25. 2. 12.
//
#include "JokerCard.hpp"
#include <iostream>
#include <string>

JokerCard::JokerCard(std::string t) : type(t) {}

void JokerCard::display() const {
    std::cout << type << " Joker";
}

std::string JokerCard::getType() const {
    return "Joker";
}

std::string JokerCard::getValue() const {
    return type;
}

std::string JokerCard::getShape() const {
    return "None";
}

std::string JokerCard::getFullInfo() const {
    return type + " Joker";
}

int JokerCard::getAttackPower() const {
    return (type == "Colored") ? 7 : (type == "Black & White") ? 5 : 0;
}

bool JokerCard::canPlayJoker(const std::string& currentShape) const {
    return (type == "Colored" && (currentShape == "♦" || currentShape == "♥")) ||
           (type == "Black & White" && (currentShape == "♠" || currentShape == "♣"));
}
