#include "../include/TextureManager.h"
#include <iostream>

TextureManager::TextureManager(SDL_Renderer* ren) : renderer(ren) {}

TextureManager::~TextureManager() {
    cleanup();
}

bool TextureManager::loadTexture(const std::string& id, const std::string& filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << filename << " - " << IMG_GetError() << std::endl;
        return false;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    textures[id] = texture;
    return true;
}

SDL_Texture* TextureManager::getTexture(const std::string& id) {
    auto it = textures.find(id);
    return (it != textures.end()) ? it->second : nullptr;
}

void TextureManager::cleanup() {
    for (auto& pair : textures) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    textures.clear();
}
