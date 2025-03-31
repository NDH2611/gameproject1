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
using namespace std;

using json = nlohmann::json;



int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();

    TTF_Font* font = graphics.loadFont("Purisa-BoldOblique.ttf", 25);
    SDL_Color color = {255, 255, 0, 255};
    SDL_Texture* scoretxt = graphics.renderText("Score: ", font, color);

    graphics.wall = graphics.layersData.size() - 1;
    graphics.diamond = 1;

    Mouse mouse;
    mouse.x = SCREEN_WIDTH / 2;
    mouse.y = 680;

    running_Main_Game(graphics, mouse, scoretxt, font);

    SDL_DestroyTexture(scoretxt);
    TTF_CloseFont(font);
    graphics.quit();
    return 0;
}




