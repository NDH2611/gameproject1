//#ifndef _GRAPHICS__H
//#define _GRAPHICS__H
//
//using json = nlohmann::json;
//using namespace std;
//
//struct Graphics {
//    SDL_Renderer *renderer;
//	SDL_Window *window;
//	SDL_Texture* tilesetTexture;
//    SDL_Texture* tilesetDiamond;
//    SDL_Texture* cachedMapTexture = nullptr;
//
//    int wall;
//    int diamond;
//    int tilesetColumns;
//    int tilesetColumnsDiamond;
//    int firstgidTilesetTexture = 1;
//    int firstgidTilesetDiamond = 1;
//    vector<vector<int>> layersData;
//    vector<int> originalDiamondLayer;
//
//	void logErrorAndExit(const char* msg, const char* error)
//    {
//        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
//        SDL_Quit();
//    }
//
//    void cacheRenderMap() {
//        cachedMapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
//        SDL_SetRenderTarget(renderer, cachedMapTexture);
//        renderMap();
//        SDL_SetRenderTarget(renderer, nullptr);
//    }
//
//    void renderCachedMap() {
//        if (cachedMapTexture) {
//            SDL_RenderCopy(renderer, cachedMapTexture, nullptr, nullptr);
//        }
//    }
//
//	void init() {
//        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) logErrorAndExit("SDL_Init", SDL_GetError());
//
//        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//            if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());
//
//        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) logErrorAndExit( "SDL_image error:", IMG_GetError());
//
//        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//            if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());
//
//        tilesetTexture = IMG_LoadTexture(renderer, "mapmat.jpg");
//        tilesetDiamond = IMG_LoadTexture(renderer, "item.jpg");
//                            /*
//                            SDL_Surface* tempSurface = IMG_Load("item.jpg");
//                            if (!tempSurface) logErrorAndExit("LoadSurface for item.jpg", IMG_GetError());
//
//                            SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 98, 96, 96));
//
//                            tilesetDiamond = SDL_CreateTextureFromSurface(renderer, tempSurface);
//                            if (!tilesetDiamond) logErrorAndExit("CreateTextureFromSurface for item.jpg", SDL_GetError());
//
//                            SDL_FreeSurface(tempSurface);
//                            */
//            if (tilesetTexture == nullptr) logErrorAndExit("LoadtilesetTexture", SDL_GetError());
//            if (tilesetDiamond == nullptr) logErrorAndExit("LoadtilesetDiamond", SDL_GetError());
//
//        loadMap("mapprj.tmj");
//            if (!loadMap("mapprj.tmj")) logErrorAndExit("Loadmap", SDL_GetError());
//        renderMap();
//
//
//        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
//        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
//
//        if (TTF_Init() == -1) {
//            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ", TTF_GetError());
//        }
//    }
//
//    bool loadMap(const string& filename) {
//        ifstream file(filename);
//        if (!file.is_open()) {
//            cerr << "can't open file" << filename << "\n";
//            return false;
//        }
//
//        json mapJson;
//        try {
//            file >> mapJson;
//        } catch (const exception& e) {
//            cerr << "read err JSON " << e.what() << "\n";
//            return false;
//        }
//        file.close();
//
//        if (!mapJson.contains("tilesets") || mapJson["tilesets"].empty() || !mapJson["tilesets"][0].contains("columns")) {
//            cerr << "can't find tileset\n";
//            return false;
//        }
//        tilesetColumns = mapJson["tilesets"][0]["columns"];
//        tilesetColumnsDiamond = mapJson["tilesets"][1]["columns"];
//
//        for (const auto& tileset : mapJson["tilesets"]) {
//            int firstgid = tileset["firstgid"];
//
//            if (tileset.contains("name")) {
//                string name = tileset["name"];
//
//                if (name == "mapmat") {
//                    firstgidTilesetTexture = firstgid;
//                }
//                if (name == "item") {
//                    firstgidTilesetDiamond = firstgid;
//                }
//            }
//        }
//
//        if (!mapJson.contains("layers") || mapJson["layers"].empty()) {
//            cerr << "can't find layer\n";
//            return false;
//        }
//
//        layersData.clear();
//        for (const auto& layer : mapJson["layers"]) {
//            if (!layer.contains("data")) {
//                cerr << "layer have no data\n";
//                continue;
//            }
//            auto layerData = layer["data"].get<vector<int>>();
//            layersData.push_back(layerData);
//
//            if (layer["name"] == "diamond") {
//                originalDiamondLayer = layerData;
//            }
//        }
//
//        return true;
//    }
//
//    void renderLayer(const vector<int>& tileData, SDL_Texture* texture, int tilesetColumns, int firstgid) {
//        SDL_Rect srcRect, destRect;
//        srcRect.w = destRect.w = TILE_SIZE;
//        srcRect.h = destRect.h = TILE_SIZE;
//
//        for (int y = 0; y < MAP_HEIGHT; ++y) {
//            for (int x = 0; x < MAP_WIDTH; ++x) {
//                int tileID = tileData[y * MAP_WIDTH + x];
//                if (tileID == 0) continue;
//
//                int tileIndex = tileID - firstgid;
//                if (tileIndex < 0) continue;
//
//                srcRect.x = (tileIndex % tilesetColumns) * TILE_SIZE;
//                srcRect.y = (tileIndex / tilesetColumns) * TILE_SIZE;
//                destRect.x = x * TILE_SIZE;
//                destRect.y = y * TILE_SIZE;
//
//                SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
//            }
//        }
//    }
//
//    void renderMap() {
//        for (size_t i = 0; i < layersData.size(); ++i) {
//            if (i == 1) {
//                renderLayer(layersData[i], tilesetDiamond, tilesetColumnsDiamond, firstgidTilesetDiamond);
//            } else {
//
//                renderLayer(layersData[i], tilesetTexture, tilesetColumns, firstgidTilesetTexture);
//            }
//        }
//    }
//
//    bool isCollision(int x, int y, int width, int height) {
//        int left = x / TILE_SIZE;
//        int right = (x + width - 1) / TILE_SIZE;
//        int top = y / TILE_SIZE;
//        int bottom = (y + height - 1) / TILE_SIZE;
//
//        if (left < 0 || right >= MAP_WIDTH || top < 0 || bottom >= MAP_HEIGHT)
//            return true;
//
//        if (layersData[wall][top * MAP_WIDTH + left] != 0 ||
//            layersData[wall][top * MAP_WIDTH + right] != 0 ||
//            layersData[wall][bottom * MAP_WIDTH + left] != 0 ||
//            layersData[wall][bottom * MAP_WIDTH + right] != 0)
//            return true;
//
//        return false;
//    }
//
//    TTF_Font* loadFont(const char* path, int size)
//    {
//        TTF_Font* gFont = TTF_OpenFont( path, size );
//        if (gFont == nullptr) {
//            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
//        }
//        return gFont;
//    }
//
//    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor)
//    {
//        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
//        if( textSurface == nullptr ) {
//            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
//            return nullptr;
//        }
//
//        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
//        if( texture == nullptr ) {
//            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
//        }
//
//        SDL_FreeSurface( textSurface );
//        return texture;
//    }
//
//    void renderTexture(SDL_Texture *texture, int x, int y)
//    {
//        SDL_Rect dest;
//
//        dest.x = x;
//        dest.y = y;
//        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
//
//        SDL_RenderCopy(renderer, texture, NULL, &dest);
//    }
//
//    SDL_Texture *loadTexture(const char *filename)
//    {
//        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
//
//        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
//        if (texture == NULL)
//            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
//
//        return texture;
//    }
//
//    void quit()
//    {
//        IMG_Quit();
//        TTF_Quit();
//        SDL_DestroyTexture(tilesetTexture);
//        SDL_DestroyRenderer(renderer);
//        SDL_DestroyWindow(window);
//
//        SDL_Quit();
//    }
//};
//
//#endif

