#ifndef MAP_H
#define MAP_H
#include <raylib.h>
//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH   512
#define FOOD_ITEMS      100

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
enum FoodType { REGULAR, BONUS, BOOST, TAILCUT };
enum paletteName {WATER, SAND, ROCK, DIRT, GRASS1, GRASS2, GRASS3};


struct Snake {
    Vector2 position;
    unsigned short tileXPos;
    unsigned short tileYPos;;
    float boostCapacity;
    float size;
    Vector2 speed;
    Color color;
};

struct Food {
    Vector2 position;
    Texture2D* foodTexture;
    float scale;
    int foodType;
    bool active;
    int points;
    int tailIncreaseSize;
    float lifetime;
};

struct GameManager
{
    void InitGame(void);
    void UpdateGame(void);
    void DrawGame(void);
    void UpdateMenu(void);
    void DrawMenu(void);
    void UnloadGame(void);

    Camera2D* camera;

    unsigned int screenWidth;
    unsigned int screenHeight;
    unsigned int framesCounter;
    bool gameOver;
    bool pause;
};

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
extern int score;
extern int counterTail;

extern Food fruits[FOOD_ITEMS];
extern Snake snake[SNAKE_LENGTH];
extern const float tileSize;

extern const float mapWidth;
extern const float mapHeight;
extern float borderWidth;
extern float offMapSize;

//----------------------------------------------------------------------------------
// Map Functions Declaration
//----------------------------------------------------------------------------------
void InitMap(void);
void CalcFruitPos(void);
void DrawMap(void);
void UnloadMap(void);
void UpdateCameraCenterInsideMap(Camera2D *camera, int screenWidth, int screenHeight);
unsigned char** AssignColors(Color* colors);

//----------------------------------------------------------------------------------
// Snake Functions Declaration
//----------------------------------------------------------------------------------
void InitSnake(void);
void SetSnakeAsCameraTarget(Camera2D *camera);
void UpdateMovement(Camera2D *camera);
bool CalcWallCollision(void);
bool CalcSelfCollision(void);
void CalcFruitCollision(void);
void DrawSnake(void);
void MoveSnake(void);
bool FruitIsOnSnake(Food fruit);

#endif