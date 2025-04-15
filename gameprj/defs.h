#ifndef _DEFS__H
#define _DEFS__H

const int TILE_SIZE = 32;
const int MAP_WIDTH = 25;
const int MAP_HEIGHT = 25;
const int SCREEN_WIDTH = TILE_SIZE * MAP_WIDTH;
const int SCREEN_HEIGHT = TILE_SIZE * MAP_HEIGHT;
const int Time_EASY = 100;
const int Time_HARD = 50;
const int Time_ASIAN = 10;
const char* WINDOW_TITLE = "NDH's Game";
const int X_SPAWN = SCREEN_WIDTH / 2;
const int Y_SPAWN = 735;
const int Num_Enemy = 15;

const char* MAN_SPRITE_FILE = "img\\bo_hong.png";
const char* MAN_SPRITE_UP_FILE = "img\\bo_hong_up.png";
const int MAN_CLIPS[][4] = {
    {  0, 0, 32, 32},
    { 32, 0, 32, 32},
    { 64, 0, 32, 32},
    { 96, 0, 32, 32}
};
const int MAN_FRAMES = sizeof(MAN_CLIPS)/sizeof(int)/4;

const char* FLAME_SPRITE_FILE = "img\\flame.png";
const int FLAME_CLIPS[][4] = {
    {  0, 0, 16, 16},
    { 16, 0, 16, 16},
    { 32, 0, 16, 16},
    { 48, 0, 16, 16},
};
const int FLAME_FRAMES = sizeof(FLAME_CLIPS)/sizeof(int)/4;


const char* ZOLTRAAK_SPRITE_FILE = "img\\zoltraak.png";
const int ZOLTRAAK_CLIPS[][4] = {
    {  0, 0,  8, 96 },
    {  8, 0,  8, 96 },
    { 16, 0,  8, 96 },
    { 24, 0,  8, 96 },
};
const int ZOLTRAAK_FRAMES = sizeof(ZOLTRAAK_CLIPS)/sizeof(int)/4;

#endif
