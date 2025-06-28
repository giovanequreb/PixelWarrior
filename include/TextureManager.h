#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>

class TextureManager {
private:
    std::map<std::string, SDL_Texture*> textures;
    SDL_Renderer* renderer;

public:
    TextureManager(SDL_Renderer* ren);
    ~TextureManager();
    bool loadTexture(const std::string& id, const std::string& filename);
    SDL_Texture* getTexture(const std::string& id);
    void cleanup();
};

#endif
