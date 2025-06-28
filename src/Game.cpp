#include "../include/Game.h"
#include "../include/Constants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

Game::Game() : currentState(PLAYING),
    window(nullptr), renderer(nullptr), texManager(nullptr),
    player(nullptr), gameCamera(nullptr), level(nullptr), hud(nullptr),
    backgroundSky(nullptr), backgroundMountains(nullptr), backgroundClouds(nullptr),
    running(false), enemySpawnTimer(0) {}

Game::~Game() {
    cleanup();
}

bool Game::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) { std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl; return false; }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) { std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl; return false; }
    srand(time(nullptr));
    window = SDL_CreateWindow("Pixel Warrior", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) { std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl; return false; }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) { std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl; return false; }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    texManager = new TextureManager(renderer);
    if (!loadMedia()) { std::cerr << "Failed to load media!" << std::endl; return false; }

    player = new Player(100 * SCALE, 100 * SCALE, SCALE + 2);
    gameCamera = new Camera(800, 600);
    hud = new HUD(renderer, texManager);

    std::vector<SDL_Texture*> idleFrames, walkFrames, jumpFrames, fallFrames, hurtFrames, shootStandFrames, shootWalkFrames;
    for(int i = 1; i <= 4; ++i) idleFrames.push_back(texManager->getTexture("p_idle_" + std::to_string(i)));
    for(int i = 1; i <= 8; ++i) walkFrames.push_back(texManager->getTexture("p_walk_" + std::to_string(i)));
    jumpFrames.push_back(texManager->getTexture("p_jump"));
    fallFrames.push_back(texManager->getTexture("p_fall"));
    for(int i = 1; i <= 4; ++i) hurtFrames.push_back(texManager->getTexture("p_hurt_" + std::to_string(i)));
    shootStandFrames = idleFrames;
    shootWalkFrames = walkFrames;
    player->setAnimation(Player::State::IDLE, idleFrames, 15);
    player->setAnimation(Player::State::WALKING, walkFrames, 6);
    player->setAnimation(Player::State::JUMPING, jumpFrames, 1, false);
    player->setAnimation(Player::State::FALLING, fallFrames, 1, false);
    player->setAnimation(Player::State::HURT, hurtFrames, 10, false);
    player->setAnimation(Player::State::DEAD, hurtFrames, 10, false);
    player->setAnimation(Player::State::SHOOT_STAND, shootStandFrames, 5, false);
    player->setAnimation(Player::State::SHOOT_WALK, shootWalkFrames, 6, false);
    player->setDefaultAnimation();

    for (int i = 0; i < 200; ++i) { playerBullets.emplace_back(SCALE); }
    for(auto& b : playerBullets) b.setTexture(texManager->getTexture("bullet"));
    for (int i = 0; i < 200; ++i) { enemyBullets.emplace_back(SCALE); }
    for(auto& b : enemyBullets) b.setTexture(texManager->getTexture("bullet"));

    level = new Level(renderer, texManager->getTexture("tileset"), 16, 16, 8, SCALE);
    std::vector<std::vector<int>> mapData(15, std::vector<int>(100, -1));
    for(int i = 0; i < 100; ++i) { mapData[12][i] = 1; for (int j = 13; j < 15; ++j) { mapData[j][i] = 9; } }
    mapData[9][20] = 0; mapData[9][21] = 0; mapData[9][22] = 0;
    mapData[7][35] = 0; mapData[7][36] = 0;
    mapData[11][45] = 0; mapData[10][45] = 0; mapData[9][45] = 0;
    level->loadMap(mapData);
    gameCamera->setLevelSize(level->getWidth(), level->getHeight());

    enemySpawnTimer = 120;
    running = true;
    return true;
}

