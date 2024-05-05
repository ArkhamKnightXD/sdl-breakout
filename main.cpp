#include <iostream>
#include <vector>
#include "inc/SDL.h"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Rect player;
SDL_Rect ball;

int playerSpeed = 800;
int ballVelocityX = 400;
int ballVelocityY = 400;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;

typedef struct
{
    SDL_Rect bounds;
    bool isDestroyed;
} Brick;

std::vector<Brick> bricks;

std::vector<Brick> createBricks()
{
    std::vector<Brick> bricks;

    int positionX;
    int positionY = 40;

    for (int i = 0; i < 8; i++)
    {
        positionX = 0;

        for (int j = 0; j < 15; j++)
        {
            Brick actualBrick = {{positionX, positionY, 60, 20}, false};

            bricks.push_back(actualBrick);
            positionX += 64;
        }

        positionY += 22;
    }

    return bricks;
}

void handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(0);
        }
    }
}

bool hasCollision(SDL_Rect bounds, SDL_Rect ball)
{
    return bounds.x < ball.x + ball.w && bounds.x + bounds.w > ball.x &&
           bounds.y < ball.y + ball.h && bounds.y + bounds.h > ball.y;
}

void update(float deltaTime)
{
    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);

    if (player.x > 0 && currentKeyStates[SDL_SCANCODE_A])
    {
        player.x -= playerSpeed * deltaTime;
    }

    else if (player.x < SCREEN_WIDTH - 74 && currentKeyStates[SDL_SCANCODE_D])
    {
        player.x += playerSpeed * deltaTime;
    }

    if (ball.y > SCREEN_HEIGHT + 32 || ball.y < -32)
    {
        ball.x = SCREEN_WIDTH / 2 - 32;
        ball.y = SCREEN_HEIGHT / 2 - 32;
    }

    if (ball.x < 0 || ball.x > SCREEN_WIDTH - 32)
    {
        ballVelocityX *= -1;
    }

    if (hasCollision(player, ball))
    {
        ballVelocityY *= -1;
    }

    for (unsigned int i = 0; i < bricks.size(); i++)
    {
        if (!bricks[i].isDestroyed && hasCollision(bricks[i].bounds, ball))
        {
            ballVelocityY *= -1;

            bricks[i].isDestroyed = true;
        }
    }

    ball.x += ballVelocityX * deltaTime;
    ball.y += ballVelocityY * deltaTime;
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (Brick brick : bricks)
    {
        if (!brick.isDestroyed)
            SDL_RenderFillRect(renderer, &brick.bounds);
    }

    SDL_RenderFillRect(renderer, &player);
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);
}

#undef main

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bricks = createBricks();

    player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 32, 74, 16};

    ball = {SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 20, 20, 20};

    Uint32 previousFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = previousFrameTime;
    float deltaTime = 0.0f;

    while (true)
    {
        currentFrameTime = SDL_GetTicks();

        deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f; // Convert to seconds

        previousFrameTime = currentFrameTime;

        handleEvents();
        update(deltaTime);
        render();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}