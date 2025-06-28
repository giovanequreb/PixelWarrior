#include "../include/Level.h"
#include "../include/Constants.h"
#include <cmath>

Level::Level(SDL_Renderer* ren, SDL_Texture* tileset, int tileW, int tileH, int tilesetCols, int scale)
    : renderer(ren), tilesetTexture(tileset), 
      baseTileWidth(tileW), baseTileHeight(tileH),
      scaledTileWidth(tileW * scale), scaledTileHeight(tileH * scale),
      tilesetColumns(tilesetCols) {}

void Level::loadMap(const std::vector<std::vector<int>>& mapData) {
    tileMap = mapData;
}

void Level::render(float cameraX, float cameraY) {
    int screenWidth = 800;
    int screenHeight = 600;
    int firstCol = (int)floor(cameraX / scaledTileWidth);
    int firstRow = (int)floor(cameraY / scaledTileHeight);
    int colsOnScreen = screenWidth / scaledTileWidth + 2;
    int rowsOnScreen = screenHeight / scaledTileHeight + 2;

    for (int row = firstRow; row < firstRow + rowsOnScreen && row < (int)tileMap.size(); ++row) {
        if (row < 0) continue;
        for (int col = firstCol; col < firstCol + colsOnScreen && col < (int)tileMap[row].size(); ++col) {
            if (col < 0) continue;
            int tileID = tileMap[row][col];
            if (tileID < 0) continue;

            int tilesetX = (tileID % tilesetColumns) * baseTileWidth;
            int tilesetY = (tileID / tilesetColumns) * baseTileHeight;

            SDL_Rect srcRect = {tilesetX, tilesetY, baseTileWidth, baseTileHeight};
            SDL_Rect destRect = { 
                (int)floor(col * scaledTileWidth - cameraX), 
                (int)floor(row * scaledTileHeight - cameraY), 
                scaledTileWidth, 
                scaledTileHeight 
            };
            SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
        }
    }
}

int Level::getWidth() const {
    if (tileMap.empty()) return 0;
    return (int)tileMap[0].size() * scaledTileWidth;
}

int Level::getHeight() const {
    return (int)tileMap.size() * scaledTileHeight;
}

bool Level::isSolidTile(int worldX, int worldY) const {
    if (worldX < 0 || worldY < 0) return false;
    int tileX = worldX / scaledTileWidth;
    int tileY = worldY / scaledTileHeight;
    if (tileY >= (int)tileMap.size() || tileX >= (int)tileMap[0].size()) return true;
    return tileMap[tileY][tileX] >= 0;
}
