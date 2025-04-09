#ifndef GAME_PLAY_H
#define GAME_PLAY_H
#include "game_state.h"

void running_Main_Game(Graphics &graphics, Mouse &mouse, SDL_Texture* scoretxt, SDL_Texture* timetxt, TTF_Font* font) {
    GameState gameState = GAME_RUNNING;
    bool running = true;
    int score = 0;
    SDL_Color color_score = {255, 255, 255, 255};
    SDL_Texture* iscore = graphics.renderText(to_string(score).c_str(), font, color_score);
    SDL_Event event;

    while (running) {

        if (gameState == GAME_RUNNING) {
            remainingTime = time_count_down(startTime, countdownTime, totalPausedTime)/1000;
            cachedRemainingTime = remainingTime;  // Lưu lại để dùng khi pause
        }
        else {
            remainingTime = cachedRemainingTime;  // Không cập nhật khi pause
        }
        SDL_Texture* iTime = graphics.renderText(to_string(remainingTime).c_str(), font, color_score);

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
                        score = 0;
                        mouse.x = SCREEN_WIDTH / 2;
                        mouse.y = 752;
                        SDL_DestroyTexture(iscore);
                        iscore = graphics.renderText(to_string(score).c_str(), font, color_score);
                        graphics.layersData[graphics.diamond] = graphics.originalDiamondLayer;
                        startTime = SDL_GetTicks();
                        remainingTime = time_count_down(startTime, countdownTime, totalPausedTime)/1000;
                        totalPausedTime = 0;
                        gameState = GAME_RUNNING;
                    }

                }
            }
        }

        if (gameState != GAME_RUNNING) {
            // Render the game state if it's paused or game over
            renderGameState(graphics, mouse, scoretxt, timetxt, font, gameState, iscore, iTime);
            SDL_DestroyTexture(iTime);
            SDL_Delay(16);  // Smooth pause rendering
            lastTime = SDL_GetTicks();
            continue;
        }

        // Move the mouse and check for collisions
        if (!graphics.isCollision(mouse.x + mouse.dx, mouse.y + mouse.dy, DESTINATION, DESTINATION)) {
            mouse.move();
            if (isCollisionWithdiamond(graphics, mouse.x, mouse.y, DESTINATION, DESTINATION)) {
                removeDiamond(graphics, mouse.x, mouse.y, DESTINATION, DESTINATION);
                score++;
                SDL_DestroyTexture(iscore);
                iscore = graphics.renderText(to_string(score).c_str(), font, color_score);
            }
        }

        // If the remaining time reaches zero, end the game
        if (remainingTime == 0) gameState = GAME_OVER;

        // Render the game state
        renderGameState(graphics, mouse, scoretxt, timetxt, font, gameState, iscore, iTime);
        SDL_DestroyTexture(iTime);

        // Control frame rate
        Uint32 frameTime = SDL_GetTicks() - lastTime;
        if (frameTime < 10) SDL_Delay(10 - frameTime);
        lastTime = SDL_GetTicks();
    }

    // Cleanup
    SDL_DestroyTexture(iscore);
}

#endif
