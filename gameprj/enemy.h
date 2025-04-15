#ifndef _GAME__STATE__H
#define _GAME__STATE__H

#include <cstdlib>
#include <ctime>
#include <vector>
#include "lootitem.h"
using namespace std;
const int ENEMY_SIZE = 16;

const int CHASE_RANGE = 64;
const int MOVE_SPEED = 2;

struct Enemy {
    int x, y;
    int width, height;
    int frame = 0;

    Enemy(int x, int y, int w = ENEMY_SIZE, int h = ENEMY_SIZE)
        : x(x), y(y), width(w), height(h) {}
};

Enemy spawnRandomEnemy(Graphics& graphics, int enemyWidth, int enemyHeight) {
    int x, y;
    do {
        x = (rand() % MAP_WIDTH) * TILE_SIZE;
        y = (rand() % MAP_HEIGHT) * TILE_SIZE;
    } while (graphics.isCollision(x, y, enemyWidth, enemyHeight) || isCollisionWithdiamond(graphics, x, y, enemyWidth, enemyHeight));

    return Enemy(x, y, enemyWidth, enemyHeight);
}

vector<Enemy> spawnMultipleEnemies(Graphics& graphics, int count, int enemyWidth, int enemyHeight) {
    vector<Enemy> enemies;

    while ((int)enemies.size() < count) {
        Enemy e = spawnRandomEnemy(graphics, enemyWidth, enemyHeight);

        bool overlap = false;
        for (const Enemy& other : enemies) {
            if (e.x == other.x && e.y == other.y) {
                overlap = true;
                break;
            }
        }

        if (!overlap) {
            enemies.push_back(e);
        }
    }

    return enemies;
}

void renderEnemies(Graphics& graphics, vector<Enemy>& enemies) {
    SDL_Texture* flameTex = graphics.textureCache["flame"];

    for (Enemy& e : enemies) {
        SDL_Rect srcRect = {
            FLAME_CLIPS[e.frame % FLAME_FRAMES][0],
            FLAME_CLIPS[e.frame % FLAME_FRAMES][1],
            FLAME_CLIPS[e.frame % FLAME_FRAMES][2],
            FLAME_CLIPS[e.frame % FLAME_FRAMES][3]
        };

        SDL_Rect destRect = { e.x, e.y, e.width, e.height };

        SDL_RenderCopy(graphics.renderer, flameTex, &srcRect, &destRect);

        e.frame++;
    }
}

bool checkWallBetween(int startX, int startY, int endX, int endY, Graphics& graphics) {
    float dx = endX - startX;
    float dy = endY - startY;
    float steps = max(abs(dx), abs(dy));

    if (steps == 0) return false;

    for (int i = 0; i <= steps; ++i) {
        int x = startX + (dx * i / steps);
        int y = startY + (dy * i / steps);

        if (graphics.isCollision(x, y, ENEMY_SIZE, ENEMY_SIZE) ||
            isCollisionWithdiamond(graphics, x, y, ENEMY_SIZE, ENEMY_SIZE)) {
            return true;
        }
    }
    return false;
}

void enemy_move(Graphics& graphics, Mouse& mouse, vector<Enemy>& enemies) {

    Uint32 frameTime = SDL_GetTicks() - lastTime;

    const float adjustedMoveSpeed = MOVE_SPEED * (frameTime / 16.0f);

    for (Enemy& e : enemies) {
        int dx = mouse.x - e.x;
        int dy = mouse.y - e.y;
        int distance = sqrt(dx * dx + dy * dy);

        if (distance <= CHASE_RANGE && !checkWallBetween(e.x, e.y, mouse.x, mouse.y, graphics)) {
            e.x += (dx != 0) ? copysign(adjustedMoveSpeed, dx) : 0;
            e.y += (dy != 0) ? copysign(adjustedMoveSpeed, dy) : 0;
        }
        else {
            int dirX = rand() % (2 * MOVE_SPEED + 1) - MOVE_SPEED;
            int dirY = rand() % (2 * MOVE_SPEED + 1) - MOVE_SPEED;
            float moveX = dirX * adjustedMoveSpeed;
            float moveY = dirY * adjustedMoveSpeed;

            if (!graphics.isCollision(e.x + moveX, e.y, e.width, e.height) &&
                !isCollisionWithdiamond(graphics, e.x + moveX, e.y, e.width, e.height)) {
                e.x += moveX;
            }

            if (!graphics.isCollision(e.x, e.y + moveY, e.width, e.height) &&
                !isCollisionWithdiamond(graphics, e.x, e.y + moveY, e.width, e.height)) {
                e.y += moveY;
            }
        }
    }
}


bool checkLaserCollisionWithEnemy(const Laser& laser, const Enemy& enemy) {
    SDL_Rect laserRect;
    switch (laser.dir) {
        case NORTH:
            laserRect = { static_cast<int>(laser.x), static_cast<int>(laser.y - laser.currentLength), laser.w, laser.currentLength };
            break;
        case SOUTH:
            laserRect = { static_cast<int>(laser.x), static_cast<int>(laser.y), laser.w, laser.currentLength };
            break;
        case WEST:
            laserRect = { static_cast<int>(laser.x - laser.currentLength), static_cast<int>(laser.y), laser.currentLength, laser.h };
            break;
        case EAST:
            laserRect = { static_cast<int>(laser.x), static_cast<int>(laser.y), laser.currentLength, laser.h };
            break;
    }

    SDL_Rect enemyRect = { enemy.x, enemy.y, enemy.width, enemy.height };

    return SDL_HasIntersection(&laserRect, &enemyRect);
}

bool kill_enemy(vector<Enemy>& enemies, vector<Laser>& lasers) {
    for (auto& laser : lasers) {
        if (!laser.active) continue;

        auto it = remove_if(enemies.begin(), enemies.end(), [&](const Enemy& e) {
            return checkLaserCollisionWithEnemy(laser, e);
        });

        if (it != enemies.end()) {
            enemies.erase(it, enemies.end());
            return true;
        }
    }
    return false;
}

#endif
