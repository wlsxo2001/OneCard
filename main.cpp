
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>

class Card {
public:
    virtual void display() const = 0;
    virtual std::string getType() const = 0; // 숫자인지 조커인지.
    virtual std::string getValue() const = 0;
    virtual std::string getShape() const = 0;
    virtual std::string getFullInfo() const = 0;
    virtual int getAttackPower() const { return 0; }
    virtual bool canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const { return false; }
    virtual bool canPlayJoker(const std::string& currentShape) const { return false; }
    virtual ~Card() {}
};

// 일반 카드
class NormalCard : public Card {
private:
    std::string color;
    std::string shape;
    std::string value;
public:
    NormalCard(std::string c, std::string s, std::string v) : color(c), shape(s), value(v) {}
    void display() const override {
        std::cout << color << " " << shape << " " << value;
    }
    std::string getType() const override {
        return "Normal";
    }
    std::string getValue() const override {
        return value;
    }
    std::string getShape() const override {
        return shape;
    }
    std::string getFullInfo() const override {
        return color + " " + shape + " " + value;
    }
    // 공격
    int getAttackPower() const override {
        if (value == "2") return 2;
        if (value == "A") return 3;
        return 0;
    }
    // 공격 대처
    bool canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const override {
        return ((attackCard->getValue() == "2" || attackCard->getValue() == "A" || attackCard->getValue() == "3") && dummyCard->getShape() == attackCard->getShape());
    }
};

class JokerCard : public Card {
private:
    std::string type;
public:
    JokerCard(std::string t) : type(t) {}

    void display() const override {
        std::cout << type << " Joker";
    }

    std::string getType() const override { return "Joker"; }
    std::string getValue() const override { return type; }
    std::string getShape() const override { return "None"; }
    std::string getFullInfo() const override { return type + " Joker"; }
    // 공격
    int getAttackPower() const override {
        return (type == "Colored") ? 7 : (type == "Black & White") ? 5 : 0;
    }
    // 컬러(♦,♥)일때, 흑백(♠,♣) 구분해서 사용 가능한지 확인
    bool canPlayJoker(const std::string& currentShape) const override {
        return (type == "Colored" && (currentShape == "♦" || currentShape == "♥")) ||
               (type == "Black & White" && (currentShape == "♠" || currentShape == "♣"));
    }
};

// 덱 (모든카드 생성 후 저장)
class Deck {
private:
    std::vector<std::shared_ptr<Card>> cards;
public:
    Deck() {
        //std::string colors[] = {"Red", "Black"};
        std::string shapes[] = {"♠", "♦", "♣", "♥"};
        std::string values[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

        for (std::string shape : shapes)
        {
            std::string color = (shape == "♠" || shape == "♣") ? "Black" : "Red";  // ♠,♣ = black, ♦,♥ = red
            for (std::string value : values)
            {
                cards.push_back(std::make_shared<NormalCard>(color, shape, value));
            }
        }

        cards.push_back(std::make_shared<JokerCard>("Colored"));
        cards.push_back(std::make_shared<JokerCard>("Black & White"));

        shuffle();
    }
    void shuffle() { // 카드 섞기
        std::cout << "카드 섞는중 ..." << std::endl;
        srand(time(0));
        std::random_shuffle(cards.begin(), cards.end());
        std::cout << "deck 에 남은 카드 수: " << cards.size() << std::endl;
    }
    // 맨 위 카드 뽑고 덱에서 삭제
    std::shared_ptr<Card> draw() //std::shared_ptr<Player> currentPlayer, std::vector<std::shared_ptr<Player>>& players, int currentPlayerIndex, Deck& deck
    {
        if (cards.empty())
        {
            std::cout << "덱의 모든 카드가 소진되었습니다!" << std::endl;
            return nullptr;
        }
        auto card = cards.back();
        cards.pop_back();
        //std::cout << "deck 에 남은 카드 수: " << cards.size() << std::endl;
        return card;
        // if (cards.empty()) {
        //     // 게임 종료 체크
        //     if (currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck)) {
        //         return nullptr; // 게임이 종료되었으므로 더 이상 카드를 뽑지 않음
        //     }
        // }
        //
        // auto card = cards.back();
        // cards.pop_back();
        // return card;
    }
    bool isEmpty() const { // deck이 비었는지 확인
        return cards.empty();
    }
    int getSize() const
    {
        return cards.size();
    }
};

class Player {
protected:
    std::vector<std::shared_ptr<Card>> hand; // player 손에 든 패
    std::string name;
public:
    Player(std::string n) : name(n) {}
    void drawCard(std::shared_ptr<Card> card) { // 덱에서 카드 뽑기
        hand.push_back(card);
    }
    void showHand() {
        std::cout << name << "'s hand (" << hand.size() << " cards): ";
        for (size_t i = 0; i < hand.size(); i++) {
            if (i > 0) std::cout << " / ";
            hand[i]->display();
        }
        std::cout << std::endl;
    }
    std::string getName() { return name; }
    int getHandSize() { return hand.size(); }
    std::vector<std::shared_ptr<Card>> getHand() const { // hand를 그대로 반환.
        return hand;
    }
    // 냈던 카드 제거.
    void playCard(std::shared_ptr<Card> playedCard) {
        auto it = std::find(hand.begin(), hand.end(), playedCard); // hand에서 냈던 카드를 찾기
        if (it != hand.end()) {
            hand.erase(it);  // 카드를 찾으면 손에서 제거
        }
    }

