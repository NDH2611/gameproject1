#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

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
#include "laser.h"
#include "enemy.h"
using namespace std;

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init();

    srand(time(0));
    TTF_Font* font = graphics.loadFont("PressStart2P-Regular.ttf", 15);
    SDL_Color color = {255, 255, 0, 255};
    SDL_Texture* scoretxt = graphics.renderText("Score: ", font, color);
    SDL_Texture* timetxt = graphics.renderText("Time: ", font, color);
    SDL_Texture* background = graphics.getOrLoadTexture("background.jpg");
    graphics.wall = graphics.layersData.size() - 1;
    graphics.diamond = 1;

    Mouse mouse;
    graphics.textureCache["mouse_side"] = graphics.loadTextureWithColorKey(graphics.renderer, MAN_SPRITE_FILE, 255, 255, 255);
    graphics.textureCache["mouse_up"] = graphics.loadTextureWithColorKey(graphics.renderer, MAN_SPRITE_UP_FILE, 255, 255, 255);


    mouse.initSprite(graphics.textureCache["mouse_side"], MAN_FRAMES, MAN_CLIPS);
    mouse.initSprite(graphics.textureCache["mouse_up"], MAN_FRAMES, MAN_CLIPS, true);

    mouse.x = X_SPAWN;
    mouse.y = Y_SPAWN;

    vector<Laser> lasers;
    graphics.textureCache["zoltraak"] = graphics.loadTextureWithColorKey(graphics.renderer, ZOLTRAAK_SPRITE_FILE, 255, 255, 255);
    graphics.textureCache["flame"] = graphics.loadTextureWithColorKey(graphics.renderer, FLAME_SPRITE_FILE, 255, 255, 255);
    vector<Enemy> enemies;

    run_Game(graphics, mouse, lasers, enemies, scoretxt, timetxt, font, background);

    SDL_DestroyTexture(scoretxt);
    SDL_DestroyTexture(timetxt);
    TTF_CloseFont(font);
    graphics.quit();
    return 0;
}




