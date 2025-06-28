#include "../include/Bullet.h"
#include "../include/Constants.h"
#include <cmath>

Bullet::Bullet(int scale)
    : x(0), y(0), velX(0), velY(0), active(false),
      width(10 * scale), height(10 * scale), 
      texture(nullptr) {}

void Bullet::setTexture(SDL_Texture* tex) {
    texture = tex;
}

void Bullet::fire(float startX, float startY, float dirX, float dirY) {
    x = startX;
    y = startY;
    velX = dirX * 10.0f * SCALE;
    velY = dirY * 10.0f * SCALE;
    active = true;
}

void Bullet::update(Level* level) {
    if (!active) return;

    x += velX;
    y += velY;

    if (level->isSolidTile((int)x, (int)y)) {
        active = false;
        return;
    }

    if (x < -width || x > level->getWidth() || y < -height || y > level->getHeight()) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer, const Camera* camera) {
    if (!active) return;
    SDL_Rect destRect = { (int)floor(x - camera->getX()), (int)floor(y - camera->getY()), width, height };
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}

bool Bullet::isActive() const { return active; }

void Bullet::deactivate() { active = false; }

SDL_Rect Bullet::getBounds() const {
    return { (int)x, (int)y, width, height };
}
