#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};
int cellSize = 30;
int offset = 75;
int cellCount = 25;

double lastUpdateTime = 0;
bool elementInDeque(Vector2 element, deque<Vector2> &deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(element, deque[i]))
            return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Update()
    {
        if (addSegment)
        {
            body.push_front(Vector2Add(body[0], direction));
            addSegment = false;
        }
        else
        {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }
    }

    void Draw() const
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            int x = body[i].x;
            int y = body[i].y;
            Rectangle segment = {
                (float)offset + x * cellSize,
                (float)offset + y * cellSize,
                (float)cellSize,
                (float)cellSize};
            DrawRectangleRounded(segment, 20, 100, darkGreen);
        }
    }
};

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Food
{
public:
    Vector2 positon = {5, 6};
    Texture2D texture;

    Food(const deque<Vector2> &body)
    {
        Image image = LoadImage("./Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        positon = GenerateRandomPos(body);
    }

    ~Food() { UnloadTexture(texture); }

    void Draw() const
    {
        DrawTexture(texture,
                    offset + positon.x * cellSize,
                    offset + positon.y * cellSize,
                    WHITE);
    }

    Vector2 GenerateRandomPos(const deque<Vector2> &body)
    {
        Vector2 position;
        do
        {
            position.x = GetRandomValue(0, cellCount - 1);
            position.y = GetRandomValue(0, cellCount - 1);
        } while (elementInDeque(position, (deque<Vector2> &)body));
        return position;
    }
};

class Game
{
public:
    bool running = true;
    Snake snake;
    Food food;
    int score = 0;

// We keep these members only for desktop builds
#ifndef PLATFORM_WEB
    Sound eatSound;
    Sound wallSound;
#endif

    Game() : snake(), food(snake.body)
    {
#ifndef PLATFORM_WEB
        InitAudioDevice();
        eatSound = LoadSound("./Sounds/eat.wav");
        wallSound = LoadSound("./Sounds/wall.wav");
#endif
    }

    ~Game()
    {
#ifndef PLATFORM_WEB
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
#endif
    }

    void Draw() const
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if (!running)
            return;
        snake.Update();
        CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionWithTail();
    }

private:
    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.positon))
        {
#ifndef PLATFORM_WEB
            PlaySound(eatSound);
#else
            // Invoke JS to play <audio id="eatSound">
            emscripten_run_script("document.getElementById('eatSound').play()");
#endif

            food.positon = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
        }
    }

    void GameOver()
    {
        snake.body = {{6, 9}, {5, 9}, {4, 9}};
        food.positon = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
    }

    void CheckCollisionWithEdges()
    {
        if (snake.body[0].x < 0 || snake.body[0].x >= cellCount ||
            snake.body[0].y < 0 || snake.body[0].y >= cellCount)
        {
#ifndef PLATFORM_WEB
            PlaySound(wallSound);
#else
            emscripten_run_script("document.getElementById('wallSound').play()");
#endif
            GameOver();
        }
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headless = snake.body;
        headless.pop_front();
        if (elementInDeque(snake.body[0], headless))
            GameOver();
    }
};

int main()
{
    InitWindow(2 * offset + cellSize * cellCount,
               2 * offset + cellSize * cellCount,
               "Snake.IO in WASM");

    Game game;
    double fpsLastTime = GetTime();
    int fpsDisplay = 0;

    while (!WindowShouldClose())
    {
        double now = GetTime();
        if (now - fpsLastTime >= 1.0)
        {
            fpsDisplay = GetFPS();
            fpsLastTime = now;
        }

        BeginDrawing();

        if (eventTriggered(0.12))
            game.Update();

        // input
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_W) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_A) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        // draw
        ClearBackground(green);
        DrawRectangleLinesEx(
            {(float)offset - 5, (float)offset - 5,
             (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10},
            5, darkGreen);
        DrawText("Snake.io in WASM", offset - 5, 20, 40, darkGreen);
        DrawText(
            TextFormat("Score: %i", game.score),
            offset - 5, offset + cellSize * cellCount + 10, 20, darkGreen);
        game.Draw();
        DrawText(
            TextFormat("FPS: %d", fpsDisplay),
            cellCount * cellSize - offset, offset + cellSize * cellCount + 10,
            20, darkGreen);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
