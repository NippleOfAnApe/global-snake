/*******************************************************************************************
*
*    Орлині крила маєм за плечима,
*    Самі ж кайданами прикуті до землі.
*
*                       Леся Українка
*
********************************************************************************************/

#include <cstddef>
#include <stdlib.h>
#include <raylib.h>
#include "Classes.h"

//----------------------------------------------------------------------------------
// Variables/Objects
//----------------------------------------------------------------------------------

static const unsigned short WIDTH_PX = 500;
static const unsigned short HEIGHT_PX = 500;
static const float TILE_SIZE = 512.0f;
static float borderWidth = 40;
static float offMapSize = 110; //how many pixels to fit outside the map in the screen when near borders

static Texture2D texPalette[7] = { 0 };
static Color* colors = { 0 };
static Texture2D bgTexture = { 0 };
static Texture2D wallTexture = { 0 };

//Map dimension calculations
static unsigned short xPreLoadTile = 3;     // how many tiles to render in each axis from player
static unsigned short yPreLoadTile = 3;
static unsigned char** tileMapCoordinates = { 0 };
static int theExtra = 0;    // extra space needed for drawing bg and fg
static float* dimensions;

static const float MAP_WIDTH = WIDTH_PX * TILE_SIZE;
static const float MAP_HEIGHT = HEIGHT_PX * TILE_SIZE;

//----------------------------------------------------------------------------------
// Map Functions Definition
//----------------------------------------------------------------------------------

void Map::Init(void)
{
    // Class variables to constants
    mapPX = WIDTH_PX;
    tileSize = TILE_SIZE;
    mapWidth = MAP_WIDTH;
    mapHeight = MAP_HEIGHT;
    
    // Each pixel is a tile. Loads an array of Colors, then parse it to const chars
    // Sets a tile pallete to a vallue according to its RGB values
    colors = LoadImageColors(LoadImage("../resources/textures/BWMap.png"));
    tileMapCoordinates = AssignColors(colors);

    texPalette[WATER] = LoadTexture("../resources/textures/03_Water.png");
    texPalette[SAND] = LoadTexture("../resources/textures/23_Sand.png");
    texPalette[ROCK] = LoadTexture("../resources/textures/04_Ground.png");
    texPalette[DIRT] = LoadTexture("../resources/textures/10_Dirt.png");
    texPalette[GRASS1] = LoadTexture("../resources/textures/15_Grass.png");
    texPalette[GRASS2] = LoadTexture("../resources/textures/18_Grass.png");
    texPalette[GRASS3] = LoadTexture("../resources/textures/20_Grass.png");

    bgTexture = LoadTexture("../resources/textures/04Dirt1920x1080.png");
    wallTexture = LoadTexture("../resources//textures/stone480.png");
    
    // Dummy value for adjusting how to display BG
    theExtra = borderWidth * 2 + offMapSize * 2;
}

float* Map::GetDimensions(void)
{
    dimensions = (float*)RL_MALLOC(sizeof(float) * 3);
    *dimensions = borderWidth;
    *(dimensions + 1) = mapWidth;
    *(dimensions + 2) = mapHeight;

    return dimensions;
}