    bool hasBothJokers() {
        bool hasColored = false, hasBlackWhite = false;
        for (auto &card : hand) {
            if (card->getValue() == "Colored") hasColored = true;
            if (card->getValue() == "Black & White") hasBlackWhite = true;
        }
        return hasColored && hasBlackWhite;
    }
    bool hasMore15() { return hand.size() >= 15; }
    // 카드 수가 0일 경우 승리 체크
    bool hasNoCards() { return hand.empty(); }

    // 카드 제출 가능한지 확인
    bool canPlayCard(const std::shared_ptr<Card>& card, const std::shared_ptr<Card>& dummyCard, bool isFirstCard) {
        if (isFirstCard) {
            // 첫 번째 카드는 value 또는 shape가 같으면 낼 수 있음
            return card->getValue() == dummyCard->getValue() || card->getShape() == dummyCard->getShape();
        } else {
            // 두 번째 이후 카드는 value가 같을 때만 낼 수 있음
            return card->getValue() == dummyCard->getValue();
        }
    }
    //종료조건 확인
    // bool checkGameOver(std::shared_ptr<Player> currentPlayer)
    // {
    //     if (currentPlayer->hasMore15() || currentPlayer->hasNoCards() || currentPlayer->hasBothJokers())
    //     {
    //         std::cout << "===================게임 종료===================" << std::endl;
    //         if (currentPlayer->hasMore15()) {
    //             std::cout << currentPlayer->getName() << "은 15장 이상의 카드를 보유하여 패배하였습니다!" << std::endl;
    //             std::cout << "=============================================" << std::endl;
    //             return currentPlayer->hasMore15();
    //         }
    //         if (currentPlayer->hasBothJokers()) {
    //             std::cout << currentPlayer->getName() << "은 두 개의 조커를 가지고 있어 패배하였습니다!" << std::endl;
    //             std::cout << "=============================================" << std::endl;
    //             return currentPlayer->hasBothJokers();
    //         }
    //         if (currentPlayer->hasNoCards()) {
    //             std::cout << currentPlayer->getName() << "가 모든 카드를 사용하여 승리하였습니다!" << std::endl;
    //             std::cout << "=============================================" << std::endl;
    //             return currentPlayer->hasNoCards();
    //         }//currentPlayerIndex = (currentPlayerIndex + 1) % players.size(); // 턴 넘기기
    //     }
    // }
    bool checkGameOver(std::shared_ptr<Player> currentPlayer, std::vector<std::shared_ptr<Player>>& players, int currentPlayerIndex, Deck& deck)
    {
        // 게임 종료 조건 확인
        if (currentPlayer->hasMore15() || currentPlayer->hasNoCards() || currentPlayer->hasBothJokers())
        {
            std::cout << "===================게임 종료===================" << std::endl;

            if (currentPlayer->hasMore15()) {
                std::cout << currentPlayer->getName() << "은 15장 이상의 카드를 보유하여 패배하였습니다!" << std::endl;
                int opponentIndex = (currentPlayerIndex + 1) % players.size();
                std::cout << players[opponentIndex]->getName() << "가 승리하였습니다!" << std::endl;
                std::cout << "=============================================" << std::endl;
                return true;
            }

            if (currentPlayer->hasBothJokers()) {
                std::cout << currentPlayer->getName() << "은 두 개의 조커를 가지고 있어 패배하였습니다!" << std::endl;
                int opponentIndex = (currentPlayerIndex + 1) % players.size();
                std::cout << players[opponentIndex]->getName() << "가 승리하였습니다!" << std::endl;
                std::cout << "=============================================" << std::endl;
                return true;
            }

            if (currentPlayer->hasNoCards()) {
                std::cout << currentPlayer->getName() << "가 모든 카드를 사용하여 승리하였습니다!" << std::endl;
                std::cout << "=============================================" << std::endl;
                return true;
            }
        }

        // 덱에 카드가 모두 소진되었는지 확인
        if (deck.isEmpty())
        {
            std::cout << "===================게임 종료===================" << std::endl;
            std::cout << "덱의 모든 카드가 소진되었습니다!" << std::endl;

            // 적은 카드를 가진 플레이어 찾기
            std::shared_ptr<Player> winner = players[0];
            for (const auto& player : players) {
                if (player->getHandSize() < winner->getHandSize()) {
                    winner = player;
                }
            }
            std::cout << winner->getName() << "가 더 적은 카드를 보유하여 승리하였습니다!" << std::endl;
            std::cout << "=============================================" << std::endl;
            return true;
        }
        return false;  // 게임이 종료되지 않았으면 false 반환
    }
};


class Game {
private:
    Deck deck;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Card> dummyCard;
    bool reverseOrder = false; // 카드를 내는 순서 반전(2명일땐 무의미)
    int attackStack = 0;
public:
    Game() {
        players.push_back(std::make_shared<Player>("player1"));
        players.push_back(std::make_shared<Player>("player2"));
        for (auto &player : players) {
            for (int i = 0; i < 5; i++) {
                player->drawCard(deck.draw()); // 5장 뽑기
            }
            player->showHand();
        }
        dummyCard = deck.draw();
    }

