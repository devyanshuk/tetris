// #include <stdio.h>
// #include <stdlib.h>
// #include <iostream>
// #include <stdbool.h>
// #include <time.h>
// #include "../include/constants.h"

// #include <SDL2/SDL.h>

// // Utility macros
// #define CHECK_ERROR(is_error, message) \
//     if((is_error)) { \
//         std::cerr << message << std::endl; \
//         exit(1); \
//     } \


// int randInt(int rmin, int rmax) {
//     return rand() % rmax + rmin;
// }


// int main(int argc, char **argv) {
//     srand((unsigned int)time(NULL));
    
//     CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());

//     SDL_Window * window;
//     SDL_Renderer * renderer;

//     SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_RENDERER_PRESENTVSYNC, &window, &renderer);

//     CHECK_ERROR(window == NULL, SDL_GetError());
//     CHECK_ERROR(renderer == NULL, SDL_GetError());

//     SDL_SetWindowTitle(window, "Tetris");

//     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

//     bool running = true;
//     SDL_Event event;
//     while(running) {
//         while(SDL_PollEvent(&event)) {
//             if(event.type == SDL_QUIT) {
//                 running = false;
//             } else if(event.type == SDL_KEYDOWN) {
//                 const char *key = SDL_GetKeyName(event.key.keysym.sym);
//                 if(strcmp(key, "C") == 0) {
//                     SDL_SetRenderDrawColor(renderer, randInt(0, 255), randInt(0, 255), randInt(0, 255), 255);
//                 }                    
//             }
//         }
//         SDL_RenderClear(renderer);
//         SDL_RenderPresent(renderer);
//     }

//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }