#ifndef TIME_H
#define TIME_H

Uint32 remainingTime = 0;
Uint32 cachedRemainingTime = 0;
Uint32 countdownTime;
Uint32 startTime = SDL_GetTicks(), lastTime = SDL_GetTicks();
Uint32 totalPausedTime = 0, pauseStartTime = 0;

Uint32 time_count_down(Uint32 startTime, Uint32 countdownTime, Uint32 totalPausedTime) {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsed = currentTime - startTime - totalPausedTime;
    return (countdownTime > elapsed) ? (countdownTime - elapsed) : 0;
}

void resetTimer() {
    startTime = SDL_GetTicks();
    lastTime = startTime;
    totalPausedTime = 0;
    cachedRemainingTime = 0;
}
#endif
