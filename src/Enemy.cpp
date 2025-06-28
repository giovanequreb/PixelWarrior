#include "../include/Enemy.h"
#include "../include/Constants.h"
#include <cmath>
#include <cstdlib>

Enemy::Enemy(float startX, float startY, int scale)
    : state(ALIVE),
      x(startX), y(startY),
      speed(-1.5f * scale / 2.0f),
      width(16 * scale), height(16 * scale),
      shootCooldown(60 + rand() % 120),
      shootInterval(180 + rand() % 120),
      currentAnimation(nullptr),
      hasFired(false)
{}

void Enemy::setAnimation(State animState, const std::vector<SDL_Texture*>& frames, int delay, bool loop) {
    animations[animState].setFrames(frames, delay, loop);
    if (state == animState || !currentAnimation) {
        currentAnimation = &animations[animState];
    }
}

void Enemy::die() {
    if (state == ALIVE || state == SHOOTING) {
        state = DYING;
        speed = 0;
        currentAnimation = &animations[DYING];
        currentAnimation->reset();
    }
}

void Enemy::attemptShooting() {
    if (state == ALIVE && shootCooldown == 0) {
        state = SHOOTING;
        currentAnimation = &animations[SHOOTING];
        currentAnimation->reset();
        hasFired = false;
        shootCooldown = shootInterval;
    }
}

bool Enemy::wantsToFire() {
    if (state == SHOOTING && !hasFired && currentAnimation && currentAnimation->getCurrentFrameIndex() >= 4) {
        hasFired = true;
        return true;
    }
    return false;
}

void Enemy::update() {
    if (shootCooldown > 0 && state == ALIVE) {
        shootCooldown--;
    }

    switch (state) {
        case ALIVE:
            x += speed;
            if (x + width < 0) state = DEAD;
            break;
        case SHOOTING:
            if (currentAnimation && currentAnimation->isFinished()) {
                state = ALIVE;
                currentAnimation = &animations[ALIVE];
            }
            break;
        case DYING:
            if (currentAnimation && currentAnimation->isFinished()) {
                state = DEAD;
            }
            break;
        case DEAD:
            break;
    }
    if (currentAnimation) {
        currentAnimation->update();
    }
}

void Enemy::render(SDL_Renderer* renderer, const Camera* camera) {
    if (state == DEAD || !currentAnimation) return;
    SDL_Texture* tex = currentAnimation->getCurrentFrameTexture();
    if (!tex) return;
    SDL_Rect destRect = { 
        (int)floor(x - camera->getX()), 
        (int)floor(y - camera->getY()), 
        width, 
        height 
    };
    SDL_RenderCopy(renderer, tex, NULL, &destRect);
}

SDL_Rect Enemy::getBounds() const {
    return { (int)x, (int)y, width, height };
}

float Enemy::getX() const { return x; }
float Enemy::getY() const { return y; }
int Enemy::getHeight() const { return height; }
