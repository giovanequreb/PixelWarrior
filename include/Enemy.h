#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <map>
#include <string>
#include "Animation.h"
#include "Camera.h"

class Enemy {
public:
    enum State { ALIVE, SHOOTING, DYING, DEAD };
    State state;

private:
    float x, y;
    float speed;
    int width, height;
    int shootCooldown;
    int shootInterval;
    std::map<State, Animation> animations;
    Animation* currentAnimation;
    bool hasFired;

public:
    Enemy(float startX, float startY, int scale);
    void setAnimation(State animState, const std::vector<SDL_Texture*>& frames, int delay, bool loop = true);
    void update();
    void die();
    void attemptShooting();
    bool wantsToFire();
    void render(SDL_Renderer* renderer, const Camera* camera);
    SDL_Rect getBounds() const;
    
    float getX() const;
    float getY() const;
    int getHeight() const;
};
#endif
