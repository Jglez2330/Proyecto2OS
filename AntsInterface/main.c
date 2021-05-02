#include <stdio.h>
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>

#define WINDOW_WIDTH (1850)
#define WINDOW_HEIGHT (850)

int main() {
    // Se inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0) {
        printf("error initializing SDL:%s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Ant Hill", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH,WINDOW_HEIGHT, 0);

    if (!win) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //Se crea el render que va a contener la ventana
    Uint8 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    SDL_Surface *background_s = IMG_Load("../Resources/dirt.jpg");
    SDL_Surface *anthill_s = IMG_Load("../Resources/antHill.png");
    if(!anthill_s || !background_s){
        printf("Error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *background_t = SDL_CreateTextureFromSurface(rend, background_s);
    SDL_Texture *anthill_t = SDL_CreateTextureFromSurface(rend, anthill_s);



    SDL_RenderCopy(rend, background_t, NULL, NULL);


    SDL_Rect antHill1_r;
    SDL_Rect antHill2_r;
    SDL_QueryTexture(anthill_t, NULL, NULL, &antHill1_r.w, &antHill1_r.h);
    SDL_QueryTexture(anthill_t, NULL, NULL, &antHill2_r.w, &antHill2_r.h);

    antHill1_r.w *= 1;
    antHill1_r.h *= 1;

    antHill1_r.w *= 1;
    antHill1_r.h *= 1;

    antHill1_r.x = 100;
    antHill1_r.y = 250;

    antHill2_r.x = 1500;
    antHill2_r.y = 250;
    
    SDL_RenderCopy(rend, anthill_t, NULL, &antHill1_r);
    SDL_RenderCopy(rend, anthill_t, NULL, &antHill2_r);

    SDL_RenderPresent(rend);

    if(!background_t){
        printf("Error creating text: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    printf("Initialization succesful!\n");
    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }
    }

    SDL_Quit();


    return 0;
}
