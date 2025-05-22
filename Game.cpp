#include "Game.h"
#include "Utils.h"
#include <algorithm>
#include <random>
#include <iostream>

Game::Game() {}
Game::~Game() { CleanUp(); }

bool Game::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;

    window = SDL_CreateWindow("Russian RPS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) return false;

    LoadAssets();
    showMenu = true;
    inGame = false;
    playerChoice = -1;
    opponentChoice = -1;
    return true;
}

void Game::LoadAssets() {
    texStart = IMG_LoadTexture(renderer, "assets/start.png");
    texExit = IMG_LoadTexture(renderer, "assets/exit.png");
    texBG = IMG_LoadTexture(renderer, "assets/background.png");
    texRock = IMG_LoadTexture(renderer, "assets/rock.png");
    texPaper = IMG_LoadTexture(renderer, "assets/paper.png");
    texScissors = IMG_LoadTexture(renderer, "assets/scissors.png");
    texReload1 = IMG_LoadTexture(renderer, "assets/reload1.png");
    texReload2 = IMG_LoadTexture(renderer, "assets/reload2.png");
    texHit = IMG_LoadTexture(renderer, "assets/hit.png");
    texMiss = IMG_LoadTexture(renderer, "assets/miss.png");
    texWin = IMG_LoadTexture(renderer, "assets/you_win.png");
    texLose = IMG_LoadTexture(renderer, "assets/you_lose.png");

    startRect = { 250, 200, 300, 100 };
    exitRect = { 250, 350, 300, 100 };
}

void Game::Run() {
    running = true;
    while (running) {
        HandleEvents();
        Update();
        Render();
        SDL_Delay(16);
    }
}

void Game::HandleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;

        if (showMenu && e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x, y = e.button.y;
            if (IsClicked(startRect, x, y)) {
                showMenu = false;
                inGame = true;
                ShowReloadAnimation();
                PlayGame();
            } else if (IsClicked(exitRect, x, y)) {
                running = false;
            }
        }

        if (inGame && e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_a) playerChoice = 0;
            else if (e.key.keysym.sym == SDLK_s) playerChoice = 1;
            else if (e.key.keysym.sym == SDLK_d) playerChoice = 2;
        }
    }
}

void Game::Update() {}

void Game::Render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texBG, nullptr, nullptr);

    if (showMenu) {
        SDL_RenderCopy(renderer, texStart, nullptr, &startRect);
        SDL_RenderCopy(renderer, texExit, nullptr, &exitRect);
    }

    SDL_RenderPresent(renderer);
}

void Game::PlayGame() {
    chamber = { 1, 0, 0, 0, 0, 0 };
    std::shuffle(chamber.begin(), chamber.end(), std::mt19937(std::random_device()()));

    ShowReloadAnimation();

    bool playing = true;
    size_t chamberIndex = 0;

    while (playing && running && chamberIndex < chamber.size()) {
        playerChoice = -1;
        opponentChoice = -1;
        while (playerChoice == -1 && running) {
            HandleEvents();
            SDL_Delay(16);
        }
        opponentChoice = GetRPS();
        int result = CompareRPS(playerChoice, opponentChoice);
        DrawChoice(playerChoice, opponentChoice);
        SDL_Delay(800);

        if (result == 0) continue;

        bool playerLoses = (result == 2);
        bool shot = chamber[chamberIndex++] == 1;

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texBG, nullptr, nullptr);
        SDL_RenderCopy(renderer, shot ? texHit : texMiss, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);

        if (shot) {
            ShowResultScreen(!playerLoses);
            playing = false;
        }
    }

    showMenu = true;
    inGame = false;
}

void Game::DrawChoice(int p1, int p2) {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texBG, nullptr, nullptr);

    SDL_Rect r1 = { 200, 400, 100, 100 };
    SDL_Rect r2 = { 500, 400, 100, 100 };

    SDL_Texture* t1 = (p1 == 0) ? texRock : (p1 == 1) ? texPaper : texScissors;
    SDL_Texture* t2 = (p2 == 0) ? texRock : (p2 == 1) ? texPaper : texScissors;

    SDL_RenderCopy(renderer, t1, nullptr, &r1);
    SDL_RenderCopy(renderer, t2, nullptr, &r2);
    SDL_RenderPresent(renderer);
}

void Game::ShowReloadAnimation() {
    for (int i = 0; i < 3 && running; ++i) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texBG, nullptr, nullptr);
        SDL_RenderCopy(renderer, (i % 2 == 0) ? texReload1 : texReload2, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_Delay(500);
    }
}

void Game::ShowResultScreen(bool win) {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texBG, nullptr, nullptr);
    SDL_RenderCopy(renderer, win ? texWin : texLose, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
}

void Game::CleanUp() {
    SDL_DestroyTexture(texStart);
    SDL_DestroyTexture(texExit);
    SDL_DestroyTexture(texBG);
    SDL_DestroyTexture(texRock);
    SDL_DestroyTexture(texPaper);
    SDL_DestroyTexture(texScissors);
    SDL_DestroyTexture(texReload1);
    SDL_DestroyTexture(texReload2);
    SDL_DestroyTexture(texHit);
    SDL_DestroyTexture(texMiss);
    SDL_DestroyTexture(texWin);
    SDL_DestroyTexture(texLose);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