void Map::Draw(Player *player)
{
    // BG when player is close to borders
    if (player->tileXPos <= 1 || player->tileXPos >= WIDTH_PX - 2 || player->tileYPos <= 1 || player->tileYPos >= HEIGHT_PX - 2)
    {
        DrawTextureTiled(bgTexture, (Rectangle){0.0f, 0.0f, 1920.0f, 1280.0f}, (Rectangle){-offMapSize - borderWidth, -offMapSize - borderWidth, mapWidth + theExtra, mapHeight + theExtra}, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
        // Borders
        DrawTextureTiled(wallTexture, (Rectangle){0.0f, 0.0f, 480.0f, 480.0f}, (Rectangle){-borderWidth, -borderWidth, mapWidth + borderWidth, borderWidth}, (Vector2){0.0f, 0.0f}, 0.0f, .5f, WHITE);
        DrawTextureTiled(wallTexture, (Rectangle){0.0f, 0.0f, 480.0f, 480.0f}, (Rectangle){-borderWidth, 0, borderWidth, mapHeight}, (Vector2){0.0f, 0.0f}, 0.0f, .5f, WHITE);
        DrawTextureTiled(wallTexture, (Rectangle){0.0f, 0.0f, 480.0f, 480.0f}, (Rectangle){-borderWidth, mapHeight, mapWidth + borderWidth, borderWidth}, (Vector2){0.0f, 0.0f}, 0.0f, .5f, WHITE);
        DrawTextureTiled(wallTexture, (Rectangle){0.0f, 0.0f, 480.0f, 480.0f}, (Rectangle){mapWidth, -borderWidth, borderWidth, mapHeight + borderWidth * 2}, (Vector2){0.0f, 0.0f}, 0.0f, .5f, WHITE);
    }

    //FG
    //Clamp iterators to 0 or MAP
    for (std::size_t i = MAX((player->tileYPos - yPreLoadTile), 0); i < MIN((player->tileYPos + yPreLoadTile + 1), HEIGHT_PX); i++)
    {
        for (std::size_t j = MAX((player->tileXPos - xPreLoadTile), 0); j < MIN((player->tileXPos + xPreLoadTile + 1), WIDTH_PX); j++)
        {
            DrawTexture(texPalette[tileMapCoordinates[i][j]], j * tileSize, i * tileSize, WHITE);
            // DrawText(TextFormat("[ %d : %d ]", j, i), j * tileSize + tileSize / 2 - (float)MeasureText(TextFormat("[ %d : %d ]", j, i), 46) / 2, i * tileSize + tileSize / 2, 46, BLACK);
            // DrawText(TextFormat("[%d : %d : %d]", colors[i * mapSize + j].r, colors[i * mapSize + j].g, colors[i * mapSize + j].b),
            //         j * tileSize + tileSize / 2 - (float)MeasureText(TextFormat("[%d : %d : %d]", colors[i * mapSize + j].r, colors[i * mapSize + j].g, colors[i * mapSize + j].b), 38) / 2,
            //         i * tileSize + tileSize / 1.5,
            //         38, PURPLE);
        }
    }
}

void Map::UpdateCameraCenterInsideMap(Camera2D *camera, Snake *target, const float screenWidth, const float screenHeight)
{
    Vector2 localTarget = target->position;
    float scrWidth = (float)screenWidth;
    float scrHeight = (float)scrHeight;

    float minX = -borderWidth - offMapSize;
    float minY = -borderWidth - offMapSize;
    float maxX = mapWidth + borderWidth + offMapSize;
    float maxY = mapHeight + borderWidth + offMapSize;

    camera->target = localTarget;
    camera->offset = (Vector2){ scrWidth/2.0f, scrHeight/2.0f };

    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);
    
    if (max.x < scrWidth) camera->offset.x = scrWidth - (max.x - scrWidth/2.0f);
    if (max.y < scrHeight) camera->offset.y = scrHeight - (max.y - scrHeight/2.0f);
    if (min.x > 0) camera->offset.x = scrWidth/2.0f - min.x;
    if (min.y > 0) camera->offset.y = scrHeight/2.0f - min.y;
}

void Map::UpdateCameraCenter(Camera2D *camera, Snake* target, const float screenWidth, const float screenHeight)
{
    camera->target = target->position;
    camera->offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
}

unsigned char** Map::AssignColors(Color* colors)
{
    //Creates a 2D array of char pointers for easy access
    unsigned char** collArray = (unsigned char**)RL_MALLOC(HEIGHT_PX * sizeof(unsigned char*));
    for (std::size_t i = 0; i < HEIGHT_PX; i++) collArray[i] = (unsigned char*)RL_MALLOC(WIDTH_PX * sizeof(unsigned char));

    for (std::size_t y = 0; y < HEIGHT_PX; y++)
    {
        for (std::size_t x = 0; x < WIDTH_PX; x++)
        {
            // Black magic
            if (colors[y * HEIGHT_PX + x].b >= 220 &&
                colors[y * HEIGHT_PX + x].r <= 220 &&
                colors[y * HEIGHT_PX + x].g <= 220) collArray[y][x] = WATER;

            if (colors[y * HEIGHT_PX + x].r <= 10) collArray[y][x] = SAND;
            else if (colors[y * HEIGHT_PX + x].r <= 50) collArray[y][x] = DIRT;
            else if (colors[y * HEIGHT_PX + x].r <= 90) collArray[y][x] = GRASS1;
            else if (colors[y * HEIGHT_PX + x].r <= 130) collArray[y][x] = GRASS2;
            else if (colors[y * HEIGHT_PX + x].r <= 170) collArray[y][x] = GRASS3;
            else collArray[y][x] = ROCK;
        }
    }
    return collArray;
}

void Map::Unload(void)
{
    UnloadTexture(bgTexture);
    UnloadTexture(wallTexture);
    for (std::size_t i = 0; i < 6; i++) UnloadTexture(texPalette[i]);
    UnloadImageColors(colors);
    RL_FREE(tileMapCoordinates);
    RL_FREE(dimensions);
}
