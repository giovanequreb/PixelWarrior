#include "../include/Camera.h"
#include "../include/Player.h"

Camera::Camera(int width, int height) 
    : x(0), y(0), screenWidth(width), screenHeight(height), levelWidth(0), levelHeight(0) {}

void Camera::setLevelSize(int lw, int lh) {
    levelWidth = lw;
    levelHeight = lh;
}

void Camera::update(const Player* player) {
    if (!player) return;
    
    x = player->getX() - (screenWidth / 2);
    y = player->getY() - (screenHeight / 2);

    if (x < 0) { x = 0; }
    if (y < 0) { y = 0; }

    if (levelWidth > screenWidth && x > levelWidth - screenWidth) {
        x = levelWidth - screenWidth;
    }
    if (levelHeight > screenHeight && y > levelHeight - screenHeight) {
        y = levelHeight - screenHeight;
    }
}

float Camera::getX() const { return x; }
float Camera::getY() const { return y; }
