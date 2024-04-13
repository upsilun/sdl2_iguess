#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int PLAYER_SIZE = 20;
const int OBJECT_SIZE = 10;
const int OBSTACLE_SIZE = 30;
const int NUM_OBJECTS = 5;
const int NUM_OBSTACLES = 5;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect playerRect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, PLAYER_SIZE, PLAYER_SIZE };
SDL_Rect objects[NUM_OBJECTS];
SDL_Rect obstacles[NUM_OBSTACLES];

int score = 0;
int playerSpeed = 5;

void initializeSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Simple SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
}

void closeSDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            closeSDL();
            exit(0);
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (playerRect.y > 0)
                        playerRect.y -= playerSpeed;
                    break;
                case SDLK_DOWN:
                    if (playerRect.y < SCREEN_HEIGHT - PLAYER_SIZE)
                        playerRect.y += playerSpeed;
                    break;
                case SDLK_LEFT:
                    if (playerRect.x > 0)
                        playerRect.x -= playerSpeed;
                    break;
                case SDLK_RIGHT:
                    if (playerRect.x < SCREEN_WIDTH - PLAYER_SIZE)
                        playerRect.x += playerSpeed;
                    break;
            }
        }
    }
}

void generateObjectsAndObstacles() {
    for (int i = 0; i < NUM_OBJECTS; ++i) {
        objects[i].x = rand() % (SCREEN_WIDTH - OBJECT_SIZE);
        objects[i].y = rand() % (SCREEN_HEIGHT - OBJECT_SIZE);
        objects[i].w = OBJECT_SIZE;
        objects[i].h = OBJECT_SIZE;
    }

    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        obstacles[i].x = rand() % (SCREEN_WIDTH - OBSTACLE_SIZE);
        obstacles[i].y = rand() % (SCREEN_HEIGHT - OBSTACLE_SIZE);
        obstacles[i].w = OBSTACLE_SIZE;
        obstacles[i].h = OBSTACLE_SIZE;
    }
}

bool checkCollision(SDL_Rect rect1, SDL_Rect rect2) {
    return (rect1.x < rect2.x + rect2.w && rect1.x + rect1.w > rect2.x && rect1.y < rect2.y + rect2.h && rect1.y + rect1.h > rect2.y);
}

void render() {
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);

    // Render player
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &playerRect);

    // Render objects
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
    for (int i = 0; i < NUM_OBJECTS; ++i) {
        SDL_RenderFillRect(gRenderer, &objects[i]);
    }

    // Render obstacles
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        SDL_RenderFillRect(gRenderer, &obstacles[i]);
    }

    SDL_RenderPresent(gRenderer);
}

int main(int argc, char* args[]) {
    initializeSDL();
    generateObjectsAndObstacles();

    bool quit = false;
    while (!quit) {
        handleInput();

        // Collision detection
        for (int i = 0; i < NUM_OBJECTS; ++i) {
            if (checkCollision(playerRect, objects[i])) {
                std::cout << "Object collected!\n";
                objects[i].x = rand() % (SCREEN_WIDTH - OBJECT_SIZE);
                objects[i].y = rand() % (SCREEN_HEIGHT - OBJECT_SIZE);
                score++;
                playerSpeed += 1; // Increase speed
                std::cout << '\a';
            }
        }

        for (int i = 0; i < NUM_OBSTACLES; ++i) {
            if (checkCollision(playerRect, obstacles[i])) {
                std::cout << "Game Over!\n";
                quit = true;
            }
        }

        render();
    }

    closeSDL();
    return 0;
}
