#include <raylib.h>
#include "Classes.h"

Camera2D cam;
Player player;
Map map;

GameManager::GameManager(unsigned int width, unsigned int height) : screenWidth(width), screenHeight(height)
{
}

void GameManager::InitGame()
{
    framesCounter = 0;
    gameOver = true;
    pause = false;
    
    mPlayer = &player;
    mCamera = &cam;
    mMap = &map;

    mPlayer->Init();
    mMap->Init();

    mCamera->offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f };
    mCamera->rotation = 0.0f;
    mCamera->zoom = 1.0f;
    mCamera->target = mPlayer->mSnake->position;
}

void GameManager::UpdateGame()
{
    if (IsKeyPressed('P')) pause = !pause;

    if (!pause)
    {
        // // Player controls
        mPlayer->UpdateMovement();

        // // Snake movement
        mPlayer->MoveSnake();

        // // Wall collision or Collision with self
        // gameOver = CalcWallCollision() || CalcSelfCollision();

        // // Fruit position calculation
        // CalcFruitPos();

        // // Collision
        // CalcFruitCollision();

        framesCounter++;
    }
}

void GameManager::DrawGame()
{
    BeginDrawing();

        ClearBackground(GRAY);
        BeginMode2D(*mCamera);
        mMap->Draw(mPlayer);

        // Draw snake
        mPlayer->DrawSnake();
    
        EndMode2D();
        if (!pause)
        DrawText("Press P to pause", screenWidth - MeasureText("Press P to pause", 18) - 20, 20, 18, BLACK);
        else
        {
            DrawText("Press P to continue", screenWidth - MeasureText("Press P to continue", 18) - 20, 20, 18, BLACK);
            DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, MAGENTA);
        }
        // DrawText(TextFormat("SCORE: %02i", score), 30, 40, 24, MAROON);
        // DrawText(TextFormat("BOOST: %.02f", snake->boostCapacity), 600, 40, 24, MAROON);
        // DrawText(TextFormat("TailCount: %d / %d", counterTail, SNAKE_LENGTH), 30, 400, 24, WHITE);
        // DrawText(TextFormat("speed.x: %d", snake->tileXPos), 30, 60, 28, DARKPURPLE);
        // DrawText(TextFormat("speed.y: %d", snake->tileYPos), 30, 100, 28, DARKPURPLE);

    EndDrawing();
}

void GameManager::UpdateMenu(void)
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

void GameManager::UpdateCamera(unsigned int cameraMode)
{
    switch (cameraMode)
    {
        case INSIDE_MAP:
            mMap->UpdateCameraCenterInsideMap(mCamera, mPlayer->mSnake, screenWidth, screenHeight);
            break;

        default:
            mMap->UpdateCameraCenter(mCamera, mPlayer->mSnake, screenWidth, screenHeight);
    }
}

void GameManager::UnloadGame()
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
    mMap->Unload();
}
