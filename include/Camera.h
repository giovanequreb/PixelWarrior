#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
class Player;

class Camera {
private:
    float x, y;
    int screenWidth, screenHeight;
    int levelWidth, levelHeight;

public:
    Camera(int width, int height);
    void setLevelSize(int lw, int lh);
    void update(const Player* player);
    float getX() const;
    float getY() const;
};
#endif
