#include "Coin.h"
#include <cstdlib> // For rand()

Coin::Coin()
    : posX(0), posY(0) {
    rect = {0, 0, SIZE, SIZE};
}

void Coin::initPos(int x, int floor){
    posX = x;
    posY = floor - SIZE - 100;
    rect= {posX, posY, SIZE, SIZE};
}

void Coin::update(int maxX){
    rect.x -= SPEED;
    if (rect.x < -rect.w) {
        rect.x = maxX;
        rect.y = 300 + rand() % 100;
    }
}

void Coin::reset(int maxX){
    rect.x = maxX;
    rect.y = 300 + rand() % 100;
}

void Coin::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 175);
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Coin::get_rect() const{
    return rect;
}

