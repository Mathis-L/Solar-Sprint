#include "Texte.h"
#include <iostream>
#include <stdexcept>


Text::Text(SDL_Renderer* renderer, const std::string& fontPath, int fontSize, int x, int y, const std::string& message, SDL_Color color)
    : renderer(renderer), message(message), color(color) {

    // Load font
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (font == nullptr) {
        // Handle error if font loading fails
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        throw std::runtime_error("Failed to load font");
    }

    // Render text surface
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (surface == nullptr) {
        // Handle error if text rendering fails
        std::cerr << "Error rendering text: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        throw std::runtime_error("Failed to render text");
    }

    // Create texture from surface
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        // Handle error if texture creation fails
        std::cerr << "Error creating texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        throw std::runtime_error("Failed to create texture");
    }

    // Set up rectangle for rendering
    rect.x = x- (surface->w/2);
    rect.y = y- (surface->h/2);
    rect.w = surface->w;
    rect.h = surface->h;

    // Free surface
    SDL_FreeSurface(surface);
}

void Text::changeText(const std::string& newMessage){
    message = newMessage;

    // Free previous texture
    SDL_DestroyTexture(texture);

    // Render new text surface
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (surface == nullptr) {
        // Handle error if text rendering fails
        std::cerr << "Error rendering text: " << TTF_GetError() << std::endl;
        return;
    }

    // Create new texture from surface
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        // Handle error if texture creation fails
        std::cerr << "Error creating texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Set up rectangle for rendering
    rect.w = surface->w;
    rect.h = surface->h;

    // Free surface
    SDL_FreeSurface(surface);
}


Text::~Text() {
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void Text::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

bool Text::initSDLTTF() {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        // Handle error if SDL_ttf initialization fails
        std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void Text::quitSDLTTF() {
    TTF_Quit();
}
