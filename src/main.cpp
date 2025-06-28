#include "../include/Game.h"
#include <iostream>

int main(int, char**) {
    Game game;
    if (game.initialize()) {
        game.run();
    } else {
        std::cerr << "Inizializzazione del gioco fallita!" << std::endl;
        return 1;
    }
    return 0;
}
