#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class Game {
public:
    Game();
    ~Game();

    bool Init();
    void Run();
    void CleanUp();

private:
    void LoadAssets();
    void HandleEvents();
    void Update();
    void Render();
    void PlayGame();
    void ShowReloadAnimation();
    void ShowResultScreen(bool win);
    void DrawChoice(int player, int opponent);

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Texture* texStart = nullptr;
    SDL_Texture* texExit = nullptr;
    SDL_Texture* texBG = nullptr;
    SDL_Texture* texRock = nullptr;
    SDL_Texture* texPaper = nullptr;
    SDL_Texture* texScissors = nullptr;
    SDL_Texture* texReload1 = nullptr;
    SDL_Texture* texReload2 = nullptr;
    SDL_Texture* texHit = nullptr;
    SDL_Texture* texMiss = nullptr;
    SDL_Texture* texWin = nullptr;
    SDL_Texture* texLose = nullptr;

    SDL_Rect startRect;
    SDL_Rect exitRect;

    bool running = false;
    bool showMenu = true;
    bool inGame = false;
    int playerChoice = -1;
    int opponentChoice = -1;

    std::vector<int> chamber;
};