bool Game::loadMedia() {
    // --- SFONDO EMBRIONALE ---
    backgroundSky = IMG_LoadTexture(renderer, "assets/Background/blue/BG_sky.png");
    backgroundMountains = IMG_LoadTexture(renderer, "assets/Background/blue/layer_01.png");
    backgroundClouds = IMG_LoadTexture(renderer, "assets/Background/blue/layer_02.png");
    if (!backgroundSky || !backgroundMountains || !backgroundClouds) {
        std::cerr << "Errore nel caricamento dello sfondo!" << std::endl;
        return false;
    }

    if(!texManager->loadTexture("heart_full", "assets/sprites/UI/HearthUI_Full.png")) return false;
    if(!texManager->loadTexture("heart_empty", "assets/sprites/UI/HearthUI_Empty.png")) return false;
    for(int i = 1; i <= 4; ++i) if(!texManager->loadTexture("p_idle_" + std::to_string(i), "assets/sprites/Gunner/Gunner_Idle_" + std::to_string(i) + ".png")) return false;
    for(int i = 1; i <= 8; ++i) if(!texManager->loadTexture("p_walk_" + std::to_string(i), "assets/sprites/Gunner/Gunner_Walk_" + std::to_string(i) + ".png")) return false;
    if(!texManager->loadTexture("p_jump", "assets/sprites/Gunner/Gunner_Jump_1.png")) return false;
    if(!texManager->loadTexture("p_fall", "assets/sprites/Gunner/Gunner_Fall_1.png")) return false;
    for(int i = 1; i <= 4; ++i) if(!texManager->loadTexture("p_hurt_" + std::to_string(i), "assets/sprites/Gunner/Gunner_Hurt_" + std::to_string(i) + ".png")) return false;
    for(int i = 1; i <= 3; ++i) if(!texManager->loadTexture("shotfx_" + std::to_string(i), "assets/sprites/FX/ShotFX_" + std::to_string(i) + ".png")) return false;
    std::vector<std::string> colors = {"Blue", "Green", "Purple", "Red", "Yellow"};
    for (const auto& color : colors) {
        for(int i = 1; i <= 3; ++i) if(!texManager->loadTexture("e_" + color + "_idle_" + std::to_string(i), "assets/sprites/Enemies/" + color + "/Enemy" + color + "_Idle_" + std::to_string(i) + ".png")) return false;
        for(int i = 1; i <= 6; ++i) if(!texManager->loadTexture("e_" + color + "_death_" + std::to_string(i), "assets/sprites/Enemies/" + color + "/Enemy" + color + "_Death_" + std::to_string(i) + ".png")) return false;
        for(int i = 1; i <= 7; ++i) if(!texManager->loadTexture("e_" + color + "_shoot_" + std::to_string(i), "assets/sprites/Enemies/" + color + "/Enemy" + color + "_Shoot_" + std::to_string(i) + ".png")) return false;
    }
    if(!texManager->loadTexture("tileset", "assets/sprites/Tileset.png")) return false;
    if(!texManager->loadTexture("bullet", "assets/sprites/FX/PlayerProjectile.png")) return false;
    return true;
}

void Game::run() {
    const int frameDelay = 1000 / 60;
    Uint32 frameStart;
    int frameTime;
    while (running) {
        frameStart = SDL_GetTicks();
        handleEvents();
        update();
        render();
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) { SDL_Delay(frameDelay - frameTime); }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            running = false;
    }
    player->handleInput(keystate);
}

void Game::handlePlayerShooting() {
    if (player->wantsToFire()) {
        for (auto& bullet : playerBullets) {
            if (!bullet.isActive()) {
                float bulletStartX = player->isFacingRight() ? player->getX() + (22 * SCALE) : player->getX() - (5 * SCALE);
                float bulletStartY = player->getY() + (7 * SCALE);
                bullet.fire(bulletStartX, bulletStartY, player->isFacingRight() ? 1.0f : -1.0f, 0);
                break;
            }
        }
    }
}

void Game::spawnEnemies() {
    if (currentState == GAME_OVER) return;
    enemySpawnTimer--;
    if (enemySpawnTimer <= 0) {
        int groundLevelRow = 12;
        int enemyHeightInTiles = 1;
        int spawnY = (groundLevelRow - enemyHeightInTiles) * (16 * SCALE);
        enemies.emplace_back(level->getWidth(), spawnY, SCALE);
        Enemy& newEnemy = enemies.back();
        std::vector<std::string> colors = {"Blue", "Green", "Purple", "Red", "Yellow"};
        std::string randomColor = colors[rand() % colors.size()];
        std::vector<SDL_Texture*> idleFrames, deathFrames, shootFrames;
        for(int i = 1; i <= 3; ++i) idleFrames.push_back(texManager->getTexture("e_" + randomColor + "_idle_" + std::to_string(i)));
        for(int i = 1; i <= 6; ++i) deathFrames.push_back(texManager->getTexture("e_" + randomColor + "_death_" + std::to_string(i)));
        for(int i = 1; i <= 7; ++i) shootFrames.push_back(texManager->getTexture("e_" + randomColor + "_shoot_" + std::to_string(i)));
        newEnemy.setAnimation(Enemy::State::ALIVE, idleFrames, 15, true);
        newEnemy.setAnimation(Enemy::State::DYING, deathFrames, 8, false);
        newEnemy.setAnimation(Enemy::State::SHOOTING, shootFrames, 7, false);
        enemySpawnTimer = 90 + rand() % 90;
    }
}

