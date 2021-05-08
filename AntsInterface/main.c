#include <stdio.h>
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_image.h"
#include <stdbool.h>
#include "SDL2/SDL_mixer.h"

# define MAX 10
#define WINDOW_WIDTH (1850)
#define WINDOW_HEIGHT (1050)
#define x_start_road (400)
#define y_start_road (100)
#define gap_roads (55)
#define w_horizontal_road (400)
#define w_vertical_road (50)
#define h_horizontal_road (100)
#define h_vertical_road (800)
#define antHill_x (30)
#define antHill_y (400)
#define sizeOfCanal (8)
#define distanceBetweenHills (1550)
#define distanceBetweenRoads ( distanceBetweenHills - 700)

typedef struct {

    char antName[21];
    int x;
    int y;
} Ant[MAX];

void drawBackground(SDL_Renderer *rend, SDL_Texture *background_t, SDL_Texture *anthill_t,
                    SDL_Rect antHill1_r, SDL_Rect antHill2_r, SDL_Rect horizontal_roads, SDL_Rect vertical_roads,
                    SDL_Rect mini_roads);

void initAudio();

void drawVerticalRoads(SDL_Renderer *rend, SDL_Rect vertical_roads, int x_startPoint, int y_startPoint);

void drawHorizontalRoads(SDL_Renderer *rend, SDL_Rect horizontal_roads, int x_startPoint, int y_startPoint);

void drawRoads(SDL_Renderer *rend, SDL_Rect horizontal_roads, SDL_Rect vertical_roads);

void drawCells(SDL_Renderer *rend);

void drawHills(SDL_Renderer *rend, SDL_Texture *anthill_t, SDL_Rect antHill1_r, SDL_Rect antHill2_r,
               SDL_Rect horizontal_roads);

void drawLines(SDL_Renderer *rend, int largoCanal, int x_start, int y_start);

void initialize_AntPos(int x_start, int y_start, Ant *f1,Ant *f2,Ant *f3,Ant *f4,Ant *f5,Ant *f6);
void drawAnts(SDL_Renderer *rend, Ant *filas[6], SDL_Rect blackAnt_r, SDL_Texture *blackAnt_t);

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
        printf("Error creating renderer: %s\n", SDL_GetError());
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
    SDL_RenderCopy(rend, blackAnt_t, NULL, &blackAnt_r);

    const int NCOLS = sizeOfCanal;
    const int NROWS = 6;

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
    bool quit = false;


    while (!quit) {
//        if (blackAnt_r.x > 2000){
//            //quit = true;
//        }
        drawBackground(rend, background_t, anthill_t, antHill1_r, antHill2_r, horizontal_roads, verticals_roads,
                       mini_roads);

        //blackAnt_r.x += 100;
        drawAnts(rend,filas,blackAnt_r,blackAnt_t);

        drawCells(rend);
        SDL_RenderPresent(rend);
        SDL_Delay(900);

    }

    SDL_Quit();


    return 0;
}


void drawCells(SDL_Renderer *rend) {
    drawLines(rend, sizeOfCanal, x_start_road, y_start_road);
    int x_startPoint = x_start_road + distanceBetweenRoads / 2 + gap_roads / 2;
    drawLines(rend, sizeOfCanal, x_startPoint, y_start_road);
}


void drawAnts(SDL_Renderer *rend, Ant *filas[6], SDL_Rect blackAnt_r, SDL_Texture *blackAnt_t){
    for (int i = 0; i < 6; ++i){
        for (int j = 0; j < sizeOfCanal; ++j) {
            printf("x:%i ", filas[i][j]->x);
            printf("y:%i\n", filas[i][j]->y);

            blackAnt_r.x = filas[i][j]->x;
            blackAnt_r.y = filas[i][j]->y;

            SDL_RenderCopy(rend, blackAnt_t, NULL, &blackAnt_r);

        }
    }
}
void drawLines(SDL_Renderer *rend, int largoCanal, int x_start, int y_start) {
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    int x1 = x_start + 2 * w_vertical_road;

    int y1 = y_start;
    int y2 = y_start + h_horizontal_road;
    int gabInHorizontal = h_vertical_road / 3 - h_horizontal_road / 3.5;


    for (int i = 0; i != 3; i++) {
        for (int i = 0; i < largoCanal; i++) {

            SDL_RenderDrawLine(rend, x1, y1, x1, y2);
            x1 += w_horizontal_road / largoCanal;
        }
        y1 += 2 * gap_roads + gabInHorizontal;
        y2 = y1 + h_horizontal_road;
        x1 = x_start + 2 * w_vertical_road;
    }
}

