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
#include "Classes.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//------------------------------------------------------------------------------------
// Variables Declaration
//------------------------------------------------------------------------------------
static const unsigned int screenWidth = 720;
static const unsigned int screenHeight = 900;

GameManager gameManager(screenWidth, screenHeight);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE); // Potentially might break the whole game
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "My Snake");
    gameManager.InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        if (!gameManager.GameOver())
        {
            gameManager.UpdateGame();
            gameManager.UpdateCamera(ALWAYS_FOLLOW);
            gameManager.DrawGame();
        }
        else
        {
            gameManager.MainMenu();
            gameManager.DrawMenu();
        }
        //----------------------------------------------------------------------------------
    }

#endif

    gameManager.UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}