/*******************************************************************************************
*
*    Терпи, терпи — терпець тебе шліфує,
*    Сталить твій дух — тож і терпи, терпи.
*    Ніхто тебе з недолі не врятує,
*    Ніхто не зіб'є з власної тропи.
*
*                        Василь Стус
*
********************************************************************************************/

#include <raylib.h>
#include "Classes.h"

//----------------------------------------------------------------------------------
// Variables/Objects
//----------------------------------------------------------------------------------

static Camera2D cam;
static Player player;
static Map map;
static FoodManager foodManager;

//----------------------------------------------------------------------------------
// GameManager Functions Definition
//----------------------------------------------------------------------------------

GameManager::GameManager(unsigned int width, unsigned int height) : screenWidth(width), screenHeight(height)
{
}

void GameManager::InitGame(void)
{
    //Reset game state
    framesCounter = 0;
    gameOver = true;
    pause = false;
    
    //Point references to Objects
    mPlayer = &player;
    mCamera = &cam;
    mMap = &map;
    mFoodManager = &foodManager;

    mPlayer->Init();
    mMap->Init();
    mFoodManager->Init();

    mCamera->offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f };
    mCamera->rotation = 0.0f;
    mCamera->zoom = 1.0f;
    mCamera->target = mPlayer->mSnake->position;
}

void GameManager::UpdateGame(void)
{
    if (IsKeyPressed('P')) pause = !pause;

    if (!pause)
    {
        // Get map coordinates to pass them further
        float* mapDimensions = mMap->GetDimensions();
        unsigned short tileSize = mMap->tileSize;
        
        // Player controls
        mPlayer->UpdateMovement();
        // Player position on tile grid
        mPlayer->UpdateTilePos(tileSize);
        // Snake movement
        mPlayer->MoveSnake();

        // Wall collision or Collision with self
        gameOver = mPlayer->CalcWallCollision(mapDimensions) || mPlayer->CalcSelfCollision();

        // Fruit position and Collision
        mFoodManager->CalcFruitPos(mPlayer->mSnake, mMap->mapWidth, mMap->mapHeight);
        mFoodManager->CalcFruitCollision(mPlayer);

        framesCounter++;
    }
}

void GameManager::DrawGame(void)
{
    BeginDrawing();

        ClearBackground(GRAY);

        BeginMode2D(*mCamera);
        
        //Draw each component
        mMap->Draw(mPlayer);
        mFoodManager->DrawFruits();
        mPlayer->DrawSnake();
        
        EndMode2D();

        DrawUI();

    EndDrawing();
}

void GameManager::DrawUI(void)
{
    BeginDrawing();

    if (!pause) DrawText("Press P to pause", screenWidth - MeasureText("Press P to pause", 18) - 20, 20, 18, BLACK);
    else
    {
        DrawText("Press P to continue", screenWidth - MeasureText("Press P to continue", 18) - 20, 20, 18, BLACK);
        DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, MAGENTA);
    }

    DrawText(TextFormat("SCORE: %02i", mPlayer->GetScore()), 30, 40, 24, MAROON);
    DrawText(TextFormat("BOOST: %.02f", mPlayer->GetBoost()), 600, 40, 24, MAROON);
    DrawText(TextFormat("TailCount: %d / %d", mPlayer->GetCounterTail(), SNAKE_LENGTH), 30, 400, 24, WHITE);
    DrawText(TextFormat("X: %d", mPlayer->tileXPos), 30, 60, 28, DARKPURPLE);
    DrawText(TextFormat("Y: %d", mPlayer->tileYPos), 30, 100, 28, DARKPURPLE);

    EndDrawing();
}

void GameManager::MainMenu(void)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        InitGame();
        gameOver = false;
    }
}

void GameManager::DrawMenu(void)
{
    BeginDrawing();
    
    ClearBackground(DARKBROWN);
    DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, RAYWHITE);

    EndDrawing();
}

void GameManager::UpdateCamera(unsigned char cameraMode)
{
    // TODO move to PlayerControlls
    if (IsKeyDown(KEY_Q)) mCamera->zoom += .01f;
    if (IsKeyDown(KEY_E)) mCamera->zoom -= .01f;
    // if (mCamera->zoom > 1.4f) mCamera->zoom = 1.4f;
    // if (mCamera->zoom < .7f) mCamera->zoom = .7f;

    switch (cameraMode)
    {
        case INSIDE_MAP:
            mMap->UpdateCameraCenterInsideMap(mCamera, mPlayer->mSnake, screenWidth, screenHeight);
            break;

        default:
            mMap->UpdateCameraCenter(mCamera, mPlayer->mSnake, screenWidth, screenHeight);
    }
}

void GameManager::UnloadGame(void)
{
    // TODO: If textures for Snake, unload here
    mMap->Unload();
    mFoodManager->Unload();
}

bool GameManager::GameOver(void) const
{
    return gameOver;
}
