#ifndef HUD_H
#define HUD_H

#include <SDL.h>
#include "TextureManager.h"

class HUD {
private:
    SDL_Renderer* renderer;
    TextureManager* texManager;

public:
    HUD(SDL_Renderer* ren, TextureManager* tm);
    void render(int playerHealth, int maxPlayerHealth);
};
#endif
