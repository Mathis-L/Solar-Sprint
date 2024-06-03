#include "Game.h"
#include <iostream>
#include <algorithm> // for std::max_element
#include <stdexcept> // for std::runtime_error

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Game::Game() 
    : isRunning(false), window(nullptr), score(0), gameover(false),
      difficulty(false), light(false), light_start(0), last_RectangleX(0) {}

int findMaxRectX(const std::vector<Obstacle>& obstacles){
    if (obstacles.empty()) {
        throw std::runtime_error("No obstacles available");
    }

    auto maxIt = std::max_element(obstacles.begin(), obstacles.end(),
                                  [](const Obstacle& a, const Obstacle& b) {
                                      return a.get_rect().x < b.get_rect().x;
                                  });

    return maxIt->get_rect().x;
}

void Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        isRunning = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        isRunning = false;
        return;
    }

    if (!Text::initSDLTTF()) {
    // Handle SDL_ttf initialization error
    isRunning = false;
    return;
    }   

    // Show the waiting screen
    showWaitingScreen();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    isRunning = true;

    // Initialize game objects
    player.initPos(WIDTH, GROUND_SIZE);
    coin.initPos(WIDTH, GROUND_SIZE);

    // Create Obstacles
    for (int i = 0; i < n_obstacle; ++i) {
        obstacles.emplace_back(); // Directly construct objects in the vector
        obstacles[i].initPos(WIDTH + i * (250 + rand() % 200), GROUND_SIZE);
    }
    last_RectangleX = findMaxRectX(obstacles);
}

void Game::showWaitingScreen() {
    SDL_Surface* surface = SDL_LoadBMP("res/logo.bmp");
    if (!surface) {
        std::cerr << "Error loading BMP: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
        return;
    }

    // Calculate the position to center the image
    const int imgWidth = 248;
    const int imgHeight = 248;
    const int posX = (WIDTH - imgWidth) / 2;
    const int posY = (HEIGHT - imgHeight) / 2;
    SDL_Rect destRect = { posX, posY, imgWidth, imgHeight };

    bool waiting = true;
    while (waiting) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                waiting = false;
            }
        }
    }

    SDL_DestroyTexture(texture);
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && gameover) {
            reset();
        }
        player.handleInput(event);
    }
}

void Game::update() {
    if (gameover) return;
    
    player.update(); //player moves
    last_RectangleX = findMaxRectX(obstacles);
    coin.update(last_RectangleX);

    for (auto& obstacle : obstacles) {
        obstacle.update(last_RectangleX); //obstacles moves
        if (player.checkCollision(obstacle.get_rect())){ //if collision 
            gameover = true;
            break;
        }
    }

    if (player.checkCollision(coin.get_rect())){
        score+=10;
        //std::cout << "Score: " << score << std::endl;
        coin.reset(last_RectangleX);
        if (score == 50){
        difficulty = true;
        }
        if (difficulty){
            light = true;
            light_start = SDL_GetTicks();
        }
    }

    if (difficulty && light){
        Uint32 currentTime = SDL_GetTicks();
        light_used = currentTime - light_start;
        if (light_used > light_time){
            light = false;
        }
    }
}


void Game::render() {
    if (difficulty && !light){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    } 
    else{
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        for (auto& obstacle : obstacles) {
            obstacle.render(renderer);
        }
    }

    coin.render(renderer);
    player.render(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, 0, GROUND_SIZE, WIDTH, GROUND_SIZE); //draw the ground line

    if (difficulty){
        renderLightTimer();
    }

    std::string scoreString = std::to_string(score);
    std::string scoreMesssage = "score: "+ scoreString;
    Text textScore(renderer, "res/arial.ttf", 20, 50 , 25, scoreMesssage, {255, 0, 0});
    textScore.render();

    if (gameover) {
        for (auto& obstacle : obstacles) {
            obstacle.render(renderer);
        }

        Text textGameOver(renderer, "res/arial.ttf", 20, WIDTH/ 2, HEIGHT / 2, "Game Over", {255, 0, 0});
        Text textPressEnter(renderer, "res/arial.ttf", 10, WIDTH/ 2, HEIGHT / 2 + 40, "Press Enter", {255, 0, 0});

        // Render "Game Over" and "Press Enter" texts
        textGameOver.render();
        textPressEnter.render();
        
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

void Game::renderLightTimer() {
    // Calculate the proportion of light time elapsed
    Uint32 light_remaining = light_time - light_used;
    float proportion = static_cast<float>(light_remaining) / light_time;

    // Define the dimensions and position of the progress bar
    const int barWidth = 200; // Width of the progress bar
    const int barHeight = 20; // Height of the progress bar
    const int barPosX = WIDTH - barWidth - 10 ; // Bar in the right top corner with some padding
    const int barPosY = 10; // Position the bar at the bottom 

    // Draw the background of the progress bar
    SDL_Rect bgRect = { barPosX, barPosY, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color inside
    SDL_RenderFillRect(renderer, &bgRect);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Gray color contour
    SDL_RenderDrawRect(renderer, &bgRect);

    if (light){
        // Draw the foreground of the progress bar
        SDL_Rect fgRect = { barPosX + 1, barPosY + 1, static_cast<int>((barWidth-2) * proportion), barHeight-2 };
        SDL_SetRenderDrawColor(renderer,255, 255, 0, 175); // Yellow
        SDL_RenderFillRect(renderer, &fgRect);
    }
    
}



void Game::reset() {
    score = 0;
    gameover = false;
    difficulty = false;
    obstacles.clear();

    coin.initPos(WIDTH, GROUND_SIZE);

    //create Obstacles
    for (int i = 0; i < n_obstacle; ++i) {
        obstacles.emplace_back(); // Directly construct objects in the vector
        obstacles[i].initPos(WIDTH + i * (250 + rand() % (200)), GROUND_SIZE);
    }
    last_RectangleX = findMaxRectX(obstacles);

}

Game::~Game(){
    clean();
}

void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Quit SDL_ttf
    Text::quitSDLTTF();
    SDL_Quit();
}