#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <unordered_map>
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace std;

struct Graphics {
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    unordered_map<string, SDL_Texture*> textureCache;

    SDL_Texture* cachedMapTexture = nullptr;
    SDL_Texture* tilesetTexture = nullptr;
    SDL_Texture* tilesetDiamond = nullptr;

    int wall;
    int diamond;
    int tilesetColumns;
    int tilesetColumnsDiamond;
    int firstgidTilesetTexture = 1;
    int firstgidTilesetDiamond = 1;
    vector<vector<int>> layersData;
    vector<int> originalDiamondLayer;

    void logErrorAndExit(const char* msg, const char* error) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_Texture* getOrLoadTexture(const string& path) {
        if (textureCache.find(path) == textureCache.end()) {
            SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
            if (!texture) logErrorAndExit("LoadTexture", SDL_GetError());
            textureCache[path] = texture;
        }
        return textureCache[path];
    }

    void freeAllTextures() {
        for (auto& pair : textureCache) {
            SDL_DestroyTexture(pair.second);
        }
        textureCache.clear();
    }

    void cacheRenderMap() {
        cachedMapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_SetRenderTarget(renderer, cachedMapTexture);
        renderMap();
        SDL_SetRenderTarget(renderer, nullptr);
    }

    void renderCachedMap() {
        if (cachedMapTexture) {
            SDL_RenderCopy(renderer, cachedMapTexture, nullptr, nullptr);
        }
    }

