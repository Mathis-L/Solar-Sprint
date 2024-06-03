#ifndef GAME_H
#define GAME_H

#include "SDL2/SDL.h"
#include <vector>
#include "Texte.h"
#include "Player.h"
#include "Obstacle.h"
#include "Coin.h"

class Game {
public:
    Game();
    ~Game();

    void init(const char* title, int width, int height);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const { return isRunning; }

    static SDL_Renderer* renderer;
    static SDL_Event event;

private:
    const int WIDTH = 800, HEIGHT = 600;
    const int GROUND_SIZE = HEIGHT - 100;

    bool isRunning;
    bool difficulty;
    bool light;
    const Uint32 light_time = 1000;
    Uint32 light_start;
    Uint32 light_used;

    SDL_Window* window;
    TTF_Font* font;
    Player player;
    Coin coin;
    int n_obstacle = 4;
    std::vector<Obstacle> obstacles;
    int score;
    bool gameover;
    void reset();
    void renderLightTimer();
    void showWaitingScreen();

    int last_RectangleX;
};

#endif