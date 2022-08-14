/*******************************************************************************************
*
*    Лиш боротись — значить жить!
*                   
*                       Іван Франко
*
********************************************************************************************/

#include <cstddef>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "Classes.h"

//----------------------------------------------------------------------------------
// Variables/Objects
//----------------------------------------------------------------------------------

static Snake snake[SNAKE_LENGTH] = { 0 };
static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
static Color SnakeColorPatern1[] = { ORANGE, SKYBLUE, MAGENTA, LIME, YELLOW };

//Aceleration
static float snakeSizeRadius = 20;              // Radius of each circle
static unsigned char snakeColorFrequency = 8;   // Circles in a row with same color
static float snakeSpeed = 4.0f;                 // Basic speed
static float boostSpeed = 2.0f;                 // Multiplier when accelerated
static float boostDrain = .4f;                  // How quickly boost is drained
static unsigned char tailStartSize = 8;         // Length of tail in the begining  
static float radiusGrowRate = .05f;             // Radius increase with length. (Multiplier of counterTail)

//Controlls
//Keys
static float turnAngle = 0.0f;     //Logic in UpdateMovement()
static float cosAnglePositive = 0.0f;
static float sinAnglePositive = 0.0f;
static float cosAngleNegative = 0.0f;
static float sinAngleNegative = 0.0f;
//Mouse
// Vector2 screnPos = { 0 };
// Vector2 mousePos = { 0 };
// float angleMouse = 0.0f;
// float dx = 0.0f, dy = 0.0f, dxx = 0.0f, dyy = 0.0f;

//----------------------------------------------------------------------------------
// Player Functions Definition
//----------------------------------------------------------------------------------

void Player::Init(void)
{
    // Pointer to Object
    mSnake = snake;

    // Default values to start with
    counterTail = tailStartSize;
    score = tailStartSize - tailStartSize;
    accelerating = false;
    boostCapacity = 0.0f;
    tileXPos = 0;
    tileYPos = 0;

    for (std::size_t i = 0; i < SNAKE_LENGTH; i++)
    {
        (mSnake + i)->position = (Vector2){ 100.0f, 100.0f };
        (mSnake + i)->radius = snakeSizeRadius;
        (mSnake + i)->speed = (Vector2){ snakeSpeed, snakeSpeed };
        (mSnake + i)->color = SnakeColorPatern1[i / snakeColorFrequency % (sizeof(SnakeColorPatern1) / sizeof(Color))];
        
        snakePosition[i] = (Vector2){ 0.0f, 0.0f };
    }
}

void Player::UpdateTilePos(unsigned short tileSize)
{
    tileXPos = mSnake->position.x / tileSize;
    tileYPos = mSnake->position.y / tileSize;
}

