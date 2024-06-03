#ifndef COIN_H
#define COIN_H

#include "SDL2/SDL.h"

class Coin {
public:
    Coin();

    void initPos(int width, int floor);
    void update(int maxX);
    void render(SDL_Renderer* renderer);
    void reset(int maxX);
    SDL_Rect get_rect() const;

private:
    static constexpr int SIZE = 25;
    static constexpr int SPEED = 5;

    int posX;
    int posY;
    SDL_Rect rect;
};

#endif