    void renderTexture(SDL_Texture* texture, int x, int y) {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    SDL_Texture* loadTexture(const char* filename) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
        SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
        }
        return texture;
    }

    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
        if (textSurface == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
        }

        SDL_FreeSurface(textSurface);
        return texture;
    }

    void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0 ||
            (window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)) == nullptr ||
            !IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) ||
            (renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == nullptr) {
            logErrorAndExit("Initialization Failed", SDL_GetError());
        }

        tilesetTexture = getOrLoadTexture("mapmat.jpg");
        tilesetDiamond = getOrLoadTexture("item.jpg");

        if (!loadMap("mapprj.tmj")) {
            logErrorAndExit("Load Map Failed", "");
        }

        cacheRenderMap();
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf Initialization Failed", TTF_GetError());
        }
    }

    bool readJsonFile(const string& filename, json& mapJson) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Can't open file: " << filename << "\n";
            return false;
        }
        try {
            file >> mapJson;
        } catch (const exception& e) {
            cerr << "Error reading JSON: " << e.what() << "\n";
            return false;
        }
        file.close();
        return true;
    }

    bool parseTilesets(const json& mapJson) {
        if (!mapJson.contains("tilesets") || mapJson["tilesets"].empty() || !mapJson["tilesets"][0].contains("columns")) {
            cerr << "Can't find tilesets\n";
            return false;
        }

        tilesetColumns = mapJson["tilesets"][0]["columns"];
        tilesetColumnsDiamond = mapJson["tilesets"][1]["columns"];

        for (const auto& tileset : mapJson["tilesets"]) {
            int firstgid = tileset["firstgid"];
            if (tileset.contains("name")) {
                string name = tileset["name"];
                if (name == "mapmat") firstgidTilesetTexture = firstgid;
                if (name == "item") firstgidTilesetDiamond = firstgid;
            }
        }
        return true;
    }

    bool parseLayers(const json& mapJson) {
        if (!mapJson.contains("layers") || mapJson["layers"].empty()) {
            cerr << "Can't find layers\n";
            return false;
        }

        layersData.clear();
        for (const auto& layer : mapJson["layers"]) {
            if (!layer.contains("data")) {
                cerr << "Layer has no data\n";
                continue;
            }

            auto layerData = layer["data"].get<vector<int>>();
            layersData.push_back(layerData);

            if (layer["name"] == "diamond") {
                originalDiamondLayer = layerData;
            }
        }
        return true;
    }

    bool loadMap(const string& filename) {
        json mapJson;
        if (!readJsonFile(filename, mapJson)) {
            return false;
        }
        if (!parseTilesets(mapJson) || !parseLayers(mapJson)) {
            return false;
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
                if (tileID <= 0) continue;

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
            SDL_Texture* currentTexture = (i == 1) ? tilesetDiamond : tilesetTexture;
            int columns = (i == 1) ? tilesetColumnsDiamond : tilesetColumns;
            int firstgid = (i == 1) ? firstgidTilesetDiamond : firstgidTilesetTexture;

            renderLayer(layersData[i], currentTexture, columns, firstgid);
        }
    }

    bool isCollision(int x, int y, int width, int height) {
        int left = x / TILE_SIZE;
        int right = (x + width - 1) / TILE_SIZE;
        int top = y / TILE_SIZE;
        int bottom = (y + height - 1) / TILE_SIZE;

        if (left < 0 || right >= MAP_WIDTH || top < 0 || bottom >= MAP_HEIGHT)
            return true;

        return layersData[wall][top * MAP_WIDTH + left] != 0 ||
               layersData[wall][top * MAP_WIDTH + right] != 0 ||
               layersData[wall][bottom * MAP_WIDTH + left] != 0 ||
               layersData[wall][bottom * MAP_WIDTH + right] != 0;
    }

    TTF_Font* loadFont(const char* path, int size) {
        TTF_Font* gFont = TTF_OpenFont(path, size);
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
        }
        return gFont;
    }

    void quit() {
        freeAllTextures();
        IMG_Quit();
        TTF_Quit();
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif
