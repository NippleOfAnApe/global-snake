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

struct Snake
{
    void Init(void);

    Vector2 position;
    float size;
    Vector2 speed;
    Color color;
};

struct Player
{
    void Init(void);
    void UpdateMovement(void);
    void MoveSnake(void);
    void DrawSnake(void);


    Snake* mSnake;

    unsigned short score;
    unsigned int counterTail;
    Vector2 currentSpeed;
    bool accelerating;
    unsigned short tileXPos;
    unsigned short tileYPos;
    float boostCapacity;
};

struct Food
{
    Vector2 position;
    Texture2D* foodTexture;
    float scale;
    int foodType;
    bool active;
    int points;
    int tailIncreaseSize;
    float lifetime;
};

struct Map
{
    void Init(void);
    void Draw(Player* player);
};

struct GameManager
{
    void InitGame(void);
    void UpdateGame(void);
    void DrawGame(void);
    void UpdateMenu(void);
    void DrawMenu(void);
    void UnloadGame(void);

    Player* mPlayer;
    Camera2D* mCamera;
    Map* mMap;

    unsigned int screenWidth;
    unsigned int screenHeight;
    unsigned int framesCounter;
    bool gameOver;
    bool pause;
};

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------

extern Food fruits[FOOD_ITEMS];
extern const float tileSize;

extern const float mapWidth;
extern const float mapHeight;
extern float borderWidth;
extern float offMapSize;

//----------------------------------------------------------------------------------
// Map Functions Declaration
//----------------------------------------------------------------------------------
//void InitMap(void);
void CalcFruitPos(void);
//void DrawMap(void);
void UnloadMap(void);
void UpdateCameraCenterInsideMap(Camera2D *camera, int screenWidth, int screenHeight);
unsigned char** AssignColors(Color* colors);

//----------------------------------------------------------------------------------
// Snake Functions Declaration
//----------------------------------------------------------------------------------
//void InitSnake(void);
//void SetSnakeAsCameraTarget(Camera2D *camera);
//void UpdateMovement(Camera2D *camera);
bool CalcWallCollision(void);
bool CalcSelfCollision(void);
void CalcFruitCollision(void);
void DrawSnake(void);
//void MoveSnake(void);
bool FruitIsOnSnake(Food fruit);

#endif