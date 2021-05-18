
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <pthread.h>


struct Params {

    int antId;
    Matrix **filas;
};

int initializeNPC(SDL_Renderer* rend, SDL_Window *win){

    //Cargar imagenes
    SDL_Surface *surface_BlackAntR = IMG_Load("../Resources/blackAntR.png");
    SDL_Surface *surface_RedAntR = IMG_Load("../Resources/redAntR.png");
    SDL_Surface *surface_QueenAntR = IMG_Load("../Resources/queenAntR.png");
    SDL_Surface *surface_BlackAntL = IMG_Load("../Resources/blackAntL.png");
    SDL_Surface *surface_RedAntL = IMG_Load("../Resources/redAntL.png");
    SDL_Surface *surface_QueenAntL = IMG_Load("../Resources/queenAntL.png");

    //Manejo de error al cargar
    if (
            !surface_BlackAntR || !surface_RedAntR || !surface_BlackAntL || !surface_RedAntL || !surface_QueenAntR || !surface_QueenAntL
    ) {
        printf("Error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Crear texturas desde la imagen
    antSprites.blackRight1 = SDL_CreateTextureFromSurface(rend, surface_BlackAntR);
    SDL_FreeSurface(surface_BlackAntR);
    antSprites.blackLeft1 = SDL_CreateTextureFromSurface(rend, surface_BlackAntL);
    SDL_FreeSurface(surface_BlackAntL);
    antSprites.redRight1 = SDL_CreateTextureFromSurface(rend, surface_RedAntR);
    SDL_FreeSurface(surface_RedAntR);
    antSprites.redLeft1 = SDL_CreateTextureFromSurface(rend, surface_RedAntL);
    SDL_FreeSurface(surface_RedAntL);
    antSprites.queenRight1 = SDL_CreateTextureFromSurface(rend, surface_QueenAntR);
    SDL_FreeSurface(surface_QueenAntR);
    antSprites.queenLeft1 = SDL_CreateTextureFromSurface(rend, surface_QueenAntL);
    SDL_FreeSurface(surface_QueenAntL);

    //Manejo de errores al crear las texturas
    if(
            !antSprites.blackRight1 || !antSprites.blackLeft1 || !antSprites.redRight1 || !antSprites.redLeft1 || !antSprites.queenRight1 || !antSprites.queenLeft1
            ){
        printf("Error creating texture: %s\n",SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    printf("Initialize NPC succesfull\n");
}


void* startAntMotion(void* params){
    struct Params * p = params;
    struct timespec {
        time_t tv_sec;
        long   tv_nsec;
    };
    struct timespec tiempo ;
    tiempo.tv_sec = 0;
    tiempo.tv_nsec = 100000000;
    while (1){
//        printf("Ejecutando movimiento de hormiga %i \n", p->antId);
        nanosleep(&tiempo,&tiempo);

        if (ants[p->antId].sentHome){
            sendHome(p->antId,ants[p->antId].side);
            //CEThread_yield();
            continue;
        }

        if (ants[p->antId].waiting){
            //CEThread_yield();
            continue;
        }
        if (positionInInitialRow( p->antId, ants[p->antId].side)){
            moveAntInStack(p->antId,  ants[p->antId].side, p->filas);
            //CEThread_yield();
            continue;
        }
        else{
            //CEThread_yield();
            continue;
        }

    }
}
void postionAnt(){
    int listSize = getCount_t(list_Ant_A_Canal1)+1;
    for(int i = 0; i < listSize;i++ ){
        for (int j = 0; j < antCounter ;j++){
            if(list_Ant_A_Canal1->dataInfo->antId == j){
                printf("Se le va a asignar una columna a la hormiga %i  \n", STACKMAX);
                ants[j].col_dest = STACKMAX;
            }
        }

    }
}

void spawnAnt(int fila, int columna, enum antType type, char side, Matrix *filas[6] ){
    if(antCounter < maxAnts) {
        dataItem  * hormiga0 = malloc(sizeof (dataItem));
        switch(type){
            //dataItem  * dataItem ;

            case black:
                hormiga0->state = 0;
                hormiga0->priority= 10;
                hormiga0->var_SJF = 4;
                hormiga0->scheduler_Selected = 0; // TODO meter el archivo de configuracion segun el canal
                hormiga0->rms_C = 1;
                hormiga0->rms_P = 6;
                //hormiga0->tid = toledoAnt_1;
                hormiga0->column = 0;
                hormiga0->row = 0;
                //dataItem  * dataInfo ;
                hormiga0->antId = antCounter;
                ants[antCounter].dataItem = *hormiga0;

               // ants[antCounter].speed = 1;
                //ants[antCounter].dataItem = ;
                if(side == 'r' ){
                    SDL_QueryTexture(antSprites.blackLeft1, NULL, NULL,&(ants[antCounter].size.w), &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = blackLeft1;
                    ants[antCounter].size.x = antHill_x + distanceBetweenHills - 20;
                    ants[antCounter].size.y = antHill_y + 100;

                }
                if(side == 'l'){

                    SDL_QueryTexture(antSprites.blackRight1, NULL, NULL,&(ants[antCounter].size.w), &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = blackRight1;
                    ants[antCounter].size.x = x_start_road - 100;
                    ants[antCounter].size.y = antHill_y + 50;

                }
                break;
            case red:
                ants[antCounter].speed = 1.5;

                hormiga0->state = 0;
                hormiga0->priority= 10;
                hormiga0->var_SJF = 4;
                hormiga0->scheduler_Selected = 0; // TODO meter el archivo de configuracion segun el canal
                hormiga0->rms_C = 1;
                hormiga0->rms_P = 6;
                //hormiga0->tid = toledoAnt_1;
                hormiga0->column = 0;
                hormiga0->row = 0;
                //dataItem  * dataInfo ;
                ants[antCounter].dataItem = *hormiga0;

                if(side == 'r' ){

                    SDL_QueryTexture(antSprites.redLeft1, NULL, NULL, &(ants[antCounter].size.w), &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = redLeft1;
                    ants[antCounter].size.x = antHill_x + distanceBetweenHills - 20;
                    ants[antCounter].size.y = antHill_y + 100;
                }
                if(side == 'l'){

                    SDL_QueryTexture(antSprites.redRight1, NULL, NULL, &(ants[antCounter].size.w), &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = redRight1;
                    ants[antCounter].size.x = x_start_road - 100;
                    ants[antCounter].size.y = antHill_y + 50;
                }
                break;
            case queen:
                ants[antCounter].speed = 1;

                hormiga0->state = 0;
                hormiga0->priority= 10;
                hormiga0->var_SJF = 4;
                hormiga0->scheduler_Selected = 0; // TODO meter el archivo de configuracion segun el canal
                hormiga0->rms_C = 1;
                hormiga0->rms_P = 6;
                //hormiga0->tid = toledoAnt_1;
                hormiga0->column = 0;
                hormiga0->row = 0;
                //dataItem  * dataInfo ;
                ants[antCounter].dataItem = *hormiga0;

                if(side == 'r' ){

                    SDL_QueryTexture(antSprites.queenLeft1, NULL, NULL, &(ants[antCounter].size.w), &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = queenLeft1;
                    ants[antCounter].size.x = antHill_x + distanceBetweenHills - 20;
                    ants[antCounter].size.y = antHill_y + 100;
                }
                if(side == 'l'){

                    SDL_QueryTexture(antSprites.queenRight1, NULL, NULL, &(ants[antCounter].size.w), &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = queenRight1;
                    ants[antCounter].size.x = x_start_road - 100;
                    ants[antCounter].size.y = antHill_y + 50;
                }
                break;

        }

        ants[antCounter].fila_act = fila;
        if(side == 'l') ants[antCounter].col_act = -1;
        if(side == 'r') ants[antCounter].col_act = COLAMAX + STACKMAX * 2 - 1;
//        ants[antCounter].col_dest = columna;

        ants[antCounter].size.h *= 0.06;
        ants[antCounter].size.w *= 0.03;
        ants[antCounter].side = side;
        ants[antCounter].x_dest = filas[fila][columna]->x;
        ants[antCounter].y_dest = filas[fila][columna]->y;
        ants[antCounter].type = type;
        ants[antCounter].antId = antCounter;
        ants[antCounter].sentHome = 0;
        ants[antCounter].passedBridge = 0;


        push_t(&list_Ant_A_Canal1, &ants[antCounter].dataItem);

        postionAnt();

//        dataList  * hormiga0 = malloc(sizeof (dataList));
//        hormiga0->state = 0;
//        hormiga0->priority= 10;
//        hormiga0->var_SJF = 4;
//        hormiga0->scheduler_Selected = scheduler_Selected;
//        hormiga0->rms_C = 1;
//        hormiga0->rms_P = 6;
//        hormiga0->tid = toledoAnt_1;
//        hormiga0->column = 0;
//        hormiga0->row = 0;



        //CEThread_t thread1;
        pthread_t thread1;
        struct Params *param;
        param = malloc(sizeof(struct Params));
        param->antId = ants[antCounter].antId;
        param->filas = filas;
//        printf("Canal Scheduler:%i \n",scheduler->canalNumber);
//        CEThread_create( &thread1,startAntMotion, param, scheduler,scheduler->canalNumber);
        pthread_create( &thread1,NULL,startAntMotion, param);

        antCounter++;
    }
}

void updateNPC(SDL_Renderer *rend, Matrix *filas[6]) {

    //Ants
    int counter;
    for(counter = 0; counter < antCounter; counter++) {



        //Comportamiento de acuerdo al tipo
        SDL_Texture *sprite;
        switch (ants[counter].type) {
            case black:
                //Movimiento

                //Animacion
                switch (ants[counter].currentSprite) {
                    case blackRight1:
                        sprite = antSprites.blackRight1;
                        break;

                    case blackLeft1:
                        sprite = antSprites.blackLeft1;
                        break;
                }
                break;
            case red:
                //Movimiento

                //Animacion
                switch (ants[counter].currentSprite) {
                    case redRight1:
                        sprite = antSprites.redRight1;
                        break;
                    case redLeft1:
                        sprite = antSprites.redLeft1;
                        break;
                }
                break;
            case queen:
                //Movimiento

                //Animacion
                switch (ants[counter].currentSprite) {
                    case queenRight1:
                        sprite = antSprites.queenRight1;
                        break;
                    case queenLeft1:
                        sprite = antSprites.queenLeft1;
                        break;
                }
                break;
        }
        bool passedAntCross = detectIfAntCross(counter,ants[counter].side);



        SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);



    }
}

