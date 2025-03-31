#ifndef TIME_H
#define TIME_H

#include <SDL.h>

Uint32 startTime = SDL_GetTicks();
Uint32 countdownTime = Time_EASY * 1000;

Uint32 time_count_down(Uint32 startTime, Uint32 countdownTime) {
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    Uint32 remainingTime = (countdownTime > elapsedTime) ? countdownTime - elapsedTime : 0;
    return remainingTime;
}

#endif
