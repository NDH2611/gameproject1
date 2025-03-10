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

    int wall;
    int tilesetColumns;
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
            if (tilesetTexture == nullptr) logErrorAndExit("LoadtilesetTexture", SDL_GetError());

        loadMap("mapprj.tmj");
            if (!loadMap("mapprj.tmj")) logErrorAndExit("Loadmap", SDL_GetError());
        renderMap();


        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    void prepareScene()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

	void prepareScene(SDL_Texture * background)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy( renderer, background, NULL, NULL);
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

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

    void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
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

    void renderLayer(const vector<int>& tileData) {
        SDL_Rect srcRect, destRect;
        srcRect.w = destRect.w = TILE_SIZE;
        srcRect.h = destRect.h = TILE_SIZE;

        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int tileIndex = tileData[y * MAP_WIDTH + x] - 1;
                if (tileIndex < 0) continue;

                srcRect.x = (tileIndex % tilesetColumns) * TILE_SIZE;
                srcRect.y = (tileIndex / tilesetColumns) * TILE_SIZE;

                destRect.x = x * TILE_SIZE;
                destRect.y = y * TILE_SIZE;

                SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
            }
        }
    }

    void renderMap() {
        for (const auto& layer : layersData) {
            renderLayer(layer);
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

    void quit()
    {
        IMG_Quit();
        SDL_DestroyTexture(tilesetTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }
};

#endif


