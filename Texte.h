#ifndef TEXTE_H
#define TEXTE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string>

class Text {
public:
    Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, int x, int y, const std::string& message, SDL_Color color);
    ~Text();
    void render();

    void changeText(const std::string& newMssage);

    static bool initSDLTTF();
    static void quitSDLTTF();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* texture;
    SDL_Rect rect;
    std::string message;
    SDL_Color color;
};

#endif