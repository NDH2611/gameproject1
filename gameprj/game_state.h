#ifndef _GAME__STATE__H
#define _GAME__STATE__H

enum GameState { GAME_RUNNING, GAME_OVER, GAME_PAUSED };

void renderGameState(Graphics &graphics, Mouse &mouse, SDL_Texture* scoretxt, SDL_Texture* timetxt, TTF_Font* font,
                     GameState gameState, SDL_Texture* iscore, SDL_Texture* iTime) {
    SDL_RenderClear(graphics.renderer);
    graphics.renderMap();
    graphics.renderTexture(scoretxt, 22, 775);
    graphics.renderTexture(iscore, 112, 775);
    graphics.renderTexture(timetxt, 670, 775);
    graphics.renderTexture(iTime, 750, 775);
    render(mouse, graphics);

    const char* stateMessage = nullptr;
    SDL_Color color = {255, 255, 255, 255};

    if (gameState == GAME_PAUSED) {
        stateMessage = "PAUSED";
        color = {255, 255, 0, 255};
    }
    else if (gameState == GAME_OVER) {
        stateMessage = "GAME OVER";
        color = {255, 0, 0, 255};
    }

    if (stateMessage) {
        SDL_Texture* stateText = graphics.renderText(stateMessage, font, color);
        if (stateText) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(stateText, nullptr, nullptr, &texW, &texH);
            int x = (SCREEN_WIDTH - texW) / 2;
            int y = (SCREEN_HEIGHT - texH) / 2;
            graphics.renderTexture(stateText, x, y);
            SDL_DestroyTexture(stateText);
        }
    }

    SDL_RenderPresent(graphics.renderer);
}



#endif

