#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <list>

#include "TextureManager.h"
#include "Player.h"
#include "Camera.h"
#include "Level.h"
#include "Bullet.h"
#include "Enemy.h"
#include "HUD.h"
#include "ParticleSystem.h"

class Game {
public:
    enum GameState { PLAYING, GAME_OVER };
    GameState currentState;

    Game();
    ~Game();

    bool initialize();
    void run();

private:
    void handleEvents();
    void update();
    void render();
    void cleanup();

    bool loadMedia();
    void handlePlayerShooting();
    void spawnEnemies();
    void updateCollisions();
    void renderBackground();

    SDL_Window* window;
    SDL_Renderer* renderer;

    TextureManager* texManager;
    Player* player;
    Camera* gameCamera;
    Level* level;
    HUD* hud;
    ParticleSystem particles;

    std::vector<Bullet> playerBullets;
    std::vector<Bullet> enemyBullets;
    std::list<Enemy> enemies;

    // --- SFONDO EMBRIONALE ---
    SDL_Texture* backgroundSky = nullptr;
    SDL_Texture* backgroundMountains = nullptr;
    SDL_Texture* backgroundClouds = nullptr;

    bool running;
    int enemySpawnTimer;
};

#endif // GAME_H