void initialize_AntPos(int x_start, int y_start, Ant *f1,Ant *f2,Ant *f3,Ant *f4,Ant *f5,Ant *f6) {

    int x1 = x_start + 2 * w_vertical_road;
    int y1 = y_start;
    int sizeOfCell = w_horizontal_road / sizeOfCanal;

    Ant *array[6] = {f1,f2,f3,f4,f5,f6};
    int gabInHorizontal = h_vertical_road / 3 - h_horizontal_road / 3.5;

    for (int i = 0; i != 6; i++) {
        if (i == 0 || i == 1) {
            for (int j = 0; j < sizeOfCanal; j++) {

                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 = x_start + 2 * w_vertical_road;
            y1 += gap_roads;

        }
        if (i == 2) { y1 += gabInHorizontal; }
        if (i == 2 || i == 3) {
            for (int j = 0; j < sizeOfCanal; j++) {

                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 = x_start + 2 * w_vertical_road;
            y1 += gap_roads;

        }
        if (i == 4) { y1 += gabInHorizontal; }
        if (i == 4 || i == 5) {
            for (int j = 0; j < sizeOfCanal; j++) {

                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 = x_start + 2 * w_vertical_road;
            y1 += gap_roads;

        }
    }







    for (int i = 0; i != 6; i++) {


        if (i == 1 || i == 3 || i == 5) y1 += gabInHorizontal;
        if (i == 0 || i == 2 || i == 4) y1 += gap_roads;

    }
}

void drawRoads(SDL_Renderer *rend, SDL_Rect horizontal_roads, SDL_Rect vertical_roads) {
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    horizontal_roads.w = distanceBetweenRoads / 2 - gap_roads / 2;
    horizontal_roads.h = 50;


    vertical_roads.w = 50;
    vertical_roads.h = h_vertical_road;

    drawHorizontalRoads(rend, horizontal_roads, x_start_road, y_start_road);


    int x_startPoint = x_start_road + distanceBetweenRoads / 2 + gap_roads / 2;


    drawHorizontalRoads(rend, horizontal_roads, x_startPoint, y_start_road);


    drawVerticalRoads(rend, vertical_roads, x_start_road, y_start_road);
    drawVerticalRoads(rend, vertical_roads, x_start_road + distanceBetweenRoads + w_vertical_road * 2, y_start_road);

}

void drawVerticalRoads(SDL_Renderer *rend, SDL_Rect vertical_roads, int x_startPoint, int y_startPoint) {
    vertical_roads.x = x_startPoint;
    vertical_roads.y = y_startPoint;
    for (int i = 0; i != 4; i++) {
        if (i == 0 || i == 1) {
            SDL_RenderFillRect(rend, &vertical_roads);
            vertical_roads.x += gap_roads;
        }
    }
}

void drawHorizontalRoads(SDL_Renderer *rend, SDL_Rect horizontal_roads, int x_startPoint, int y_startPoint) {
    horizontal_roads.x = x_startPoint + 2 * w_vertical_road;
    horizontal_roads.y = y_startPoint;
    int gabInHorizontal = h_vertical_road / 3 - h_horizontal_road / 3.5;
    for (int i = 0; i != 6; i++) {
        if (i == 0 || i == 1) {
            SDL_RenderFillRect(rend, &horizontal_roads);
            horizontal_roads.y += gap_roads;

        }
        if (i == 2) { horizontal_roads.y += gabInHorizontal; }
        if (i == 2 || i == 3) {
            SDL_RenderFillRect(rend, &horizontal_roads);
            horizontal_roads.y += gap_roads;

        }
        if (i == 4) { horizontal_roads.y += gabInHorizontal; }
        if (i == 4 || i == 5) {
            SDL_RenderFillRect(rend, &horizontal_roads);
            horizontal_roads.y += gap_roads;

        }
    }
}

void initAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void drawBackground(SDL_Renderer *rend, SDL_Texture *background_t, SDL_Texture *anthill_t, SDL_Rect antHill1_r,
                    SDL_Rect antHill2_r, SDL_Rect horizontal_roads, SDL_Rect vertical_roads, SDL_Rect mini_roads) {
    SDL_RenderClear(rend);


    SDL_RenderCopy(rend, background_t, NULL, NULL);


    drawHills(rend, anthill_t, antHill1_r, antHill2_r, mini_roads);
    drawRoads(rend, horizontal_roads, vertical_roads);

}

void
drawHills(SDL_Renderer *rend, SDL_Texture *anthill_t, SDL_Rect antHill1_r, SDL_Rect antHill2_r, SDL_Rect mini_roads) {
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

    SDL_RenderFillRect(rend, &mini_roads);

    SDL_RenderCopy(rend, anthill_t, NULL, &antHill1_r);

    SDL_RenderCopy(rend, anthill_t, NULL, &antHill2_r);

    int temp_gap = 0;
    for (int i = 0; i != 2; i++) {
        mini_roads.x = antHill1_r.x + antHill1_r.w;
        mini_roads.y = y_start_road + h_vertical_road / 2 - 50 + temp_gap;
        SDL_RenderFillRect(rend, &mini_roads);
        temp_gap += gap_roads;
    }

    temp_gap = 0;
    for (int i = 0; i != 2; i++) {
        mini_roads.x = antHill2_r.x - 125;
        mini_roads.y = y_start_road + h_vertical_road / 2 - 50 + temp_gap;
        SDL_RenderFillRect(rend, &mini_roads);
        temp_gap += gap_roads;
    }
}

