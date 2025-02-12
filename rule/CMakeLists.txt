add_library(rule
        src/Game.cpp
        src/Card.cpp
        src/Deck.cpp
        src/Player.cpp
        src/JokerCard.cpp
        src/NormalCard.cpp
)

target_include_directories(rule
        PUBLIC
        include
)
