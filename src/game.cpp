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