#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include "defs.h"
#include "graphics.h"
#include "player_move.h"
using namespace std;

using json = nlohmann::json;



int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();

    graphics.wall = graphics.layersData.size() - 1;

    Mouse mouse;
    mouse.x = SCREEN_WIDTH / 2;
    mouse.y = 680;

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN)
            {
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
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_D:
                        mouse.dx = 0;
                        mouse.dy = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        if (!graphics.isCollision(mouse.x + mouse.dx, mouse.y + mouse.dy)) {
            mouse.move();
        }

        SDL_RenderClear(graphics.renderer);
        graphics.renderMap();
        render(mouse, graphics);
        SDL_RenderPresent(graphics.renderer);

        SDL_Delay(10);
    }

    graphics.quit();
    return 0;
}


