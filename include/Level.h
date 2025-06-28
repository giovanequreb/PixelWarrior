#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <vector>

class Level {
private:
    SDL_Renderer* renderer;
    SDL_Texture* tilesetTexture;
    std::vector<std::vector<int>> tileMap;
    
    int baseTileWidth, baseTileHeight;
    int scaledTileWidth, scaledTileHeight;
    int tilesetColumns;

public:
    Level(SDL_Renderer* ren, SDL_Texture* tileset, int tileW, int tileH, int tilesetCols, int scale);
    
    void loadMap(const std::vector<std::vector<int>>& mapData);
    void render(float cameraX, float cameraY);
    
    int getWidth() const;
    int getHeight() const;
    int getScaledTileWidth() const { return scaledTileWidth; }
    int getScaledTileHeight() const { return scaledTileHeight; }
    
    bool isSolidTile(int worldX, int worldY) const;
};

#endif