    void start() {
        std::cout << "게임 시작!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        size_t currentPlayerIndex = 0;
        int attackStack=0; // 공격 누적치
        std::vector<std::shared_ptr<Card>> usedAttackCards;  // 이미 사용된 공격 카드를 추적
        while (true)
        {
            std::cout << "현재 더미 카드: ";
            dummyCard->display();
            std::cout << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "deck 에 남은 카드 수: " << deck.getSize() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            bool defended = false;
            auto &currentPlayer = players[currentPlayerIndex];

            // 공격 카드일 경우 + 직전에 사용되었던 공격카드가 아닌경우
            if (dummyCard->getAttackPower() > 0 && std::find(usedAttackCards.begin(), usedAttackCards.end(), dummyCard) == usedAttackCards.end())
            {
                attackStack += dummyCard->getAttackPower();

                // 조커이면 무조건 먹음
                if (dummyCard->getType() == "Joker")
                {
                    std::cout << currentPlayer->getName() << "가 Joker 공격을 당해 " << attackStack << "장의 카드를 뽑습니다." << std::endl;
                    for (int i = 0; i < attackStack; i++)
                    {
                        // currentPlayer->drawCard(deck.draw());
                        // std::cout << currentPlayer->getName() << "가 덱에서 " << deck.draw()->getFullInfo() << "를 뽑았습니다." << std::endl;
                        auto drawnCard = deck.draw();  // 카드를 한 번만 뽑고
                        currentPlayer->drawCard(drawnCard);
                        std::cout << currentPlayer->getName() << "가 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                    }
                    currentPlayer->showHand(); // hand에 수정되는지 확인
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    if (currentPlayer->checkGameOver(currentPlayer,players, currentPlayerIndex, deck)) {return;} // 종료 조건 확인
                    attackStack=0;
                }

                else // 조커가 아니면
                {
                    // 플레이어가 공격에 대처할 수 있는 카드가 있는지 확인
                    for (const auto& card : currentPlayer->getHand()) {
                        if (card->canDefend(card,dummyCard)) {
                            defended = true;
                            currentPlayer->playCard(card);  // 대처 카드 내기
                            if (card->getAttackPower()>0) attackStack+=card->getAttackPower(); // 공격카드로 대처한 경우 (Stack 누적)
                            else attackStack=0; // 방어카드로 대처한 경우( Stack 초기화 )
                            dummyCard = card;  // 더미 카드 갱신
                            std::cout << currentPlayer->getName() << "가 " << card->getFullInfo() << "로 대응했습니다." << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            std::cout << "현재 더미 카드: ";
                            currentPlayer->showHand(); // hand에 수정되는지 확인
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            if (currentPlayer->checkGameOver(currentPlayer,players, currentPlayerIndex, deck)) {return;} // 종료 조건 확인
                            attackStack=0;
                            dummyCard->display();
                            std::cout << std::endl;
                            break;
                        }
                    }

                    if (!defended)
                    {
                        // 방어할 카드가 없으면 공격에 맞춰 카드 뽑기
                        std::cout << currentPlayer->getName() << "가 공격당해 " << attackStack << "장의 카드를 뽑습니다." << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        for (int i = 0; i < attackStack; i++) {
                            // currentPlayer->drawCard(deck.draw());
                            // std::cout << currentPlayer->getName() << "가 덱에서 " << deck.draw()->getFullInfo() << "를 뽑았습니다." << std::endl;
                            auto drawnCard = deck.draw();  // 카드를 한 번만 뽑고
                            currentPlayer->drawCard(drawnCard);
                            std::cout << currentPlayer->getName() << "가 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                        }
                        currentPlayer->showHand(); // hand에 수정되는지 확인
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        if (currentPlayer->checkGameOver(currentPlayer,players, currentPlayerIndex, deck)) {return;} // 종료 조건 확인
                        usedAttackCards.push_back(dummyCard); // 사용된 최근 공격카드는 따로 저장
                        attackStack=0; // 사용했으므로 Stack 초기화
                    }
                }
            }

            else // 공격 카드가 아닐 경우
            {
                bool played = false;
                int cnt = 0;  // 현재 턴에서 낸 카드 수

                do {
                    played = false;
                    for (const auto& card : currentPlayer->getHand()) {
                        if (currentPlayer->canPlayCard(card, dummyCard, cnt == 0)) {
                            std::cout << currentPlayer->getName() << "가 " << card->getFullInfo() << "를 냈습니다." << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            cnt++;
                            dummyCard = card;  // 더미 카드 갱신
                            played = true;
                            currentPlayer->playCard(card);  // 해당 카드를 손에서 제거
                            currentPlayer->showHand(); // hand에 수정되는지 확인
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            if (currentPlayer->checkGameOver(currentPlayer,players, currentPlayerIndex, deck)) {return;} // 종료 조건 확인
                            break;
                        }
                    }
                } while (played);  // 한 번이라도 카드를 낸 경우 반복

                if (cnt == 0) {
                    // 카드가 없으면 덱에서 한 장 뽑기
                    auto drawnCard = deck.draw();
                    currentPlayer->drawCard(drawnCard);
                    std::cout << currentPlayer->getName() << "가 낼 카드가 없어 한 장을 뽑습니다." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << currentPlayer->getName() << "가 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    currentPlayer->showHand(); // hand에 추가되는지 확인
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    if (currentPlayer->checkGameOver(currentPlayer,players, currentPlayerIndex, deck)) {return;} // 종료 조건 확인
                }
            }

            currentPlayerIndex = (currentPlayerIndex + 1) % players.size(); // 턴 넘기기
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main() {
    Game game;
    game.start();
    return 0;
}

//추가할것들
//deck 에서 모든 카드를 쓴 경우 처리
//joker 카드 canPlay 조건 처리
//J,Q,K 기능 추가