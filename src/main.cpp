#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

Mix_Chunk *collisionSound = nullptr;
Mix_Chunk *collisionWithPlayerSound = nullptr;

SDL_Rect player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 32, 74, 16};
SDL_Rect ball = {SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 20, 20, 20};

int playerSpeed = 800;
int ballVelocityX = 425;
int ballVelocityY = 425;
int playerScore = 0;

bool isAutoPlayMode = false;

typedef struct {
    
    SDL_Rect bounds;
    bool isDestroyed;
} Brick;

std::vector<Brick> createBricks() {

    std::vector<Brick> bricks;

    int positionX;
    int positionY = 40;

    for (int row = 0; row < 8; row++) {

        positionX = 0;

        for (int column = 0; column < 15; column++) {

            Brick actualBrick = {{positionX, positionY, 60, 20}, false};

            bricks.push_back(actualBrick);
            positionX += 64;
        }

        positionY += 22;
    }

    return bricks;
}

std::vector<Brick> bricks = createBricks();

void quitGame() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {

            quitGame();
            exit(0);
        }
    }
}

void update(float deltaTime)
{
    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);

    if (currentKeyStates[SDL_SCANCODE_W]) {
        isAutoPlayMode = !isAutoPlayMode;
    }

    if (isAutoPlayMode && ball.x < SCREEN_WIDTH - player.w) {
        player.x = ball.x;
    }

    if (player.x > 0 && currentKeyStates[SDL_SCANCODE_A]) {
        player.x -= playerSpeed * deltaTime;
    }

    else if (player.x < SCREEN_WIDTH - player.w && currentKeyStates[SDL_SCANCODE_D]) {
        player.x += playerSpeed * deltaTime;
    }

    if (ball.y > SCREEN_HEIGHT + ball.h) {

        ball.x = SCREEN_WIDTH / 2 - ball.w;
        ball.y = SCREEN_HEIGHT / 2 - ball.h;

        ballVelocityX *= -1;
    }
    
    if (ball.x < 0 || ball.x > SCREEN_WIDTH - ball.w) {

        ballVelocityX *= -1;
        Mix_PlayChannel(-1, collisionSound, 0);
    }

    if (SDL_HasIntersection(&player, &ball) || ball.y < 0) {

        ballVelocityY *= -1;
        Mix_PlayChannel(-1, collisionWithPlayerSound, 0);
    }

    for (Brick &brick : bricks) {

        if (!brick.isDestroyed && SDL_HasIntersection(&brick.bounds, &ball)) {

            ballVelocityY *= -1;
            brick.isDestroyed = true;

            Mix_PlayChannel(-1, collisionSound, 0);
        }
    }

    ball.x += ballVelocityX * deltaTime;
    ball.y += ballVelocityY * deltaTime;
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

    for (Brick brick : bricks) {

        if (!brick.isDestroyed) 
            SDL_RenderFillRect(renderer, &brick.bounds);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer, &player);
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);
}


Mix_Chunk *loadSound(const char *p_filePath)
{
    Mix_Chunk *sound = nullptr;

    sound = Mix_LoadWAV(p_filePath);
    if (sound == nullptr)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

    return sound;
}

int main(int argc, char *args[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL crashed. Error: " << SDL_GetError();
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
    
    collisionSound = loadSound("res/sounds/magic.wav");
    collisionWithPlayerSound = loadSound("res/sounds/drop.wav");

    Uint32 previousFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = previousFrameTime;
    float deltaTime = 0.0f;

    while (true) {

        currentFrameTime = SDL_GetTicks();
        deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f;
        previousFrameTime = currentFrameTime;

        handleEvents();
        update(deltaTime);
        render();
    }

    return 0;
}