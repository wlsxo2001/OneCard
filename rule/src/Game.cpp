#include "Game.hpp"

Game::Game()
{
    std::cout << "게임 시작!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    players.push_back(std::make_shared<Player>("player1"));
    players.push_back(std::make_shared<Player>("player2"));

    for (auto &player : players)
    {
        for (int i = 0; i < 5; i++)
        {
            player->drawCard(deck.draw()); // 5장 뽑기
        }
        player->showHand();
    }
    dummyCard = deck.draw();
    do
    {
        if (dummyCard->getAttackPower() > 0)
        {
            std::cout << "더미의 첫 카드가 공격카드이므로 다시 뽑습니다." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            dummyCard = deck.draw();
        }
    }while (dummyCard->getAttackPower() != 0);
}


void Game::start()
{
    size_t currentPlayerIndex = 0;
    int attackStack = 0;

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

        // 공격 카드 처리
        if (dummyCard->getAttackPower() > 0 && std::find(usedAttackCards.begin(), usedAttackCards.end(), dummyCard) == usedAttackCards.end())
        {
            attackStack += dummyCard->getAttackPower();

            // 조커 공격 처리
            if (dummyCard->getType() == "Joker")
            {
                std::cout << currentPlayer->getName() << "가 Joker 공격을 당해 " << attackStack << "장의 카드를 뽑아야 합니다." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                for (int i = 0; i < attackStack; i++)
                {
                    auto drawnCard = deck.draw();
                    if (!drawnCard) // nullptr이면 덱이 비어있는 상태
                    {
                        currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck);
                        return; // 게임 종료
                    }
                    currentPlayer->drawCard(drawnCard);
                    std::cout << currentPlayer->getName() << "가 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                }
                currentPlayer->showHand();
                std::this_thread::sleep_for(std::chrono::seconds(1));
                if (currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck)) return;
                attackStack = 0;
                usedAttackCards.push_back(dummyCard);
            }
            else
            {
                // 방어 가능 여부 확인
                for (const auto& card : currentPlayer->getHand())
                {
                    if (card->canDefend(card, dummyCard)) // canDefend by Defense card or Attack card
                    {
                        defended = true;
                        currentPlayer->playCard(card);
                        attackStack = (card->getAttackPower() > 0) ? attackStack + card->getAttackPower() : 0;
                        dummyCard = card;
                        std::cout << currentPlayer->getName() << "가 " << card->getFullInfo() << "로 대응했습니다." << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        currentPlayer->showHand();
                        if (currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck)) return;
                        if ( card->getValue() == "3" ) attackStack = 0; // else make higher attackStack
                        dummyCard->display();
                        std::cout << std::endl;
                        break;
                    }
                }

                // 방어 실패 시 카드 뽑기
                if (!defended)
                {
                    std::cout << currentPlayer->getName() << "가 공격당해 " << attackStack << "장의 카드를 뽑습니다." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    for (int i = 0; i < attackStack; i++)
                    {
                        auto drawnCard = deck.draw();
                        if (!drawnCard) // nullptr이면 덱이 비어있는 상태
                        {
                            currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck);
                            return; // 게임 종료
                        }
                        currentPlayer->drawCard(drawnCard);
                        std::cout << currentPlayer->getName() << "가 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                    }
                    currentPlayer->showHand();
                    if (currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck)) return;
                    usedAttackCards.push_back(dummyCard);
                    attackStack = 0;
                }
            }
        }

        else
        {
            // 일반 카드 플레이
            bool played = false;
            int cnt = 0;
            int JQKcnt = 0;
            do {
                played = false;
                for (const auto& card : currentPlayer->getHand())
                {
                    if (currentPlayer->canPlayCard(card, dummyCard, cnt == 0))
                    {
                        // if (card->getAttackPower()>0)
                        // {
                        //     attackStack += card->getAttackPower();
                        // }
                        std::cout << currentPlayer->getName() << "가 " << card->getFullInfo() << "를 냈습니다." << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        cnt++;
                        dummyCard = card;
                        card->change7(dummyCard); // 7 이면 shape 변경 가능
                        played = true;
                        currentPlayer->playCard(card);
                        currentPlayer->showHand();
                        if (currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck)) return;
                        if (card->useJQK()) // repeat 1 turn if J,K  ff
                        {
                            std::cout << currentPlayer->getName() << "가 " << card->getValue() << "를 냈으므로 한턴 더 플레이 합니다." << std::endl;
                            JQKcnt++;
                        }
                        break;
                    }
                }
                if (JQKcnt > 0) break;
            } while (played);
            if (JQKcnt > 0) continue; // one more turn
            // 카드가 없으면 한 장 뽑기
            if (cnt == 0)
            {
                auto drawnCard = deck.draw();
                if (!drawnCard) // nullptr이면 덱이 비어있는 상태
                {
                    currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck);
                    return; // 게임 종료
                }
                currentPlayer->drawCard(drawnCard);
                std::cout << currentPlayer->getName() << "가 낼 카드가 없어 한 장을 뽑습니다." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << currentPlayer->getName() << "가 덱에서 " << drawnCard->getFullInfo() << "를 뽑았습니다." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                currentPlayer->showHand();
                if (currentPlayer->checkGameOver(currentPlayer, players, currentPlayerIndex, deck)) return;
            }
        }

        // 턴 넘기기
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}
