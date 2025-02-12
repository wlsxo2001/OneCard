#include "Card.hpp"

int Card::getAttackPower() const {
    return 0;
}

bool Card::canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const {
    return false;
}

bool Card::canPlayJoker(const std::string& currentShape) const {
    return false;
}
