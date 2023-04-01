#include "Game.hpp"

int main() {

    bol::Game game {};

    game.setup();
    game.mainLoop();

    return 0;
}