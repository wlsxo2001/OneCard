#include "Player.hpp"


Player::Player(std::string n) : name(n) {}

void Player::drawCard(std::shared_ptr<Card> card)
{
    hand.push_back(card);
}

void Player::showHand()
{
    std::cout << name << "'s hand (" << hand.size() << " cards): ";
    for (size_t i = 0; i < hand.size(); i++)
    {
        if (i > 0) std::cout << " / ";
        hand[i]->display();
    }
    std::cout << std::endl;
}

std::string Player::getName()
{
    return name;
}

int Player::getHandSize()
{
    //if (hand.size() == 1) std::cout << getName() << ": 원카드!" << std::endl;
    return hand.size();
}

std::vector<std::shared_ptr<Card>> Player::getHand() const
{
    return hand;
}

void Player::playCard(std::shared_ptr<Card> playedCard) //사용했던 카드를 hand에서 제거
{
    auto it = std::find(hand.begin(), hand.end(), playedCard);
    if (it != hand.end())
    {
        hand.erase(it);
    }
}

bool Player::hasBothJokers()
{
    bool hasColored = false, hasBlackWhite = false;
    for (auto &card : hand)
    {
        if (card->getValue() == "Colored") hasColored = true;
        if (card->getValue() == "Black & White") hasBlackWhite = true;
    }
    return hasColored && hasBlackWhite;
}

bool Player::hasMore15()
{
    return hand.size() >= 15;
}

bool Player::hasNoCards()
{
    return hand.empty();
}

bool Player::canPlayCard(const std::shared_ptr<Card>& card, const std::shared_ptr<Card>& dummyCard, bool isFirstCard)
{
    if (isFirstCard) // 턴 의처음에 는 shape, value 둘 중 하나만 같으면 됨. or canPlayJoker이거나
    {
        if (dummyCard->getType() == "Normal")
        {
            return card->getValue() == dummyCard->getValue() || card->getShape() == dummyCard->getShape() || card->canPlayJoker(dummyCard->getColor());
        }
        // if dummy is Joker, check only color
        else if (dummyCard->getType() == "Joker")
        {
            return (card->getColor()=="Red" && dummyCard->getColor()=="Colored") || (card->getColor()=="Black" && dummyCard->getColor()=="Black & White");
        }
    }
    else // 두번 째 이후의 턴에서는 value 가 같거나 canPlayJoker 인 경우에만 가능
    {
        return card->getValue() == dummyCard->getValue() || card->canPlayJoker(dummyCard->getColor());
    }
}

bool Player::checkGameOver(std::shared_ptr<Player> currentPlayer, std::vector<std::shared_ptr<Player>>& players, int currentPlayerIndex, Deck& deck)
{
    if (currentPlayer->hasMore15() || currentPlayer->hasNoCards() || currentPlayer->hasBothJokers())
    {
        std::cout << "===================게임 종료===================" << std::endl;

        if (currentPlayer->hasMore15())
        {
            std::cout << currentPlayer->getName() << "은 15장 이상의 카드를 보유하여 패배하였습니다!" << std::endl;
            int opponentIndex = (currentPlayerIndex + 1) % players.size();
            std::cout << players[opponentIndex]->getName() << "가 승리하였습니다!" << std::endl;
            std::cout << "=============================================" << std::endl;
            return true;
        }

        if (currentPlayer->hasBothJokers())
        {
            std::cout << currentPlayer->getName() << "은 두 개의 조커를 가지고 있어 패배하였습니다!" << std::endl;
            int opponentIndex = (currentPlayerIndex + 1) % players.size();
            std::cout << players[opponentIndex]->getName() << "가 승리하였습니다!" << std::endl;
            std::cout << "=============================================" << std::endl;
            return true;
        }

        if (currentPlayer->hasNoCards())
        {
            std::cout << currentPlayer->getName() << "가 모든 카드를 사용하여 승리하였습니다!" << std::endl;
            std::cout << "=============================================" << std::endl;
            return true;
        }
    }

    if (deck.isEmpty())
    {
        std::cout << "===================게임 종료===================" << std::endl;
        std::cout << "덱의 모든 카드가 소진되었습니다!" << std::endl;

        std::shared_ptr<Player> winner = players[0];
        for (const auto& player : players)
        {
            if (player->getHandSize() < winner->getHandSize())
            {
                winner = player;
            }
        }
        std::cout << winner->getName() << "가 더 적은 카드를 보유하여 승리하였습니다!" << std::endl;
        std::cout << "=============================================" << std::endl;
        return true;
    }
    return false;
}
