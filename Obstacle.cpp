#include "Obstacle.h"
#include <cstdlib> // For rand()


Obstacle::Obstacle()
    : obstacle_posX(0), obstacle_posY(0) {
    rect = {0, 0, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
}

void Obstacle::initPos(int x, int floor){
    obstacle_posX = x;
    obstacle_posY = floor - OBSTACLE_HEIGHT;
    rect= {obstacle_posX, obstacle_posY, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
}

void Obstacle::update(int maxX){
    rect.x -= OBSTACLE_SPEED;
    if (rect.x < -rect.w) {
        rect.x = maxX + 250 + rand() % (200);
    }
}

void Obstacle::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Obstacle::get_rect() const{
    return rect;
}

