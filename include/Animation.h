#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <SDL.h>

class Animation {
private:
    std::vector<SDL_Texture*> frames;
    size_t currentFrame; // Cambiato da int a size_t per risolvere il warning
    int frameDelay;
    int frameTimer;
    bool shouldLoop;
    bool finished;

public:
    Animation();
    void setFrames(const std::vector<SDL_Texture*>& newFrames, int delay, bool loop);
    void update();
    void reset();
    SDL_Texture* getCurrentFrameTexture() const;
    bool isFinished() const;
    bool isLooping() const;

    // ---> NUOVA FUNZIONE RICHIESTA DA ENEMY.CPP <---
    size_t getCurrentFrameIndex() const; 
};

#endif // ANIMATION_H
