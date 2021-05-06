#ifndef SDLSPACE_H_INCLUDED
#define SDLSPACE_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

#include "painter.h"
using namespace std;

class SDLspace {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Painter* painter;
    string title;
    int width;
    int height;

   public:
    SDLspace(string title_, int width_, int height_);
    ~SDLspace();
    void logSDLError(ostream& os, const string& msg, bool fatal);
    void openFont(string f, int size);
    void createTextTexture(string text, int x, int y);
    void createImageBackground(string fileName);
    void createImage(string fileName, int x, int y);
    void updateScreen();
};

#endif // SDLSPACE_H_INCLUDED
