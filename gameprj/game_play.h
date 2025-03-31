#ifndef GAME__PLAY__H
#define GAME__PLAY__H

void running_Main_Game(Graphics &graphics, Mouse &mouse, SDL_Texture* scoretxt, TTF_Font* font) {
    bool running = true;

    int score = 0;
    SDL_Color color_score = {255, 255, 255, 255};
    SDL_Texture* iscore = graphics.renderText(to_string(score).c_str(), font, color_score);

    SDL_Event event;

    while (running) {
        Uint32 remainingTime = time_count_down(startTime, countdownTime);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {
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
            if (event.type == SDL_KEYUP) {
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

        if (!graphics.isCollision(mouse.x + mouse.dx, mouse.y + mouse.dy, DESTINATION, DESTINATION)) {
            mouse.move();
            if (isCollisionWithdiamond(graphics, mouse.x, mouse.y, DESTINATION, DESTINATION)) {
                removeDiamond(graphics, mouse.x, mouse.y, DESTINATION, DESTINATION);
                score += 1;
                SDL_DestroyTexture(iscore);
                iscore = graphics.renderText(to_string(score).c_str(), font, color_score);
            }
        }

        if (remainingTime == 0) {
            running = false;
        }

        SDL_RenderClear(graphics.renderer);
        graphics.renderMap();

        graphics.renderTexture(scoretxt, 12, 765);
        graphics.renderTexture(iscore, 112, 765);

        render(mouse, graphics);
        SDL_RenderPresent(graphics.renderer);

        SDL_Delay(10);
    }
}

#endif
