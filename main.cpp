#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;

    if (!game.Init()) {
        SDL_Log("Failed to initialize game.");
        return -1;
    }


    game.Run();

    return 0;
}
