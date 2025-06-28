#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>
#include "Camera.h"
#include "Level.h" // Aggiungi questa inclusione

class Bullet {
private:
    float x, y;
    float velX, velY;
    bool active;
    int width, height;
    SDL_Texture* texture;

public:
    Bullet(int scale);
    void setTexture(SDL_Texture* tex);
    void fire(float startX, float startY, float dirX, float dirY);
    void update(Level* level); // Modifica qui
    void render(SDL_Renderer* renderer, const Camera* camera);
    bool isActive() const;
    void deactivate();
    SDL_Rect getBounds() const;
};
#endif