void Player::UpdateMovement(void)
{
    // Mouse Movement

    // mousePos = GetMousePosition();

    //     /*Mouse controlls*/
    // screnPos = GetScreenToWorld2D(mousePos, *camera);
    // dx = screnPos.x - snake->position.x;
    // dy = screnPos.y - snake->position.y;
    // angleMouse = atan2f(dy, dx);       //cos = x(-1;1)     sin = y(-1;1)

    // dxx = cosf(angleMouse);
    // dyy = sinf(angleMouse);

    // //Sets the direction. If top/down/left/right, then move straight
    // if (dxx >= .99f) snake[0].speed = (Vector2){snakeSpeed * 2, 0};
    // else if (dxx <= -.99f) snake[0].speed = (Vector2){-snakeSpeed * 2, 0};
    // else if (dyy >= .99f) snake[0].speed = (Vector2){0, snakeSpeed * 2};
    // else if (dyy <= -.99f) snake[0].speed = (Vector2){0, -snakeSpeed * 2};
    // else if (dxx > 0 && dyy > 0) snake[0].speed = (Vector2){snakeSpeed + snakeSpeed * dxx, snakeSpeed + snakeSpeed * dyy};
    // else if (dxx > 0 && dyy < 0) snake[0].speed = (Vector2){snakeSpeed + snakeSpeed * dxx, -snakeSpeed + snakeSpeed * dyy};
    // else if (dxx < 0 && dyy > 0) snake[0].speed = (Vector2){-snakeSpeed + snakeSpeed * dxx, snakeSpeed + snakeSpeed * dyy};
    // else snake[0].speed = (Vector2){-snakeSpeed + snakeSpeed * dxx, -snakeSpeed + snakeSpeed * dyy};

    //Keyboard Movement

    // The bigger the snake, the slower it turns
    if (counterTail < 50) turnAngle = 8.0f;
    else if (counterTail < 100) turnAngle = 7.0f;
    else if (counterTail < 150) turnAngle = 6.0f;
    else if (counterTail < 200) turnAngle = 5.0f;
    else if (counterTail < 250) turnAngle = 4.0f;
    else if (counterTail < 300) turnAngle = 3.0f;
    else turnAngle = 2.0f;

    cosAnglePositive = cosf(turnAngle * DEG2RAD);
    sinAnglePositive = sinf(turnAngle * DEG2RAD);
    cosAngleNegative = cosf(-turnAngle * DEG2RAD);
    sinAngleNegative = sinf(-turnAngle * DEG2RAD);

    // Stores a last non-accelerating vector to return back to after acceleration
    if (!accelerating) currentSpeed = mSnake->speed;

    // Can't turn while accelerating
    // Easy Trigonometry
    if (IsKeyDown(KEY_RIGHT) && IsKeyUp(KEY_LEFT) && !accelerating)
    {
        mSnake->speed = (Vector2){mSnake->speed.x * cosAnglePositive - mSnake->speed.y * sinAnglePositive, mSnake->speed.x * sinAnglePositive + mSnake->speed.y * cosAnglePositive};
    }
    else if (IsKeyDown(KEY_LEFT) && IsKeyUp(KEY_RIGHT) && !accelerating)
    {
        mSnake->speed = (Vector2){mSnake->speed.x * cosAngleNegative - mSnake->speed.y * sinAngleNegative, mSnake->speed.x * sinAngleNegative + mSnake->speed.y * cosAngleNegative};
    }

    //Acceleration
    if (IsKeyDown(KEY_SPACE))
    {
        // if (boostCapacity > 0)
        // {
        //     accelerating = true;
        //     boostCapacity -= boostDrain;
        //     mSnake->speed = (Vector2){currentSpeed.x * boostSpeed, currentSpeed.y * boostSpeed};
        // }else
        // {
        //     boostCapacity = 0.0f;
        //     accelerating = false;
        //     mSnake->speed = currentSpeed;
        // }
        //DEBUG
        accelerating = true;
        snake->speed = (Vector2){currentSpeed.x * 8.0f, currentSpeed.y * 8.0f};
    }

    if (IsKeyReleased(KEY_SPACE))
    {
        accelerating = false;
        mSnake->speed = currentSpeed;
    }
}

void Player::MoveSnake(void)
{
    for (std::size_t i = 0; i < counterTail; i++)
    {
        // Store previous position (of each circle) in a separate Vector
        snakePosition[i] = (mSnake + i)->position;

        if (i == 0)
        {
            // Head moce forward according to Speed vector
            mSnake->position.x += mSnake->speed.x;
            mSnake->position.y += mSnake->speed.y;
        }
        // Move each body part to its previous position
        else (mSnake + i)->position = snakePosition[i-1];
    }
}

void Player::CutTail(void)
{
    // Cust a tail in half
    for (std::size_t i = 0; i < counterTail / 2; i++)
    (mSnake + counterTail - i)->position = snakePosition[counterTail - 1];
}

void Player::IncreaseTail(unsigned short size)
{
    for (unsigned short i = 0; i < size; i++)
    (mSnake + counterTail + i)->position = snakePosition[counterTail - 1];
}

void Player::DrawSnake(void)
{
    for (std::size_t i = counterTail - 1; i > 0; i--)
    DrawCircleV((mSnake + i)->position, (mSnake + i)->radius, (mSnake + i)->color);
}

void Player::IncreaseSnakeRadius(void)
{
    //Increase circles size
    for (std::size_t i = 0; i < SNAKE_LENGTH; i++)
    (mSnake + i)->radius = snakeSizeRadius + counterTail * radiusGrowRate;
}

bool Player::CalcWallCollision(float* dimensions)
{
    // Dimensions: 0 - BorderWidth, 1 - MapWidth, 2 - MapHeight
    return (((mSnake->position.x - mSnake->radius) > (*(dimensions + 1) - *dimensions)) ||
            ((mSnake->position.y - mSnake->radius) > (*(dimensions + 2) - *dimensions)) ||
            (mSnake->position.x - mSnake->radius < 0) || (mSnake->position.y - mSnake->radius < 0));
}

bool Player::CalcSelfCollision(void)        //TODO work on better collision recognition
{
    // for (int i = 1; i < counterTail; i++)
    // {
    //     return ((snake->position.x == snake[i].position.x) && (snake->position.y == snake[i].position.y));
    // }
    return false;
}

unsigned short Player::GetScore(void) const
{
    return score;
}

unsigned short Player::GetCounterTail(void) const
{
    return counterTail;
}

float Player::GetBoost(void) const
{
    return boostCapacity;
}

void Player::SetScore(unsigned short value)
{
    score += value;
}

void Player::SetCounterTail(unsigned short value)
{
    counterTail += value;
}

void Player::SetBoost(float value)
{
    boostCapacity += value;
}