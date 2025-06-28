#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <SDL.h>
#include <vector>
#include "Camera.h"

class ParticleSystem {
private:
    struct Particle {
        float x, y, velX, velY;
        int life, maxLife;
        Uint8 r, g, b, a;
        SDL_Texture* texture = nullptr;
        int width, height;
        bool flip;
    };
    std::vector<Particle> particles;

public:
    ParticleSystem();
    void addMuzzleFlash(float x, float y);
    void addShotFX(float x, float y, SDL_Texture* tex, bool isFacingRight);
    void update();
    void render(SDL_Renderer* renderer, const Camera* camera);
};
#endif
