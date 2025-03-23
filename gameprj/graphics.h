#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

struct Graphics {
    SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture* tilesetTexture;
    SDL_Texture* tilesetDiamond;

    int wall;
    int diamond;
    int tilesetColumns;
    int tilesetColumnsDiamond;
    int firstgidTilesetTexture = 1;
    int firstgidTilesetDiamond = 1;
    vector<vector<int>> layersData;

	void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

	void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) logErrorAndExit( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        tilesetTexture = IMG_LoadTexture(renderer, "mapmat.jpg");
        tilesetDiamond = IMG_LoadTexture(renderer, "item.jpg");
            if (tilesetTexture == nullptr) logErrorAndExit("LoadtilesetTexture", SDL_GetError());
            if (tilesetDiamond == nullptr) logErrorAndExit("LoadtilesetDiamond", SDL_GetError());

        loadMap("mapprj.tmj");
            if (!loadMap("mapprj.tmj")) logErrorAndExit("Loadmap", SDL_GetError());
        renderMap();


        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ", TTF_GetError());
        }
    }

    bool loadMap(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "can't open file" << filename << "\n";
            return false;
        }

        json mapJson;
        try {
            file >> mapJson;
        } catch (const exception& e) {
            cerr << "read err JSON " << e.what() << "\n";
            return false;
        }
        file.close();

        if (!mapJson.contains("tilesets") || mapJson["tilesets"].empty() || !mapJson["tilesets"][0].contains("columns")) {
            cerr << "can't find tileset\n";
            return false;
        }
        tilesetColumns = mapJson["tilesets"][0]["columns"];
        tilesetColumnsDiamond = mapJson["tilesets"][1]["columns"];

        for (const auto& tileset : mapJson["tilesets"]) {
            int firstgid = tileset["firstgid"];

            if (tileset.contains("name")) {
                string name = tileset["name"];

                if (name == "mapmat") {
                    firstgidTilesetTexture = firstgid;
                }
                if (name == "item") {
                    firstgidTilesetDiamond = firstgid;
                }
            }
        }

        if (!mapJson.contains("layers") || mapJson["layers"].empty()) {
            cerr << "can't find layer\n";
            return false;
        }

        layersData.clear();
        for (const auto& layer : mapJson["layers"]) {
            if (!layer.contains("data")) {
                cerr << "layer have no data\n";
                continue;
            }
            layersData.push_back(layer["data"].get<vector<int>>());
        }

        return true;
    }

    void renderLayer(const vector<int>& tileData, SDL_Texture* texture, int tilesetColumns, int firstgid) {
        SDL_Rect srcRect, destRect;
        srcRect.w = destRect.w = TILE_SIZE;
        srcRect.h = destRect.h = TILE_SIZE;

        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int tileID = tileData[y * MAP_WIDTH + x];
                if (tileID == 0) continue;

                int tileIndex = tileID - firstgid;
                if (tileIndex < 0) continue;

                srcRect.x = (tileIndex % tilesetColumns) * TILE_SIZE;
                srcRect.y = (tileIndex / tilesetColumns) * TILE_SIZE;
                destRect.x = x * TILE_SIZE;
                destRect.y = y * TILE_SIZE;

                SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
            }
        }
    }

    void renderMap() {
        for (size_t i = 0; i < layersData.size(); ++i) {
            if (i == 1) {
                renderLayer(layersData[i], tilesetDiamond, tilesetColumnsDiamond, firstgidTilesetDiamond);
            } else {

                renderLayer(layersData[i], tilesetTexture, tilesetColumns, firstgidTilesetTexture);
            }
        }
    }

    bool isCollision(int x, int y, int width, int height) {
        int left = x / TILE_SIZE;
        int right = (x + width - 1) / TILE_SIZE;
        int top = y / TILE_SIZE;
        int bottom = (y + height - 1) / TILE_SIZE;

        if (left < 0 || right >= MAP_WIDTH || top < 0 || bottom >= MAP_HEIGHT)
            return true;

        if (layersData[wall][top * MAP_WIDTH + left] != 0 ||
            layersData[wall][top * MAP_WIDTH + right] != 0 ||
            layersData[wall][bottom * MAP_WIDTH + left] != 0 ||
            layersData[wall][bottom * MAP_WIDTH + right] != 0)
            return true;

        return false;
    }

    TTF_Font* loadFont(const char* path, int size)
    {
        TTF_Font* gFont = TTF_OpenFont( path, size );
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
        }
        return gFont;
    }

    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
        }

        SDL_FreeSurface( textSurface );
        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void quit()
    {
        IMG_Quit();
        TTF_Quit();
        SDL_DestroyTexture(tilesetTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }
};

#endif


