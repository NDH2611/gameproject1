#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "player_move.h"

using namespace std;

int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();

    Mouse mouse;
    mouse.x = SCREEN_WIDTH / 2;
    mouse.y = 880;

    bool quit = false;
    SDL_Event event;
    bool moving = false;
    while (!quit && !gameOver(mouse)) {
        graphics.prepareScene();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;

            if (event.type == SDL_KEYDOWN)
            {
                moving = true;
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                        mouse.turnNorth();
                        break;
                    case SDL_SCANCODE_S:
                        mouse.turnSouth();
                        break;
                    case SDL_SCANCODE_A:
                        mouse.turnWest();
                        break;
                    case SDL_SCANCODE_D:
                        mouse.turnEast();
                        break;
                    default:
                        break;
                }
            }
            if (event.type == SDL_KEYUP)
            {
                moving = false;
            }
        }

        if (moving) {
            mouse.move();
        }

        render(mouse, graphics);

        graphics.presentScene();
        SDL_Delay(10);
    }

    graphics.quit();
    return 0;
}

