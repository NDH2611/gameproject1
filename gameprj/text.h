/*#ifndef _TEXT__H
#define _TEXT__H

#include "defs.h"
#include "graphics.h"

TTF_Font* loadFont(Graphics &graphics, const char* path, int size)
{
    TTF_Font* gFont = TTF_OpenFont( path, size );
    if (gFont == nullptr) {
        graphics.SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
    }
    return gFont;
}

SDL_Texture* renderText(Graphics &graphics, const char* text, TTF_Font* font, SDL_Color textColor)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
    if( textSurface == nullptr ) {
        graphics.SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface( graphics.renderer, textSurface );
    if( texture == nullptr ) {
        graphics.SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
    }

    SDL_FreeSurface( textSurface );
    return texture;
}

TTF_Font* font = loadFont(graphics, "Purisa-BoldOblique.ttf", 100);
void inscore(Graphics &graphics, int score)
{

    SDL_Color color = {255, 255, 0, 0};
    for(int i = 10; i > 0; i--)
    {
        score += 10;

        SDL_Texture* helloText = graphics.renderText(to_string(score).c_str(), font, color);
        graphics.renderTexture(helloText, 50, 50);
        SDL_DestroyTexture( helloText );

        SDL_Delay(100);
        graphics.presentScene();
        SDL_RenderClear(graphics.renderer);
    }
    TTF_CloseFont( font );
}

#endif
*/