void Game::updateCollisions() {
    for (auto& bullet : playerBullets) {
        if (!bullet.isActive()) continue;
        SDL_Rect bulletBounds = bullet.getBounds();
        for (auto& enemy : enemies) {
            if (enemy.state == Enemy::State::ALIVE || enemy.state == Enemy::State::SHOOTING) {
                SDL_Rect enemyBounds = enemy.getBounds();
                if (SDL_HasIntersection(&bulletBounds, &enemyBounds)) {
                    bullet.deactivate();
                    enemy.die();
                }
            }
        }
    }
    for (auto& bullet : enemyBullets) {
        if (!bullet.isActive()) continue;
        SDL_Rect bulletBounds = bullet.getBounds();
        SDL_Rect playerBounds = player->getBounds();
        if (SDL_HasIntersection(&bulletBounds, &playerBounds)) {
            bullet.deactivate();
            player->takeDamage(1);
        }
    }
}

void Game::update() {
    if (currentState == GAME_OVER && player->isDead()) {
        player->update(level);
        return;
    }
    player->update(level);
    handlePlayerShooting();
    if (player->isDead() && player->currentState != Player::State::DEAD) {
        player->currentState = Player::State::DEAD;
    }
    gameCamera->update(player);
    for (auto& bullet : playerBullets) bullet.update(level);
    for (auto& bullet : enemyBullets) bullet.update(level);
    spawnEnemies();
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if(it->state == Enemy::State::DEAD) {
            it = enemies.erase(it);
        } else {
            it->update();
            it->attemptShooting();
            if (it->wantsToFire()) {
                for (auto& bullet : enemyBullets) {
                    if (!bullet.isActive()) {
                        bullet.fire(it->getX(), it->getY() + it->getHeight() / 2, -1.0f, 0);
                        break;
                    }
                }
            }
            ++it;
        }
    }
    updateCollisions();
    particles.update();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
    SDL_RenderClear(renderer);
    renderBackground();
    level->render(gameCamera->getX(), gameCamera->getY());
    for (auto& enemy : enemies) enemy.render(renderer, gameCamera);
    player->render(renderer, gameCamera);
    for (auto& bullet : playerBullets) bullet.render(renderer, gameCamera);
    for (auto& bullet : enemyBullets) bullet.render(renderer, gameCamera);
    particles.render(renderer, gameCamera);
    hud->render(player->getHealth(), player->getMaxHealth());
    SDL_RenderPresent(renderer);
}

void Game::renderBackground() {
    // --- SFONDO EMBRIONALE PARALLAX ---
    // Cielo (fermo)
    SDL_Rect dest = {0, 0, 800, 600};
    SDL_RenderCopy(renderer, backgroundSky, NULL, &dest);

    // Montagne (parallax lento)
    int offsetMountains = (int)(gameCamera->getX() * 0.2f) % 800;
    SDL_Rect destMountains1 = { -offsetMountains, 0, 800, 600 };
    SDL_Rect destMountains2 = { -offsetMountains + 800, 0, 800, 600 };
    SDL_RenderCopy(renderer, backgroundMountains, NULL, &destMountains1);
    SDL_RenderCopy(renderer, backgroundMountains, NULL, &destMountains2);

    // Nuvole (parallax più veloce)
    int offsetClouds = (int)(gameCamera->getX() * 0.4f) % 800;
    SDL_Rect destClouds1 = { -offsetClouds, 0, 800, 600 };
    SDL_Rect destClouds2 = { -offsetClouds + 800, 0, 800, 600 };
    SDL_RenderCopy(renderer, backgroundClouds, NULL, &destClouds1);
    SDL_RenderCopy(renderer, backgroundClouds, NULL, &destClouds2);
}

void Game::cleanup() {
    if (backgroundSky) SDL_DestroyTexture(backgroundSky);
    if (backgroundMountains) SDL_DestroyTexture(backgroundMountains);
    if (backgroundClouds) SDL_DestroyTexture(backgroundClouds);
    delete hud;
    delete player;
    delete gameCamera;
    delete level;
    delete texManager;
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
