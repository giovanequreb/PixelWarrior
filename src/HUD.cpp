#include "../include/HUD.h"
#include "../include/Constants.h"
#include <cmath>

HUD::HUD(SDL_Renderer* ren, TextureManager* tm)
    : renderer(ren), texManager(tm) {}

void HUD::render(int playerHealth, int maxPlayerHealth) {
    SDL_Texture* heartFull = texManager->getTexture("heart_full");
    SDL_Texture* heartEmpty = texManager->getTexture("heart_empty");

    if (!heartFull || !heartEmpty) return;
    
    int heartWidth = 16 * SCALE;
    int heartHeight = 16 * SCALE;
    int padding = 4 * SCALE;

    for (int i = 0; i < maxPlayerHealth; ++i) {
        SDL_Rect destRect = { 10 + i * (heartWidth + padding), 10, heartWidth, heartHeight };
        
        if (i < playerHealth) {
            SDL_RenderCopy(renderer, heartFull, NULL, &destRect);
        } else {
            SDL_RenderCopy(renderer, heartEmpty, NULL, &destRect);
        }
    }
}
