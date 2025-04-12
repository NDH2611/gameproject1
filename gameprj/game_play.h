#ifndef GAME_PLAY_H
#define GAME_PLAY_H
#include "game_state.h"

void resetGame(Graphics &graphics, Mouse &mouse, int &score, SDL_Texture* &iscore,
               TTF_Font* font, SDL_Color color_score,
               Uint32 &startTime, Uint32 &totalPausedTime,
               Uint32 &remainingTime, GameState &gameState)
{
    score = 0;
    mouse.reset();

    if (iscore) SDL_DestroyTexture(iscore);
    iscore = graphics.renderText(to_string(score).c_str(), font, color_score);

    graphics.layersData[graphics.diamond] = graphics.originalDiamondLayer;

    startTime = SDL_GetTicks();
    totalPausedTime = 0;

    remainingTime = time_count_down(startTime, countdownTime, totalPausedTime) / 1000;
    gameState = GAME_RUNNING;
}

void running_Main_Game(Graphics &graphics, Mouse &mouse, SDL_Texture* scoretxt, SDL_Texture* timetxt, TTF_Font* font) {

    GameState gameState;
    gameState = GAME_RUNNING;
    bool running = true;
    int score = 0;
    SDL_Color color_score = {255, 255, 255, 255};
    SDL_Texture* iscore = graphics.renderText(to_string(score).c_str(), font, color_score);
    SDL_Texture* iTime = nullptr;
    Uint32 previousRemainingTime = -1;
    SDL_Event event;

    while (running) {
        if (gameState == GAME_RUNNING) {
            remainingTime = time_count_down(startTime, countdownTime, totalPausedTime)/1000;
            cachedRemainingTime = remainingTime;
        }
        else {
            remainingTime = cachedRemainingTime;
        }
        if (remainingTime != previousRemainingTime) {
            previousRemainingTime = remainingTime;
            if (iTime) SDL_DestroyTexture(iTime);
            iTime = graphics.renderText(to_string(remainingTime).c_str(), font, color_score);
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            keyboard(event, mouse);

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_P) {
                    gameState = (gameState == GAME_RUNNING) ? GAME_PAUSED : GAME_RUNNING;

                    if (gameState == GAME_RUNNING && pauseStartTime != 0) {
                        totalPausedTime += SDL_GetTicks() - pauseStartTime;
                        pauseStartTime = 0;
                    }
                    else if (gameState == GAME_PAUSED) {
                        pauseStartTime = SDL_GetTicks();
                    }
                }
                else if (gameState == GAME_OVER) {
                    if(event.key.keysym.scancode == SDL_SCANCODE_R)
                    {
                        resetGame(graphics, mouse, score, iscore, font, color_score, startTime, totalPausedTime, remainingTime, gameState);
                    }
                }
            }
        }

        if (gameState != GAME_RUNNING) {
            graphics.renderCachedMap();
            renderGameState(graphics, mouse, scoretxt, timetxt, font, gameState, iscore, iTime);
            SDL_Delay(16);
            lastTime = SDL_GetTicks();
            continue;
        }

        if (!graphics.isCollision(mouse.x + mouse.dx, mouse.y + mouse.dy, DESTINATION, DESTINATION)) {
            mouse.move();
            if (isCollisionWithdiamond(graphics, mouse.x, mouse.y, DESTINATION, DESTINATION)) {
                removeDiamond(graphics, mouse.x, mouse.y, DESTINATION, DESTINATION);
                score++;
                SDL_DestroyTexture(iscore);
                iscore = graphics.renderText(to_string(score).c_str(), font, color_score);
            }
        }

        if (remainingTime == 0) gameState = GAME_OVER;

        graphics.renderCachedMap();
        renderGameState(graphics, mouse, scoretxt, timetxt, font, gameState, iscore, iTime);

        Uint32 frameTime = SDL_GetTicks() - lastTime;
        if (frameTime < 10) SDL_Delay(10 - frameTime);
        lastTime = SDL_GetTicks();
    }
    SDL_DestroyTexture(iscore);
    SDL_DestroyTexture(iTime);
}

#endif
