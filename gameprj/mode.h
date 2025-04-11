#ifndef MODE__H
#define MODE__H

enum ModeState {
    STATE_MODE,
    EASY_MODE,
    HARD_MODE,
    ASIAN_MODE
};

struct ModeButton {
    SDL_Rect rect;
    const char* text;
    ModeState nextModeState;
};

const vector<ModeButton> modeButtons = {
    {{330, 230, 150, 50}, "EASY", EASY_MODE},
    {{330, 300, 150, 50}, "HARD", HARD_MODE},
    {{330, 370, 150, 50}, "ASIAN", ASIAN_MODE}
};

void renderMode(Graphics &graphics, TTF_Font* font, SDL_Texture* background, const vector<ModeButton>& buttons)
{
    SDL_RenderClear(graphics.renderer);
    graphics.renderTexture(background, 0, 0);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color buttonColor = {0, 128, 255, 100};
    SDL_SetRenderDrawBlendMode(graphics.renderer, SDL_BLENDMODE_BLEND);

    for (const auto& button : buttons) {
        SDL_SetRenderDrawColor(graphics.renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
        SDL_RenderFillRect(graphics.renderer, &button.rect);

        SDL_Texture* buttonText = graphics.renderText(button.text, font, white);

        int textW, textH;
        SDL_QueryTexture(buttonText, NULL, NULL, &textW, &textH);

        int textX = button.rect.x + (button.rect.w - textW) / 2;
        int textY = button.rect.y + (button.rect.h - textH) / 2;

        graphics.renderTexture(buttonText, textX, textY);
        SDL_DestroyTexture(buttonText);
    }

    SDL_RenderPresent(graphics.renderer);
}

ModeState handleModeEvent(SDL_Event& event, int mouseX, int mouseY, const vector<ModeButton>& buttons) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        SDL_Point point = {mouseX, mouseY};
        for (const auto& button : buttons) {
            if (SDL_PointInRect(&point, &button.rect)) {
                return button.nextModeState;
            }
        }
    }
    return STATE_MODE;
}

#endif

