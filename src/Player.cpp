#include "Player.h"
#include "Constants.h"
#include <iostream>

Player::Player(float startX, float startY, int scale)
    : currentState(IDLE),
      x(startX), y(startY),
      velX(0), velY(0),
      width(16 * scale), height(16 * scale),
      onGround(false), facingRight(true),
      speed(4.5f),
      jumpForce(-8.5f),
      jumpHoldForce(-0.4f),
      gravity(0.4f),
      fallMultiplier(1.8f),
      jumpHoldDuration(12),
      jumpHoldTimer(0),
      currentAnimation(nullptr), health(3), maxHealth(3),
      invincible(false), invincibilityTimer(0),
      wantsToFireThisFrame(false),
      shootCooldown(15), shootCooldownTimer(0)
{
}

void Player::setAnimation(State state, const std::vector<SDL_Texture*>& frames, int delay, bool shouldLoop) {
    animations[state].setFrames(frames, delay, shouldLoop);
}

void Player::setDefaultAnimation() {
    if (animations.count(IDLE)) {
        currentState = IDLE;
        currentAnimation = &animations[IDLE];
    }
}

void Player::handleInput(const Uint8* keystate) {
    if (currentState == HURT || currentState == DEAD) {
        velX = 0;
        return;
    }
    
    velX = 0;
    if (keystate[SDL_SCANCODE_LEFT]) {
        velX = -speed;
        facingRight = false;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        velX = speed;
        facingRight = true;
    }
    
    if (keystate[SDL_SCANCODE_UP] && onGround) {
        velY = jumpForce;
        onGround = false;
        jumpHoldTimer = 0;
    }
    
    wantsToFireThisFrame = false;
    if (keystate[SDL_SCANCODE_Z]|| keystate[SDL_SCANCODE_SPACE] && shootCooldownTimer <= 0) {
        wantsToFireThisFrame = true;
        shootCooldownTimer = shootCooldown;
    }
}

void Player::update(Level* level) {
    if (shootCooldownTimer > 0) shootCooldownTimer--;
    if (invincible) {
        invincibilityTimer--;
        if (invincibilityTimer <= 0) invincible = false;
    }
    
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_UP] && jumpHoldTimer < jumpHoldDuration && velY < 0) {
        velY += jumpHoldForce;
        jumpHoldTimer++;
    } else {
        jumpHoldTimer = jumpHoldDuration; 
    }
    
    if (velY > 0) {
        velY += gravity * fallMultiplier;
    } else {
        velY += gravity;
    }

    x += velX;
    SDL_Rect playerBounds = getBounds();
    if (velX < 0 && (level->isSolidTile(playerBounds.x, playerBounds.y) || level->isSolidTile(playerBounds.x, playerBounds.y + playerBounds.h - 1))) {
        x = (float)((int)(playerBounds.x / level->getScaledTileWidth() + 1) * level->getScaledTileWidth());
    } else if (velX > 0 && (level->isSolidTile(playerBounds.x + playerBounds.w, playerBounds.y) || level->isSolidTile(playerBounds.x + playerBounds.w, playerBounds.y + playerBounds.h - 1))) {
        x = (float)((int)((playerBounds.x + playerBounds.w) / level->getScaledTileWidth()) * level->getScaledTileWidth() - playerBounds.w);
    }

    y += velY;
    playerBounds = getBounds();
    onGround = false;
    if (velY > 0 && (level->isSolidTile(playerBounds.x, playerBounds.y + playerBounds.h) || level->isSolidTile(playerBounds.x + playerBounds.w - 1, playerBounds.y + playerBounds.h))) {
        y = (float)((int)((playerBounds.y + playerBounds.h) / level->getScaledTileHeight()) * level->getScaledTileHeight() - playerBounds.h);
        velY = 0;
        onGround = true;
    } else if (velY < 0 && (level->isSolidTile(playerBounds.x, playerBounds.y) || level->isSolidTile(playerBounds.x + playerBounds.w - 1, playerBounds.y))) {
        y = (float)((int)((playerBounds.y / level->getScaledTileHeight()) + 1) * level->getScaledTileHeight());
        velY = 0;
    }
    
    State previousState = currentState;
    if (currentAnimation && !currentAnimation->isLooping() && !currentAnimation->isFinished()) {
    } else {
        if (onGround) {
            currentState = (velX != 0) ? WALKING : IDLE;
        } else {
            currentState = (velY < 0) ? JUMPING : FALLING;
        }
    }

    if (wantsToFireThisFrame) {
        currentState = (onGround && velX != 0) ? SHOOT_WALK : SHOOT_STAND;
    }
    
    if (animations.count(currentState)) {
        currentAnimation = &animations[currentState];
        if (currentState != previousState) {
            currentAnimation->reset();
        }
    }
    if (currentAnimation) {
        currentAnimation->update();
    }
}

void Player::render(SDL_Renderer* renderer, const Camera* camera) {
    if (!currentAnimation) return;
    SDL_Texture* tex = currentAnimation->getCurrentFrameTexture();
    if (tex) {
        SDL_Rect destRect = { (int)(x - camera->getX()), (int)(y - camera->getY()), width, height };
        if (invincible && (invincibilityTimer / 6) % 2 == 0) {} else {
            SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
            SDL_RenderCopyEx(renderer, tex, NULL, &destRect, 0, NULL, flip);
        }
    }
}

bool Player::wantsToFire() const { return wantsToFireThisFrame; }

void Player::takeDamage(int amount) {
    if (!invincible && currentState != DEAD) {
        health -= amount;
        invincible = true;
        invincibilityTimer = 120;
        currentState = HURT;
        animations[HURT].reset();
        if (health <= 0) {
            health = 0;
            currentState = DEAD;
            animations[DEAD].reset();
        }
    }
}

int Player::getHealth() const { return health; }
int Player::getMaxHealth() const { return maxHealth; }
SDL_Rect Player::getBounds() const { return {(int)x, (int)y, width, height }; }
float Player::getX() const { return x; }
float Player::getY() const { return y; }
float Player::getCenterX() const { return x + width / 2.0f; }
float Player::getCenterY() const { return y + height / 2.0f; }
bool Player::isFacingRight() const { return facingRight; }
bool Player::isDead() const { return currentState == DEAD; }
