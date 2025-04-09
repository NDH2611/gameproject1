#ifndef PLAYER_MOVE_H
#define PLAYER_MOVE_H

#define INITIAL_SPEED 4
#define DESTINATION 16

struct Mouse {
    int x, y;
    int dx = 0, dy = 0;
    int speed = INITIAL_SPEED;
    void move() {
        x += dx;
        y += dy;
    }
    void turnNorth() {
        dy = -speed;
        dx = 0;
    }
    void turnSouth() {
        dy = speed;
        dx = 0;
    }
    void turnWest() {
        dy = 0;
        dx = -speed;
    }
    void turnEast() {
        dy = 0;
        dx = speed;
    }
};

void render(const Mouse& mouse, const Graphics& graphics) {
    SDL_Rect filled_rect;
    filled_rect.x = mouse.x;
    filled_rect.y = mouse.y;
    filled_rect.w = DESTINATION;
    filled_rect.h = DESTINATION;
    SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(graphics.renderer, &filled_rect);
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
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                mouse.dx = 0;
                mouse.dy = 0;
                break;
            default: break;
        }
    }
}
#endif

