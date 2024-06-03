#include "Player.h"
#include "Game.h"
#include <cmath> // For std::sin

Player::Player()
    : player_posX(0), player_posY(0), initialY(0), isJumping(false), jumpStartTime(0) {
    rect = {0, 0, PLAYER_SIZE, PLAYER_SIZE};
}

void Player::initPos(int width, int floor) {
    player_posX = width / 8;
    player_posY = floor - PLAYER_SIZE;
    rect = {player_posX, player_posY, PLAYER_SIZE, PLAYER_SIZE};
    initialY = player_posY;
}

void Player::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !isJumping) {
        isJumping = true;
        jumpStartTime = SDL_GetTicks();
    }
}

void Player::update() {
    if (isJumping) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - jumpStartTime;

        if (elapsedTime < JUMP_DURATION) {
            rect.y = initialY - JUMP_HEIGHT * sin((elapsedTime / static_cast<float>(JUMP_DURATION)) * M_PI);
        } else {
            rect.y = initialY;
            isJumping = false;
        }
    }
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool Player::checkCollision(const SDL_Rect& otherRect){
    return (SDL_HasIntersection(&rect, &otherRect));
}
