#ifndef PLAYER_MOVE_H
#define PLAYER_MOVE_H

#define INITIAL_SPEED 4
#define DESTINATION 32

struct Mouse {
    int x, y;
    int dx = 0, dy = 0;
    int speed = INITIAL_SPEED;
    bool flip = false;
    double angle = 0.0;
    bool isMoving = false;

    Sprite sideSprite;
    Sprite updownSprite;
    Sprite* currentSprite;

    void initSprite(SDL_Texture* tex, int frames, const int clips[][4], bool updown = false) {
        if (updown)
            updownSprite.init(tex, frames, clips);
        else
            sideSprite.init(tex, frames, clips);
        currentSprite = &sideSprite;
    }

    void move() {
        x += dx;
        y += dy;

        if (isMoving) {
            currentSprite->tick();
        }
        else {
            currentSprite = &sideSprite;
            sideSprite.currentFrame = 0;
            angle = 0.0;
        }
    }

    void turnNorth() {
        dy = -speed; dx = 0;
        angle = 0.0;
        currentSprite = &updownSprite;
    }
    void turnSouth() {
        dy = speed; dx = 0;
        angle = 180.0;
        currentSprite = &updownSprite;
    }
    void turnWest() {
        dx = -speed; dy = 0;
        flip = false;
        angle = 0.0;
        currentSprite = &sideSprite;
    }
    void turnEast() {
        dx = speed; dy = 0;
        flip = true;
        angle = 0.0;
        currentSprite = &sideSprite;
    }

    void reset() {
        x = X_SPAWN;
        y = Y_SPAWN;
        dx = dy = 0;
        flip = false;
        angle = 0.0;
        currentSprite = &sideSprite;
        sideSprite.currentFrame = 0;
        isMoving = false;
    }
};

void render(const Mouse& mouse, Graphics& graphics) {
    graphics.render(mouse.x, mouse.y, *(mouse.currentSprite), mouse.flip, mouse.angle);
}

void keyboard(SDL_Event &event, Mouse &mouse)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W: mouse.turnNorth(); break;
            case SDL_SCANCODE_S: mouse.turnSouth(); break;
            case SDL_SCANCODE_A: mouse.turnWest(); break;
            case SDL_SCANCODE_D: mouse.turnEast(); break;
            default: break;
        }
        mouse.isMoving = true;
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                mouse.dx = 0;
                mouse.dy = 0;
                mouse.isMoving = false;
                break;
            default: break;
        }
    }
}
#endif

