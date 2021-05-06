#include "SDLspace.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

#include "painter.h"
using namespace std;

SDLspace::SDLspace(string title_, int width_, int height_) : title(title_), width(width_), height(height_) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(cout, "SDL_Init", true);
    if (TTF_Init() < 0)
        logSDLError(cout, "TTF_Init", true);
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        logSDLError(cout, "CreateWindow", true);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        logSDLError(cout, "CreateRenderer", true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, width, height);
}

SDLspace::~SDLspace() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void SDLspace::logSDLError(ostream& os, const string& msg, bool fatal) {
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void SDLspace::openFont(string f, int size) {
    font = TTF_OpenFont(f.c_str(), size);
    painter = new Painter(window, renderer, font);
}

void SDLspace::createTextTexture(string text, int x, int y) {
    SDL_Rect srcRest;
    SDL_Rect desRect;
    SDL_Texture* texture = NULL;
    texture = painter->textTexture(text, &srcRest, &desRect, x, y);
    SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
    SDL_DestroyTexture(texture);
}

void SDLspace::createImageBackground(string fileName) {
    SDL_Texture* texture = NULL;
    texture = painter->loadTexture("images\\" + fileName);
    painter->createImage(texture);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
}

void SDLspace::createImage(string fileName, int x, int y) {
    SDL_Rect srcRest;
    SDL_Rect desRect;
    SDL_Texture* texture = NULL;
    texture = painter->loadImage("images\\" + fileName, &srcRest, &desRect, x, y);
    SDL_RenderCopy(renderer, texture, &srcRest, &desRect);
    SDL_DestroyTexture(texture);
}

void SDLspace::updateScreen() {
    SDL_RenderPresent(renderer);
}
