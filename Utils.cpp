#include "Utils.h"
#include <cstdlib>

bool IsClicked(SDL_Rect& rect, int x, int y) {
    return (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h);
}

bool FireBullet(std::vector<int>& chamber) {
    if (chamber.empty()) return false;
    int bullet = chamber.back();
    chamber.pop_back();
    return bullet == 1;
}

int GetRPS() {
    return rand() % 3;
}

int CompareRPS(int p1, int p2) {
    if (p1 == p2) return 0;
    if ((p1 == 0 && p2 == 2) || (p1 == 1 && p2 == 0) || (p1 == 2 && p2 == 1)) return 1;
    return 2;
}
