#include <iostream>
#include "inc/SDL.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Rect rectangle;

const int SPEED = 600;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;
const int FRAME_RATE = 60; // Desired frame rate (frames per second)

// Function to handle events
void handleEvents() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
            
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(0);
        }
    }
}

// Function to update rectangle movement.
void update(float deltaTime) {

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if (currentKeyStates[SDL_SCANCODE_W] && rectangle.y > 0) {
        rectangle.y -= SPEED * deltaTime;
    }

    else if (currentKeyStates[SDL_SCANCODE_S] && rectangle.y < SCREEN_HEIGHT - rectangle.h) {
        rectangle.y += SPEED * deltaTime;
    }

    else if (currentKeyStates[SDL_SCANCODE_A] && rectangle.x > 0) {
        rectangle.x -= SPEED * deltaTime;
    }

    else if (currentKeyStates[SDL_SCANCODE_D] && rectangle.x < SCREEN_WIDTH - rectangle.w) {
        rectangle.x += SPEED * deltaTime;
    }
}

// Function to render graphics
void render() {
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Render the rectangle
    SDL_RenderFillRect(renderer, &rectangle);

    // Present the renderer
    SDL_RenderPresent(renderer);
}

// Function to cap frame rate
void capFrameRate(Uint32 frameStartTime) {

    Uint32 frameTime = SDL_GetTicks() - frameStartTime;
    
    if (frameTime < 1000 / FRAME_RATE) {
        SDL_Delay(1000 / FRAME_RATE - frameTime);
    }
}

#undef main

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    rectangle = {SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 - 16, 32, 32};

    Uint32 previousFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = previousFrameTime;
    float deltaTime = 0.0f;

    // Main loop
    while (true) {
        
        currentFrameTime = SDL_GetTicks();

    // Calculate delta time in seconds
        deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f; // Convert to seconds

    // Update the previous frame time for the next iteration
        previousFrameTime = currentFrameTime;

        handleEvents();
        update(deltaTime);
        render();

        // capFrameRate(currentFrameTime);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}