#ifndef PLAYER_H
#define PLAYER_H

#include "SDL2/SDL.h"

class Player {
public:
    Player();

    void initPos(int width, int floor);
    void handleInput(SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer);
    bool checkCollision(const SDL_Rect& otherRect);

private:
    static constexpr int PLAYER_SIZE = 50;
    static constexpr Uint32 JUMP_DURATION = 600; // Customize jump duration
    static constexpr int JUMP_HEIGHT = 150; // Customize jump height

    int player_posX;
    int player_posY;
    int initialY;
    bool isJumping;
    Uint32 jumpStartTime;
    SDL_Rect rect;
};

#endif