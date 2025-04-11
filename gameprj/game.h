#ifndef _GAME__H
#define _GAME__H
#include "mode.h"

void setTimeMode(ModeState mode) {
    switch (mode) {
        case EASY_MODE:
            countdownTime = Time_EASY * 1000;
            break;
        case HARD_MODE:
            countdownTime = Time_HARD * 1000;
            break;
        case ASIAN_MODE:
            countdownTime = Time_ASIAN * 1000;
            break;
        default:
            countdownTime = 0;
            break;
    }
}

void handleAllEvents(Graphics &graphics, Mouse &mouse, ScreenState &currentScreen, ModeState &currentMode, SDL_Event &event, bool &run) {
    if (event.type == SDL_QUIT) {
        run = false;
        return;
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    switch (currentScreen) {
        case SCREEN_MENU:
            currentScreen = handleMenuEvent(event, mouseX, mouseY, menuButtons);
            break;
        case SCREEN_MODE: {
            ModeState selectedMode = handleModeEvent(event, mouseX, mouseY, modeButtons);

            if (selectedMode != STATE_MODE) {
                setTimeMode(selectedMode);
                currentMode = selectedMode;
                mouse.x = SCREEN_WIDTH / 2;
                mouse.y = 752;
                graphics.layersData[graphics.diamond] = graphics.originalDiamondLayer;

                currentScreen = SCREEN_MENU;
            }
            break;
        }
        default:
            break;
    }
}

void renderScreen(ScreenState &currentScreen, ModeState &currentMode, Graphics &graphics, Mouse &mouse,
                  SDL_Texture* scoretxt, SDL_Texture* timetxt, TTF_Font* font, SDL_Texture* backgroundTexture, bool &run) {
    if (currentScreen == SCREEN_QUIT) {
        run = false;
        return;
    }

    switch (currentScreen) {
        case SCREEN_MENU:
            renderMenu(graphics, font, backgroundTexture, menuButtons);
            break;
        case SCREEN_MODE:
            renderMode(graphics, font, backgroundTexture, modeButtons);
            break;
        case SCREEN_GAME:
            resetTimer();
            running_Main_Game(graphics, mouse, scoretxt, timetxt, font);
            currentScreen = SCREEN_MENU;
            break;
        case SCREEN_HELP:
            cout << "Hướng dẫn sử dụng...\n";
            currentScreen = SCREEN_MENU;
            break;
    }

}

void run_Game(Graphics &graphics, Mouse &mouse, SDL_Texture* scoretxt, SDL_Texture* timetxt, TTF_Font* font, SDL_Texture* backgroundTexture) {
    graphics.cacheRenderMap();
    ScreenState currentScreen = SCREEN_MENU;
    ModeState currentMode = STATE_MODE;
    bool run = true;

    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handleAllEvents(graphics, mouse, currentScreen, currentMode, event, run);
        }
        renderScreen(currentScreen, currentMode, graphics, mouse, scoretxt, timetxt, font, backgroundTexture, run);
    }
}

#endif
