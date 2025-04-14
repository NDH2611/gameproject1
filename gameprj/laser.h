#ifndef LASER_H_
#define LASER_H_
#include "defs.h"
const int LASER_WIDTH = 8;
const int LASER_MAX_LENGTH = 96;
const int LASER_GROW_SPEED = 4;

enum Direction {
    NORTH,
    SOUTH,
    WEST,
    EAST
};

struct Laser {
    float x, y;
    float dx, dy;
    int w, h;
    int currentLength;
    int maxLength;
    int growSpeed;
    bool growing;
    bool active;
    Direction dir;

    int frame = 0;
    int frameTimer = 0;
    int frameDelay = 4;


    Laser(float startX, float startY, Direction direction)
        : x(startX), y(startY), dx(0), dy(0),
          w(0), h(0), currentLength(0),
          maxLength(LASER_MAX_LENGTH), growSpeed(LASER_GROW_SPEED),
          growing(true), active(true), dir(direction)
    {
        setDirection(direction);
    }

    void setDirection(Direction direction) {
        dir = direction;
        dx = dy = 0;
        w = h = 0;
        switch (dir) {
            case NORTH:
                dy = -1;
                w = LASER_WIDTH;
                break;
            case SOUTH:
                dy = 1;
                w = LASER_WIDTH;
                break;
            case WEST:
                dx = -1;
                h = LASER_WIDTH;
                break;
            case EAST:
                dx = 1;
                h = LASER_WIDTH;
                break;
        }
    }
void move(Graphics& graphics) {
    if (growing) {
        int nextLength = min(currentLength + growSpeed, maxLength);

        int tx = static_cast<int>(x), ty = static_cast<int>(y);
        int tw = LASER_WIDTH, th = nextLength;

        switch (dir) {
            case NORTH: ty -= nextLength; break;
            case SOUTH: break;
            case WEST:  tx -= nextLength; tw = nextLength; th = LASER_WIDTH; break;
            case EAST:  tw = nextLength; th = LASER_WIDTH; break;
        }

        if (graphics.isCollision(tx, ty, tw, th)) {
            growing = false;
        }
        else {
            currentLength = nextLength;
            if (currentLength == maxLength) growing = false;
        }
    }
    else {
        currentLength -= growSpeed;
        if (currentLength <= 0) {
            currentLength = 0;
            active = false;
            return;
        }

        switch (dir) {
            case NORTH: y -= growSpeed; break;
            case SOUTH: y += growSpeed; break;
            case WEST:  x -= growSpeed; break;
            case EAST:  x += growSpeed; break;
        }
    }

    if (dir == NORTH || dir == SOUTH) {
        w = LASER_WIDTH;
        h = currentLength;
    }
    else {
        w = currentLength;
        h = LASER_WIDTH;
    }
}

    void render(SDL_Renderer* renderer, SDL_Texture* texture) {

        SDL_Rect src = {
            ZOLTRAAK_CLIPS[frame][0],
            ZOLTRAAK_CLIPS[frame][1],
            ZOLTRAAK_CLIPS[frame][2],
            ZOLTRAAK_CLIPS[frame][3]
        };
        SDL_Rect dst;
        double angle = 0;

        switch (dir) {
            case NORTH:
                dst = { static_cast<int>(x), static_cast<int>(y - currentLength), LASER_WIDTH, currentLength };
                angle = 0;
                break;
            case SOUTH:
                dst = { static_cast<int>(x), static_cast<int>(y), LASER_WIDTH, currentLength };
                angle = 180;
                break;
            case WEST:
                dst = { static_cast<int>(x - currentLength), static_cast<int>(y), currentLength, LASER_WIDTH };
                angle = 270;
                break;
            case EAST:
                dst = { static_cast<int>(x), static_cast<int>(y), currentLength, LASER_WIDTH };
               angle = 90;
                break;
        }

        SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, nullptr, SDL_FLIP_NONE);
        frameTimer++;
        if (frameTimer >= frameDelay) {
            frame = (frame + 1) % ZOLTRAAK_FRAMES;
            frameTimer = 0;
        }

    }
};

void updateLasers(vector<Laser>& lasers, Graphics& graphics) {
    for (auto& laser : lasers) {
        if (laser.active) laser.move(graphics);
    }

    lasers.erase(remove_if(lasers.begin(), lasers.end(),
                           [](const Laser& l){ return !l.active; }),
                 lasers.end());
}

void renderLasers(vector<Laser>& lasers, SDL_Renderer* renderer, SDL_Texture* texture) {
    for (auto& laser : lasers) {
        if (laser.active) laser.render(renderer, texture);
    }
}

void Zoltraak(SDL_Event &event, vector<Laser>& lasers, Mouse &mouse)
{
    switch (event.key.keysym.scancode)
    {
        case SDL_SCANCODE_UP:
            lasers.push_back(Laser(mouse.x + (DESTINATION - LASER_WIDTH) / 2, mouse.y + DESTINATION / 2, NORTH));
            break;

        case SDL_SCANCODE_DOWN:
            lasers.push_back(Laser(mouse.x + (DESTINATION - LASER_WIDTH) / 2, mouse.y + DESTINATION / 2, SOUTH));
            break;

        case SDL_SCANCODE_LEFT:
            lasers.push_back(Laser(mouse.x + DESTINATION / 2, mouse.y + (DESTINATION - LASER_WIDTH) / 2, WEST));
            break;

        case SDL_SCANCODE_RIGHT:
            lasers.push_back(Laser(mouse.x + DESTINATION / 2, mouse.y + (DESTINATION - LASER_WIDTH) / 2, EAST));
            break;

        default:
            break;
    }
}

#endif
