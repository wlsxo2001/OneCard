//
// Created by jintae on 25. 2. 12.
//
#include "NormalCard.hpp"

NormalCard::NormalCard(std::string c, std::string s, std::string v) : color(c), shape(s), value(v) {}

void NormalCard::display() const {
    std::cout << color << " " << shape << " " << value;
}

std::string NormalCard::getType() const {
    return "Normal";
}

std::string NormalCard::getValue() const {
    return value;
}

std::string NormalCard::getShape() const {
    return shape;
}

std::string NormalCard::getFullInfo() const {
    return color + " " + shape + " " + value;
}

int NormalCard::getAttackPower() const {
    if (value == "2") return 2;
    if (value == "A") return 3;
    return 0;
}

bool NormalCard::canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const {
    return ((attackCard->getValue() == "2" || attackCard->getValue() == "A" || attackCard->getValue() == "3")
            && dummyCard->getShape() == attackCard->getShape());
}
