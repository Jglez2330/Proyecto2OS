#include <stdio.h>
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include "SDL2/SDL_mixer.h"

#define WINDOW_WIDTH (1850)
#define WINDOW_HEIGHT (1050)
void drawBackground(SDL_Renderer *rend,SDL_Texture *background_t, SDL_Texture *anthill_t,SDL_Rect antHill1_r,SDL_Rect antHill2_r,SDL_Rect horizontal_roads);
void initAudio();
void createSurfaces();
int main() {
    // Se inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0) {
        printf("error initializing SDL:%s\n", SDL_GetError());
        return 1;
    }

    initAudio();

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

    //Cargamos las imagenes
    SDL_Surface *background_s = IMG_Load("../Resources/background.png");
    SDL_Surface *anthill_s = IMG_Load("../Resources/antHill.png");
    SDL_Surface *blackAnt_s = IMG_Load("../Resources/blackAnt.png");



    if(!anthill_s || !background_s){
        printf("Error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Creamos las texturas
    SDL_Texture *background_t = SDL_CreateTextureFromSurface(rend, background_s);
    SDL_Texture *anthill_t = SDL_CreateTextureFromSurface(rend, anthill_s);
    SDL_Texture *blackAnt_t = SDL_CreateTextureFromSurface(rend, blackAnt_s);



    //Creamos los contenedores de los objetos y les cargamos las imagenes

    SDL_Rect antHill1_r;
    SDL_Rect antHill2_r;
    SDL_Rect blackAnt_r;
    SDL_QueryTexture(anthill_t, NULL, NULL, &antHill1_r.w, &antHill1_r.h);
    SDL_QueryTexture(anthill_t, NULL, NULL, &antHill2_r.w, &antHill2_r.h);
    SDL_QueryTexture(blackAnt_t, NULL, NULL, &blackAnt_r.w, &blackAnt_r.h);

    int antX_start = 450;
    int antY_start = 350;



    blackAnt_r.x = antX_start;
    blackAnt_r.y = antY_start;

    antHill1_r.w *= 0.8;
    antHill1_r.h *= 0.8;

    antHill2_r.w *= 0.8;
    antHill2_r.h *= 0.8;

    antHill1_r.x = 100;
    antHill1_r.y = 350;

    antHill2_r.x = 1500;
    antHill2_r.y = 350;

    blackAnt_r.w *= 0.03;
    blackAnt_r.h *= 0.06;
    SDL_RenderCopy(rend, background_t, NULL, NULL);
    SDL_RenderCopy(rend, anthill_t, NULL, &antHill1_r);
    SDL_RenderCopy(rend, anthill_t, NULL, &antHill2_r);
    SDL_RenderCopy(rend, blackAnt_t, NULL, &blackAnt_r);



    SDL_Rect horizontal_roads;
    horizontal_roads.x = 400;
    horizontal_roads.y = 100;
    horizontal_roads.w = 50;
    horizontal_roads.h = 800;

    SDL_SetRenderDrawColor( rend, 255, 255, 255, 255 );


    for (int i=0;i !=4;i++){
        if(i==0 || i == 1){
            SDL_RenderFillRect( rend, &horizontal_roads );
            horizontal_roads.x +=55;
        }
        if(i==2){horizontal_roads.x += 850;}
        if(i==2 || i == 3){
            SDL_RenderFillRect( rend, &horizontal_roads );
            horizontal_roads.x +=55;
        }


    }


    if(!background_t){
        printf("Error creating text: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_RenderPresent(rend);
    printf("Initialization succesful!\n");
    bool quit = false;

    //SDL_Delay(10000);
    while (!quit)
    {
        if (blackAnt_r.x > 2000){
            //quit = true;
        }
        drawBackground(rend,background_t,anthill_t,antHill1_r,antHill2_r,horizontal_roads);

        blackAnt_r.x +=100;
        SDL_RenderCopy(rend, blackAnt_t, NULL, &blackAnt_r);

        SDL_RenderPresent(rend);
        SDL_Delay(500);

    }

    SDL_Quit();


    return 0;
}
void initAudio(){
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}
void drawBackground(SDL_Renderer *rend,SDL_Texture *background_t, SDL_Texture *anthill_t,SDL_Rect antHill1_r,SDL_Rect antHill2_r, SDL_Rect horizontal_roads){
    SDL_RenderClear(rend);


    SDL_RenderCopy(rend, background_t, NULL, NULL);
    SDL_RenderCopy(rend, anthill_t, NULL, &antHill1_r);
    SDL_RenderCopy(rend, anthill_t, NULL, &antHill2_r);
    horizontal_roads.x = 400;
    horizontal_roads.y = 100;
    horizontal_roads.w = 50;
    horizontal_roads.h = 800;
    for (int i=0;i !=4;i++){
        if(i==0 || i == 1){
            SDL_RenderFillRect( rend, &horizontal_roads );
            horizontal_roads.x +=55;
        }
        if(i==2){horizontal_roads.x += 850;}
        if(i==2 || i == 3){
            SDL_RenderFillRect( rend, &horizontal_roads );
            horizontal_roads.x +=55;
        }


    }



}
