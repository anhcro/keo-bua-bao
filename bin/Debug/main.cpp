#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

bool isInside(int x, int y, SDL_Rect rect) {
    return (x > rect.x && x < rect.x + rect.w &&
            y > rect.y && y < rect.y + rect.h);
}

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Failed to load image: " << IMG_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cout << "SDL or SDL_image Init failed: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL Split Images",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (!window) {
        std::cout << "Window creation failed: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* startTexture = loadTexture("Start.png", renderer);
    SDL_Texture* exitTexture = loadTexture("Exit.png", renderer);

    if (!startTexture || !exitTexture) {
        return -1;
    }

    int SCREEN_WIDTH, SCREEN_HEIGHT;
    SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    int imgW = 400; // Kích thước ảnh
    int imgH = 200;

    // Tính toán vị trí để 2 ảnh nằm hai bên, cách nhau đều, căn giữa theo chiều dọc
    SDL_Rect startRect = {
        SCREEN_WIDTH / 4 - imgW / 2,         // vị trí giữa nửa trái
        (SCREEN_HEIGHT - imgH) / 2,
        imgW,
        imgH
    };

    SDL_Rect exitRect = {
        3 * SCREEN_WIDTH / 4 - imgW / 2,     // vị trí giữa nửa phải
        (SCREEN_HEIGHT - imgH) / 2,
        imgW,
        imgH
    };

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (isInside(x, y, startRect) || isInside(x, y, exitRect)) {
                    quit = true;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, startTexture, nullptr, &startRect);
        SDL_RenderCopy(renderer, exitTexture, nullptr, &exitRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(exitTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
