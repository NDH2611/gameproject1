#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "json.hpp"
#include "defs.h"
#include "graphics.h"
#include "player_move.h"
#include "lootitem.h"
#include "time.h"
#include "game_play.h"
#include "game_state.h"
#include "menu.h"
#include "game.h"
#include "mode.h"
using namespace std;

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();

    TTF_Font* font = graphics.loadFont("PressStart2P-Regular.ttf", 15);
    SDL_Color color = {255, 255, 0, 255};
    SDL_Texture* scoretxt = graphics.renderText("Score: ", font, color);
    SDL_Texture* timetxt = graphics.renderText("Time: ", font, color);
    SDL_Texture* background = graphics.loadTexture("background.jpg");
    graphics.wall = graphics.layersData.size() - 1;
    graphics.diamond = 1;

    Mouse mouse;
    mouse.x = SCREEN_WIDTH / 2;
    mouse.y = 752;

    //running_Main_Game(graphics, mouse, scoretxt, timetxt, font);
    run_Game(graphics, mouse, scoretxt, timetxt, font, background);

    SDL_DestroyTexture(scoretxt);
    SDL_DestroyTexture(timetxt);
    SDL_DestroyTexture(background);
    TTF_CloseFont(font);
    graphics.quit();
    return 0;
}




