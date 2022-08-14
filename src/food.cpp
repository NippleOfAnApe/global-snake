/*******************************************************************************************
*
*    Здається — кращого немає
*    Нічого в Бога, як Дніпро
*    Та наша славная країна...
*
*                    Тарас Шевченко
*
********************************************************************************************/

#include <cstddef>
#include <stdlib.h>
#include <raylib.h>
#include "Classes.h"

//----------------------------------------------------------------------------------
// Variables/Objects
//----------------------------------------------------------------------------------

const unsigned short FOOD_ITEMS = 30;       // How many items in an area
unsigned short foodPoolDistance = 1000;     // How many PX in each direction to spawn fruits
Food fruits[FOOD_ITEMS] = { 0 };

//Food object properties
static float minusFoodLifetime = 8.0f;
static float bonusFoodLifetime = 10.0f;
static float regularFoodLifetime = 40.0f;
static short minusFruitPoints = 50;
static short bonusFruitPoints = 10;
static short regularFruitPoints = 2;
static float minusFruitScale = .8f;
static float bonusFruitScale = 1.2f;
static float regularFruitScale = .6f;
static short bonusFruitTailIncrease = 5;
static short regularFruitTailIncrease = 1;
static float boostAdd = 40;

static Texture2D raspberryTexture = { 0 };
static Texture2D pineapleTexture = { 0 };
static Texture2D sushiTexture = { 0 };
static Texture2D pizzaTexture = { 0 };

//----------------------------------------------------------------------------------
// FoodManager Functions Definition
//----------------------------------------------------------------------------------

void FoodManager::Init(void)
{
    // Pointer to Object
    mFood = fruits;

    // Every fruit is inactive
    for (std::size_t i = 0; i < FOOD_ITEMS; i++) (mFood + i)->active = false;

    raspberryTexture = LoadTexture("../resources/items/raspberry64.png");
    pineapleTexture = LoadTexture("../resources/items/pineaple64.png");
    sushiTexture = LoadTexture("../resources/items/sushi64.png");
    pizzaTexture = LoadTexture("../resources/items/pizza64.png");
}

void FoodManager::CalcFruitPos(Snake* snake, unsigned int widthMAX, unsigned int heightMAX)
{
    for (std::size_t i = 0; i < FOOD_ITEMS; i++)
    {
        if ((mFood + i)->lifetime <= 0) (mFood + i)->active = false;

        if (!(mFood + i)->active)
        {
            Vector2 pivot = snake->position;
            unsigned int minX = MAX(pivot.x - foodPoolDistance, 0);
            unsigned int maxX = MIN(pivot.x + foodPoolDistance, widthMAX);
            unsigned int minY = MAX(pivot.y - foodPoolDistance, 0);
            unsigned int maxY = MIN(pivot.y + foodPoolDistance, heightMAX);
            Vector2 pos = {(float)GetRandomValue(minX, maxX), (float)GetRandomValue(minY, maxY)};

            (mFood + i)->active = true;
            // Mod 40 = rarity
            int randomValue = GetRandomValue(1, 40);
            // Tail cut fruit
            if (randomValue % 20 == 0)
            {
                (mFood + i)->radius = minusFruitScale;
                (mFood + i)->foodTexture = &pizzaTexture;
                (mFood + i)->foodType = TAILCUT;
                (mFood + i)->position = pos;
                (mFood + i)->points = minusFruitPoints;
                (mFood + i)->lifetime = minusFoodLifetime;
            }
            // Speed boost fruit
            else if (randomValue % 10 == 0) 
            {
                (mFood + i)->radius = .5f;
                (mFood + i)->foodTexture = &sushiTexture;
                (mFood + i)->foodType = BOOST;
                (mFood + i)->position = pos;
                (mFood + i)->points = bonusFruitPoints;
                (mFood + i)->tailIncreaseSize = bonusFruitTailIncrease + 5;
                (mFood + i)->lifetime = bonusFoodLifetime;
            }
            // Bonus fruit
            else if (randomValue % 5 == 0) 
            {
                (mFood + i)->radius = bonusFruitScale;
                (mFood + i)->foodType = BONUS;
                (mFood + i)->foodTexture = &pineapleTexture;
                (mFood + i)->position = pos;
                (mFood + i)->points = bonusFruitPoints;
                (mFood + i)->tailIncreaseSize = bonusFruitTailIncrease;
                (mFood + i)->lifetime = bonusFoodLifetime;
            }
            // Main fruit
            else
            {
                (mFood + i)->radius = regularFruitScale;
                (mFood + i)->foodTexture = &raspberryTexture;
                (mFood + i)->foodType = REGULAR;
                (mFood + i)->position = pos;
                (mFood + i)->points = regularFruitPoints;
                (mFood + i)->tailIncreaseSize = regularFruitTailIncrease;
                (mFood + i)->lifetime = regularFoodLifetime + regularFoodLifetime * GetRandomValue(-10, 10) / 20;
            }
            
            if (FruitIsOnSnake(mFood + i, snake))
            (mFood + i)->position = pos;

        }

        (mFood + i)->lifetime -= GetFrameTime();
    }
}

void FoodManager::CalcFruitCollision(Player* player)
{
    for (std::size_t i = 0; i < FOOD_ITEMS; i++)
    {
        if (CheckCollisionCircles(player->mSnake->position, player->mSnake->radius, (mFood + i)->position, 32 * (mFood + i)->radius))
        {
            // Can't cut if tail if not long enough
            if ((mFood + i)->foodType == TAILCUT && player->GetCounterTail() > 2)
            {
                // Cuts a tail in half
                player->CutTail();
            }
            else
            {
                // Increase a tail by fruit's amount
                player->IncreaseTail((mFood + i)->tailIncreaseSize);
            }
            
            if ((mFood + i)->foodType == BOOST)
            {
                player->SetBoost(boostAdd);
            }

            // Update playrs's values
            player->SetCounterTail((mFood + i)->tailIncreaseSize);
            player->SetScore((mFood + i)->points);
            player->IncreaseSnakeRadius();

            (mFood + i)->active = false;
        }
    }
}

bool FoodManager::FruitIsOnSnake(Food* fruit, Snake* snake)
{
    if (!CheckCollisionCircles(fruit->position, fruit->radius, snake->position, snake->radius)) return false;
    return true;
}

//Draws a texture and a circle around it
void FoodManager::DrawFruits()
{
    for (std::size_t i = 0; i < FOOD_ITEMS; i++)
    {
        DrawTextureEx(*fruits[i].foodTexture, (Vector2){fruits[i].position.x - 32 * fruits[i].radius, fruits[i].position.y - 32 * fruits[i].radius}, 0, fruits[i].radius, WHITE);
        DrawCircleLines(fruits[i].position.x, fruits[i].position.y, 32 * fruits[i].radius, RED);
    }
}

//Unloads eerything in gpu
void FoodManager::Unload()
{
    UnloadTexture(raspberryTexture);
    UnloadTexture(pineapleTexture);
    UnloadTexture(sushiTexture);
    UnloadTexture(pizzaTexture);
}