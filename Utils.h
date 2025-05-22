#pragma once
#include <SDL.h>
#include <vector>

bool IsClicked(SDL_Rect& rect, int x, int y);
bool FireBullet(std::vector<int>& chamber);
int GetRPS();
int CompareRPS(int p1, int p2);
