#ifndef MAP_H
#define MAP_H

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------

#define SNAKE_LENGTH    512

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

//----------------------------------------------------------------------------------
// Enums
//----------------------------------------------------------------------------------

enum FoodType { REGULAR, BONUS, BOOST, TAILCUT };
enum paletteName {WATER, SAND, ROCK, DIRT, GRASS1, GRASS2, GRASS3};
enum camMode {INSIDE_MAP, ALWAYS_FOLLOW};

//----------------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------------

struct Snake
{
    Vector2 position;
    Vector2 speed;
    float radius;
    Color color;
};

struct Food
{
    Vector2 position;
    Texture2D* foodTexture;
    unsigned short foodType;
    short points;
    float radius;
    bool active;
    short tailIncreaseSize;
    float lifetime;
};

//----------------------------------------------------------------------------------
// Classes
//----------------------------------------------------------------------------------

class Player
{
    public:
        void Init(void);
        void UpdateMovement(void);
        void MoveSnake(void);
        void CutTail(void);
        void IncreaseTail(unsigned short size);
        void IncreaseSnakeRadius(void);
        bool CalcWallCollision(float* dimensions);
        bool CalcSelfCollision(void);
        void DrawSnake(void);
        void UpdateTilePos(unsigned short tileSize);

        unsigned short GetScore(void) const;
        unsigned short GetCounterTail(void) const;
        float GetBoost(void) const;

        //Set = Update. Add "value" to to existing value
        void SetScore(unsigned short value);
        void SetCounterTail(unsigned short value);
        void SetBoost(float value);

        Snake* mSnake;
        unsigned short tileXPos;
        unsigned short tileYPos;

    private:
        unsigned short score;
        unsigned short counterTail;
        float boostCapacity;
        Vector2 currentSpeed;
        bool accelerating;
};

class FoodManager
{
    public:
        void Init(void);
        void Unload(void);
        void CalcFruitPos(Snake* snake, unsigned int maxX, unsigned int maxY);
        bool FruitIsOnSnake(Food* fruit, Snake* snake);
        void CalcFruitCollision(Player* player);
        void DrawFruits(void);
        
        Food* mFood;
};

class Map
{
    public:
        void Init(void);
        unsigned char** AssignColors(Color* colors);
        float* GetDimensions(void);     //1 - borderWidth, 2 - mapWidth, 3 - mapHight. I know it's inconvenient
        void UpdateCameraCenterInsideMap(Camera2D* camera, Snake* target, const float screenWidth, const float screenHeight);
        void UpdateCameraCenter(Camera2D* camera, Snake* target, const float screenWidth, const float screenHeight);
        void Draw(Player* player);
        void Unload(void);

        unsigned short mapPX;
        float tileSize;
        float mapWidth;
        float mapHeight;
};

class GameManager
{
    public:
        GameManager(unsigned int width, unsigned int height);

        void InitGame(void);
        void UpdateGame(void);
        void DrawGame(void);
        void DrawUI(void);
        void MainMenu(void);
        void DrawMenu(void);
        void UpdateCamera(unsigned char cameraMode);
        void UnloadGame(void);
        bool GameOver(void) const;

        Player* mPlayer;
        FoodManager* mFoodManager;
        Camera2D* mCamera;
        Map* mMap;
        unsigned int screenWidth;
        unsigned int screenHeight;

    private:
        unsigned int framesCounter;
        bool gameOver;
        bool pause;
};

#endif