#ifndef LOOT_ITEM_H
#define LOOT_ITEM_H

#include "defs.h"
int score = 0;
bool isCollisionWithdiamond(Graphics &graphics, int x, int y, int width, int height) {
    int left = x / TILE_SIZE;
    int right = (x + width - 1) / TILE_SIZE;
    int top = y / TILE_SIZE;
    int bottom = (y + height - 1) / TILE_SIZE;

    if (graphics.layersData[graphics.diamond][top * MAP_WIDTH + left] != 0 ||
        graphics.layersData[graphics.diamond][top * MAP_WIDTH + right] != 0 ||
        graphics.layersData[graphics.diamond][bottom * MAP_WIDTH + left] != 0 ||
        graphics.layersData[graphics.diamond][bottom * MAP_WIDTH + right] != 0)
        {
            score += 10;
            return true;
        }
    return false;
}

void removeDiamond(Graphics &graphics, int x, int y) {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;
    int index = tileY * MAP_WIDTH + tileX;

    if (graphics.layersData[1][index] != 0) {
        graphics.layersData[1][index] = 0;
    }
}

#endif

