#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL2/SDL.h"

class Obstacle {
public:
    Obstacle();

    void initPos(int width, int floor);
    void update(int maxX);
    void render(SDL_Renderer* renderer);
    SDL_Rect get_rect() const;

private:
    static constexpr int OBSTACLE_WIDTH = 25;
    static constexpr int OBSTACLE_HEIGHT = 50;
    static constexpr int OBSTACLE_SPEED = 5;

    int obstacle_posX;
    int obstacle_posY;
    SDL_Rect rect;
};

#endif