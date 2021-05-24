#include <stdio.h>
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_timer.h"

//#include "../Synchronizer/synchronizer.h"
//

#include "variables.c"
#include "../Scheduler/LinkedList.h"
#include "functions.c"
#include "antsInterface.c"



int main() {




    //control de flujo para canales
    channel_Ants = malloc(sizeof (channelAnts)*3);
    synchronizerInit();
    synchronizer_Ants_Init();


    for(int i = 0;i<channelCount;i++){
        channel_Ants[i].channelNumber = i;
        channel_Ants[i].controlFLow = fileValues[i].metodoControlFlujo;
        channel_Ants[i].scheduler_selected = fileValues[i].calendarizador;
        channel_Ants[i].largoCanal = fileValues[i].largoCanal;
        channel_Ants[i].countAntsWait = fileValues[i].cantidadHormigasOrdenadas;
        channel_Ants[i].timer = fileValues[i].timeCambioLetrero;
        channel_Ants[i].count_W = fileValues[i].parametroW;
        channel_Ants[i].valueRMS = fileValues[i].tiempoMaximoRMS;
        channel_Ants[i].list_Ants_L = NULL;
        channel_Ants[i].list_Ants_R = NULL;
        channel_Ants[i].spacesInBridge = fileValues[i].largoCanal;
        channel_Ants[i].semaforoActive_L = 0;
        channel_Ants[i].semaforoActive_R = 0;
        channel_Ants[i].passedAnts = 0;
        channel_Ants[i].sideFlag =1;
        channel_Ants[i].parametroW_Fixed = fileValues[i].parametroW;
        if (channel_Ants[i].controlFLow > 0){
            init_alarm(fileValues[i].timeCambioLetrero , i, channel_Ants[i].controlFLow);
            channel_Ants[i].count_W = w_timer;
        }
    }




    // Se inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
//        printf("error initializing SDL:%s\n", SDL_GetError());
        return 1;
    }

    initAudio();

    SDL_Window *win = SDL_CreateWindow("Matrix Hill", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (!win) {
//        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //Se crea el render que va a contener la ventana
    Uint8 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend) {
//        printf("Error creating render: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Cargamos las imagenes
    SDL_Surface *background_s = IMG_Load("../Resources/background.png");
    SDL_Surface *anthill_s = IMG_Load("../Resources/antHill.png");
    SDL_Surface *blackAnt_s = IMG_Load("../Resources/blackAntR.png");
    SDL_Surface *letreroL_s = IMG_Load("../Resources/letreroL.png");
    SDL_Surface *letreroR_s = IMG_Load("../Resources/letreroR.png");

    if (!anthill_s || !background_s) {
//        printf("Error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Creamos las texturas
    SDL_Texture *background_t = SDL_CreateTextureFromSurface(rend, background_s);
    SDL_Texture *anthill_t = SDL_CreateTextureFromSurface(rend, anthill_s);
    SDL_Texture *blackAnt_t = SDL_CreateTextureFromSurface(rend, blackAnt_s);
    SDL_Texture *letreroR_t = SDL_CreateTextureFromSurface(rend, letreroR_s);
    SDL_Texture *letreroL_t = SDL_CreateTextureFromSurface(rend, letreroL_s);

    //Creamos los contenedores de los objetos y les cargamos las imagenes
    SDL_Rect letrero1;
    SDL_Rect letrero2;
    SDL_Rect letrero3;



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
    w_ant = blackAnt_r.w;
    h_ant = blackAnt_r.h;

    cellSize = w_horizontal_road / sizeOfStack;
    drawLines(rend, sizeOfStack, x_start_road, y_start_road);
    int x_startPoint = x_start_road + distanceBetweenRoads / 2 + gap_roads / 2;
    drawLines(rend, sizeOfStack, x_startPoint, y_start_road);
    SDL_Rect canal_road;
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


//    printf("C0: %i \n",channel_Ants[0].largoCanal);
//    printf("C1: %i \n",channel_Ants[1].largoCanal);
//    printf("C2: %i \n",channel_Ants[2].largoCanal);


    //Setteamos los del canal 0
    Matrix *f1 = malloc((sizeOfStack * 2 + channel_Ants[0].largoCanal) * sizeof(Matrix));
    Matrix *f2 = malloc((sizeOfStack * 2 + channel_Ants[0].largoCanal) * sizeof(Matrix));
    //Setteamos los del canal 1

    Matrix *f3 = malloc((sizeOfStack * 2 + channel_Ants[1].largoCanal) * sizeof(Matrix));
    Matrix *f4 = malloc((sizeOfStack * 2 + channel_Ants[1].largoCanal) * sizeof(Matrix));
    //Setteamos los del canal 2
    Matrix *f5 = malloc((sizeOfStack * 2 + channel_Ants[2].largoCanal) * sizeof(Matrix));
    Matrix *f6 = malloc((sizeOfStack * 2 + channel_Ants[2].largoCanal) * sizeof(Matrix));


    initialize_AntPos(x_start_road, y_start_road, &*f1,&*f2,&*f3,&*f4,&*f5,&*f6);

    Matrix *filas[6] = {f1,f2,f3,f4,f5,f6};



    if (!background_t) {
//        printf("Error creating text: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }


    SDL_RenderPresent(rend);
//    printf("Initialization succesful!\n");

    blackAnt_r.x = x_startAntHill + 200;
    blackAnt_r.y = y_startAntHill + 50;
    int close_requested = 0;
    int redAntRequested = 0;
    int blackAntRequested = 0;
    int queenAntRequested = 0;

    //Inicializamos los calendarizadores
    //synchronizerInit();

    letrero1.x = filas[0][STACKMAX ]->x;
    letrero1.y = filas[0][STACKMAX ]->y - 120;

    letrero2.x = filas[2][STACKMAX ]->x;
    letrero2.y = filas[2][STACKMAX ]->y - 120;

    letrero3.x = filas[4][STACKMAX ]->x;
    letrero3.y = filas[4][STACKMAX ]->y - 120;


    SDL_QueryTexture(letreroR_t, NULL, NULL, &letrero1.w, &letrero1.h);
    SDL_QueryTexture(letreroR_t, NULL, NULL, &letrero2.w, &letrero2.h);
    SDL_QueryTexture(letreroR_t, NULL, NULL, &letrero3.w, &letrero3.h);

    struct timespec {
        time_t tv_sec;
        long   tv_nsec;
    };
    struct timespec tiempo ;
    tiempo.tv_sec = 0;
    tiempo.tv_nsec = 1000000;

    //

    int sendHomeRequested = 0;
    initializeNPC(rend,win);
    int fila = 0;
    char side = 'l';

    while (!close_requested) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close_requested = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                            queenAntRequested  = 1;
                            side = 'l';
                            fila = 0;
                            break;
                        case SDL_SCANCODE_S:

                            queenAntRequested  = 1;
                            fila = 2;
                            side = 'l';
                            break;
                        case SDL_SCANCODE_X:

                            queenAntRequested  = 1;
                            fila = 4;
                            side = 'l';
                            break;
                        case SDL_SCANCODE_E:
                            fila = 0;
                            redAntRequested  = 1;
                            side = 'l';
                            break;
                        case SDL_SCANCODE_D:
                            fila = 2;
                            redAntRequested  = 1;
                            side = 'l';
                            break;
                        case SDL_SCANCODE_C:
                            fila = 4;
                            redAntRequested  = 1;
                            side = 'l';
                            break;
                        case SDL_SCANCODE_R:
                            fila = 0;
                            blackAntRequested  = 1;
                            side = 'l';
                            break;
                        case SDL_SCANCODE_F:
                            fila = 2;
                            blackAntRequested  = 1;
                            side = 'l';
                            break;
                        case SDL_SCANCODE_V:
                            fila = 4;
                            blackAntRequested  = 1;
                            side = 'l';
                            break;
                            //Aca empiezan las del lado derecho
                        case SDL_SCANCODE_U:
                            queenAntRequested  = 1;
                            side = 'r';
                            fila = 1;
                            break;
                        case SDL_SCANCODE_H:

                            queenAntRequested  = 1;
                            fila = 3;
                            side = 'r';
                            break;
                        case SDL_SCANCODE_B:

                            queenAntRequested  = 1;
                            fila = 5;
                            side = 'r';
                            break;
                        case SDL_SCANCODE_I:
                            fila = 1;
                            redAntRequested  = 1;
                            side = 'r';
                            break;
                        case SDL_SCANCODE_J:
                            fila = 3;
                            redAntRequested  = 1;
                            side = 'r';
                            break;
                        case SDL_SCANCODE_N:
                            fila = 5;
                            redAntRequested  = 1;
                            side = 'r';
                            break;
                        case SDL_SCANCODE_O:
                            fila = 1;
                            blackAntRequested  = 1;
                            side = 'r';
                            break;
                        case SDL_SCANCODE_K:
                            fila = 3;
                            blackAntRequested  = 1;
                            side = 'r';
                            break;
                        case SDL_SCANCODE_M:
                            fila = 5;
                            blackAntRequested  = 1;
                            side = 'r';
                            break;




                    }
                    break;
            }

        }
        //drawAnts(rend, filas, blackAnt_r, blackAnt_t);

        drawBackground(rend, background_t, anthill_t, antHill1_r, antHill2_r, horizontal_roads, verticals_roads,
                       mini_roads, canal_road, letreroR_t,letreroL_t,letrero1,letrero2,letrero3);
        drawCells(rend);
        if (sendHomeRequested){
            printf("Send home\n");
            for(int counter = 0; counter < antCounter; counter++) {
                ants[counter].sentHome = 1;
            }
            sendHomeRequested = 0;
        }
        if (blackAntRequested){

            enum antType type = black;

            spawnAnt(fila,type,side,filas);
            blackAntRequested = 0;
        }
        if (queenAntRequested){

            enum antType type = queen;
            spawnAnt(fila,type,side,filas);
            queenAntRequested = 0;
        }


        if (redAntRequested){
            enum antType type = red;
            spawnAnt(fila,type,side,filas);
            redAntRequested = 0;
        }
        updateNPC(rend, filas);
//        drawAnts(rend, filas, blackAnt_r, blackAnt_t);
        SDL_RenderPresent(rend);
//        if (hay_threads) {
//            CEThread_yield();
//        }
//       SDL_Delay(10);
    }

}