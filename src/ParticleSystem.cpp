#include "../include/ParticleSystem.h"
#include "../include/Constants.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

ParticleSystem::ParticleSystem() {}

void ParticleSystem::addMuzzleFlash(float x, float y) {
    for (int i = 0; i < 10; ++i) {
        Particle p;
        p.x = x;
        p.y = y;
        p.velX = (rand() % 100 - 50) / 20.0f;
        p.velY = (rand() % 100 - 50) / 20.0f;
        p.life = p.maxLife = 5 + rand() % 5;
        p.r = 255;
        p.g = 255;
        p.b = 200;
        p.a = 255;
        particles.push_back(p);
    }
}

void ParticleSystem::addShotFX(float x, float y, SDL_Texture* tex, bool isFacingRight) {
    Particle p;
    p.x = x;
    p.y = y;
    p.texture = tex;
    p.life = 4;
    p.width = 16 * SCALE;
    p.height = 16 * SCALE;
    p.flip = !isFacingRight;
    p.velX = 0;
    p.velY = 0;
    particles.push_back(p);
}

void ParticleSystem::update() {
    for (auto it = particles.begin(); it != particles.end();) {
        it->x += it->velX;
        it->y += it->velY;
        it->life--;
        if (!it->texture) {
            it->a = (Uint8)((255.0f * it->life) / it->maxLife);
        }
        if (it->life <= 0) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }
}

void ParticleSystem::render(SDL_Renderer* renderer, const Camera* camera) {
    SDL_BlendMode oldBlendMode;
    SDL_GetRenderDrawBlendMode(renderer, &oldBlendMode);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    for (const auto& p : particles) {
        if(p.texture) {
            SDL_Rect dest = {(int)floor(p.x - camera->getX()), (int)floor(p.y - camera->getY()), p.width, p.height};
            SDL_RendererFlip flip = p.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            SDL_RenderCopyEx(renderer, p.texture, NULL, &dest, 0, NULL, flip);
        } else {
            SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, p.a);
            SDL_Rect rect = { (int)floor(p.x - camera->getX()), (int)floor(p.y - camera->getY()), 2, 2 };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_SetRenderDrawBlendMode(renderer, oldBlendMode);
}
