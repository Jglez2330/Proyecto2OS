#include <stdio.h>
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_timer.h"
#include <unistd.h>
#include "antsInterface.c"
#include "functions.c"





int main() {
    // Se inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
        printf("error initializing SDL:%s\n", SDL_GetError());
        return 1;
    }

    initAudio();

    SDL_Window *win = SDL_CreateWindow("Ant Hill", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (!win) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //Se crea el render que va a contener la ventana
    Uint8 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend) {
        printf("Error creating rendrer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Cargamos las imagenes
    SDL_Surface *background_s = IMG_Load("../Resources/background.png");
    SDL_Surface *anthill_s = IMG_Load("../Resources/antHill.png");
    SDL_Surface *blackAnt_s = IMG_Load("../Resources/blackAnt.png");


    if (!anthill_s || !background_s) {
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

    antHill1_r.x = antHill_x;
    antHill1_r.y = antHill_y;

    antHill2_r.x = antHill1_r.x + distanceBetweenHills;
    antHill2_r.y = antHill1_r.y;

    blackAnt_r.w *= 0.03;
    blackAnt_r.h *= 0.06;


    int cellSize = w_horizontal_road / sizeOfCanal;
    drawLines(rend, sizeOfCanal, x_start_road, y_start_road);
    int x_startPoint = x_start_road + distanceBetweenRoads / 2 + gap_roads / 2;
    drawLines(rend, sizeOfCanal, x_startPoint, y_start_road);
    SDL_Rect horizontal_roads;
    SDL_Rect verticals_roads;
    SDL_Rect mini_roads;
    mini_roads.h = 50;
    mini_roads.w = 100;

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);


    drawRoads(rend, horizontal_roads, verticals_roads);

    SDL_RenderCopy(rend, background_t, NULL, NULL);
    drawHills(rend, anthill_t, antHill1_r, antHill2_r, mini_roads);


    const int x_startAntHill = antHill_x;
    const int y_startAntHill = antHill_y;




    int n = sizeOfCanal;
    Ant *f1 = malloc(n * sizeof(Ant));
    Ant *f2 = malloc(n * sizeof(Ant));
    Ant *f3 = malloc(n * sizeof(Ant));
    Ant *f4 = malloc(n * sizeof(Ant));
    Ant *f5 = malloc(n * sizeof(Ant));
    Ant *f6 = malloc(n * sizeof(Ant));


    initialize_AntPos(x_start_road, y_start_road, &*f1,&*f2,&*f3,&*f4,&*f5,&*f6);

    Ant *filas[6] = {f1,f2,f3,f4,f5,f6};



    if (!background_t) {
        printf("Error creating text: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }


    SDL_RenderPresent(rend);
    printf("Initialization succesful!\n");

    blackAnt_r.x = x_startAntHill + 200;
    blackAnt_r.y = y_startAntHill + 50;
    int close_requested = 0;
    int redAntRequested = 0;
    int blackAntRequested = 0;

    initializeNPC(rend,win);
    while (!close_requested) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close_requested = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_B:

                            blackAntRequested  = 1;
                            break;
                        case SDL_SCANCODE_R:

                            redAntRequested  = 1;
                            break;
                    }
                    break;
            }

        }

        drawBackground(rend, background_t, anthill_t, antHill1_r, antHill2_r, horizontal_roads, verticals_roads,
                       mini_roads);
        drawCells(rend);
        if (blackAntRequested){
            printf("Spawn Ant\n");
            enum antType type = black;
            spawnAnt(antX_start,antY_start,type,'r');
            blackAntRequested = 0;
        }

        if (redAntRequested){
            printf("Spawn Ant\n");
            enum antType type = red;
            spawnAnt(antX_start,antY_start,type,'r');
            redAntRequested = 0;
        }
        updateNPC(rend);
        SDL_RenderPresent(rend);
        SDL_Delay(900);



    }

}