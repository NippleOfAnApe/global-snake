/*******************************************************************************************
*
*    А й правда, крилатим ґрунту не треба.
*    Землі немає, то буде небо.
*    Немає поля, то буде воля.
*    Немає пари, то будуть хмари.
*                
*                       Ліна Костенко
*
********************************************************************************************/

#include <raylib.h>
#include <raymath.h>
#include "Classes.h"
#include <stdbool.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const unsigned int screenWidth = 1280;
static const unsigned int screenHeight = 720;

GameManager gameManager;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Make the window resizable
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "My Snake");
    gameManager.InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        if (!gameManager.gameOver)
        {
            gameManager.UpdateGame();
            gameManager.DrawGame();
        }
        else
        {
            gameManager.UpdateMenu();
            gameManager.DrawMenu();
        }
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    gameManager.UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
// void InitGame(void)
// {   
//     framesCounter = 0;
//     gameOver = false;
//     pause = false;

//     SetSnakeAsCameraTarget(&camera);
//     camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f };
//     camera.rotation = 0.0f;
//     camera.zoom = 1.0f;
//     InitSnake();
//     InitMap();
// }

// Update and Draw (one frame)
// void UpdateDrawFrame(void)
// {
//     UpdateGame();
//     DrawGame();
// }

// // Update game (one frame)`LOGIC
// void UpdateGame(void)
// {
//     if (!gameManager.gameOver)
//     {
//         if (IsKeyPressed('P')) gameManager.pause = !gameManager.pause;

//         if (!gameManager.pause)
//         {
//             // Player controls
//             UpdateMovement(gameManager.camera);

//             // Snake movement
//             MoveSnake();

//             // Wall collision or Collision with self
//             gameManager.gameOver = CalcWallCollision() || CalcSelfCollision();

//             // Fruit position calculation
//             CalcFruitPos();

//             // Collision
//             CalcFruitCollision();
            
//             //Camera updater
//             UpdateCameraCenterInsideMap(gameManager.camera, screenWidth, screenHeight);

//             gameManager.framesCounter++;
//         }
//     }
//     else
//     {
//         if (IsKeyPressed(KEY_ENTER))
//         {
//             gameManager.InitGame();
//             gameManager.gameOver = false;
//         }
//     }
// }

// //Draw game (one frame)
// void DrawGame(void)
// {
//     BeginDrawing();

//         ClearBackground(GRAY);
//         if (!gameManager.gameOver)
//         {
//             BeginMode2D(*gameManager.camera);
//             DrawMap();

//             // Draw snake
//             DrawSnake();
        
//             EndMode2D();
//             DrawUI();   //UI on top of game elements
//         }
//         else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, RAYWHITE);

//     EndDrawing();
// }

// //Draws UI textboxes
// void DrawUI(void)
// {
//     if (!gameManager.pause)
//     DrawText("Press P to pause", screenWidth - MeasureText("Press P to pause", 18) - 20, 20, 18, BLACK);
//     else
//     {
//         DrawText("Press P to continue", screenWidth - MeasureText("Press P to continue", 18) - 20, 20, 18, BLACK);
//         DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
//     }
//     DrawText(TextFormat("SCORE: %02i", score), 30, 40, 24, MAROON);
//     DrawText(TextFormat("BOOST: %.02f", snake->boostCapacity), 600, 40, 24, MAROON);
//     DrawText(TextFormat("TailCount: %d / %d", counterTail, SNAKE_LENGTH), 30, 400, 24, WHITE);
//     DrawText(TextFormat("speed.x: %d", snake->tileXPos), 30, 60, 28, DARKPURPLE);
//     DrawText(TextFormat("speed.y: %d", snake->tileYPos), 30, 100, 28, DARKPURPLE);
//}

// Unload game variables
// void UnloadGame(void)
// {
//     // TODO: Unload all dynamic loaded data (textures, sounds, models...)
//     UnloadMap();
// }

