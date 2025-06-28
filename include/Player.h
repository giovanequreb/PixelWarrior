#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <map>
#include <SDL.h>
#include "Animation.h"
#include "Camera.h"
#include "Level.h"

class Player {
public:
    enum State { IDLE, WALKING, JUMPING, FALLING, SHOOT_STAND, SHOOT_WALK, HURT, DEAD };
    State currentState;

private:
    float x, y;
    float velX, velY;
    int width, height;

    bool onGround;
    bool facingRight;

    // Parametri di Fisica Migliorati
    float speed;
    float jumpForce;
    float jumpHoldForce;
    float gravity;
    float fallMultiplier;
    int jumpHoldDuration;
    int jumpHoldTimer;

    std::map<State, Animation> animations;
    Animation* currentAnimation;

    int health;
    int maxHealth;

    bool invincible;
    int invincibilityTimer;

    bool wantsToFireThisFrame;
    int shootCooldown;
    int shootCooldownTimer;

public:
    Player(float startX, float startY, int scale);

    void setAnimation(State state, const std::vector<SDL_Texture*>& frames, int delay, bool shouldLoop = true);
    void setDefaultAnimation(); // Imposta l'animazione iniziale per risolvere il bug

    void handleInput(const Uint8* keystate);
    void update(Level* level);
    void render(SDL_Renderer* renderer, const Camera* camera);

    bool wantsToFire() const;
    void takeDamage(int amount);

    // Getters
    int getHealth() const;
    int getMaxHealth() const;
    SDL_Rect getBounds() const;
    float getX() const;
    float getY() const;
    float getCenterX() const;
    float getCenterY() const;
    bool isFacingRight() const;
    bool isDead() const;
};

#endif
