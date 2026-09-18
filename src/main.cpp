#include "Game.hpp"
#include <ctime>
#include <cstdlib>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    Game game;
    game.run();
    return 0;
}
