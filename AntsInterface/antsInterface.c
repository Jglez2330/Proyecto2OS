
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../Scheduler/Scheduler.h"
#include "../CEThread/CEThread.h"
#include <unistd.h>
//#include "../Scheduler/LinkedList.h"


int alpha = 0;
int hay_threads;

static struct sigaction letrero_alarm;
struct itimerval timer_letrero;
struct Params {

    int antId;
    Matrix **filas;
};
CEThread_mutex_t mutex;
void alarm_handler(int sig);
void init_alarm(long time){

    sigemptyset( &letrero_alarm.sa_mask );
    letrero_alarm.sa_flags = 0;
    letrero_alarm.sa_handler = alarm_handler;
    sigaction( SIGALRM, &letrero_alarm, NULL );

    alarm(5);  /* timer will pop in five seconds */
}
void alarm_handler(int sig){
    char* r = "Hola Mundo!\n";
    write(1, r, strlen(r));
    alarm(5);

}
int initializeNPC(SDL_Renderer *rend, SDL_Window *win) {

    //Cargar imagenes
    SDL_Surface *surface_BlackAntR = IMG_Load("../Resources/blackAntR.png");
    SDL_Surface *surface_RedAntR = IMG_Load("../Resources/redAntR.png");
    SDL_Surface *surface_QueenAntR = IMG_Load("../Resources/queenAntR.png");
    SDL_Surface *surface_BlackAntL = IMG_Load("../Resources/blackAntL.png");
    SDL_Surface *surface_RedAntL = IMG_Load("../Resources/redAntL.png");
    SDL_Surface *surface_QueenAntL = IMG_Load("../Resources/queenAntL.png");

    //Manejo de error al cargar
    if (
            !surface_BlackAntR || !surface_RedAntR || !surface_BlackAntL || !surface_RedAntL || !surface_QueenAntR ||
            !surface_QueenAntL
            ) {
//        printf("Error creating surface\n");
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
    if (
            !antSprites.blackRight1 || !antSprites.blackLeft1 || !antSprites.redRight1 || !antSprites.redLeft1 ||
            !antSprites.queenRight1 || !antSprites.queenLeft1
            ) {
//        printf("Error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
//    printf("Initialize NPC succesfull\n");
}
//[4,2,3,0]
//4 == 4

void setMovingAnts() {
    for (int i = 0; i < antCounter; i++) {
        ants[antCounter].notSorting = 0;
        ants[antCounter].waiting = 0;
    }
}

void postionAllAnt(listNode_t list, Matrix *filas[6]) {
//        printf("\nREORDENANDO \n");
    int listSize = getCount_t(&list) + 1;
//    printf("\nCANTIDAD DE HORMIGAS %i\n", listSize);
    for (int i = 0; i < listSize; i++) {
//        printf("El elemento de la lista es : %i \n", getNode_t(list_Ant_A_Canal1, i)->antId);
        for (int j = 0; j < antCounter; j++) {
            if (getNode_t(&list, i)->antId == ants[j].antId) {
//                printf("Se le va a asignar una columna a la hormiga %i  a la hormiga numero %i \n", (STACKMAX - 1) - i, j);
                if (ants[j].side == 'l') {
                    ants[j].col_dest = (STACKMAX - 1) - i;
//                    ants[antCounter].finalX = filas[ants[j].fila_dest][ants[j].col_dest]->x;
//                    ants[antCounter].finalY = filas[ants[j].fila_dest][ants[j].col_dest]->y;


                } else if (ants[j].side == 'r') {
                    ants[j].col_dest = (STACKMAX + channel_Ants[ants[j].canal].largoCanal) + i;
//                    ants[antCounter].finalX = filas[ants[j].fila_dest][ants[j].col_dest]->x;
//                    ants[antCounter].finalY = filas[ants[j].fila_dest][ants[j].col_dest]->y;

                }
            }
        }
    }
}

void crossAnt(int id) {

    if (ants[id].antId == id) {
        if (ants[id].side == 'l') {
            ants[id].col_dest = STACKMAX + channel_Ants[ants[id].canal].largoCanal + 2;
            ants[id].dataItem.state = 99;
            ants[id].colitionFlag = 1;

        } else if (ants[id].side == 'r') {
            ants[id].col_dest = STACKMAX - 2;
            ants[id].colitionFlag = 1;
            ants[id].dataItem.state = 99;
        }
    }
}


int equidad() {

}

int countAntsWaiting(long canal, char side) {
    int cuenta = 0;
    for (int i = 0; i < antCounter; i++) {
        if (ants[i].canal == canal && ants[i].dataItem.state == 1 && ants[i].side == side)cuenta++;
    }
    return cuenta;
}

/*
int ants_Waiting_2_Terminated(long canal, char side) {
    for (int i = 0; i < antCounter; i++) {
        if (ants[i].canal == canal && ants[i].dataItem.state == 1 && ants[i].side == side)
            ants[i].dataItem.state = 99;
    }
    return 0;
}*/
int changeAntsWaiting(long canal, char side) {
    int cuenta = 0;
    for (int i = 0; i < antCounter; i++) {
        if (ants[i].canal == canal && ants[i].dataItem.state == 1 && ants[i].side == side)ants[i].waiting = 2;
    }
    return cuenta;
}

int countAntsPassed(long canal, char side) {
    int cuenta = 0;
    for (int i = 0; i < antCounter; i++) {
        if (ants[i].canal == canal && ants[i].passedBridge == 1 && ants[i].side == side)cuenta++;
    }
    return cuenta;
}

bool antsFlowBridge(int antId_in, Matrix *filas[6]) {
    //channel_Ants[i].channelNumber = i;
    //channel_Ants[ants[antId_in].canal];

//    channel_Ants[ants[antId_in].canal]

    if (ants[antId_in].passedBridge == 1) {


        channel_Ants[ants[antId_in].canal].spacesInBridge += 1;
        ants[antId_in].passedBridge = 2;
        printf("Se va a desbloquear la hormiga que viene detras de la fila: %i \n",ants[antId_in].fila);
        printList_t(channel_Ants[ants[antId_in].canal].list_Ants_R);
        unblock_threads_from_list_ants(ants[antId_in].fila);
        if (ants[antId_in].fila % 2 == 0){
//            printf("Lista izquierda ordenada es:\n");
            write(1,"Lista izquierda derecha es:\n",strlen("Lista izquierda derecha es:\n"));
            printList_t(channel_Ants[ants[antId_in].canal].list_Ants_L);
        }
        else{
//            printf("Lista ordenada derecha es:\n");
            write(1,"Lista ordenada derecha es:\n", strlen("Lista ordenada derecha es:\n"));
            printList_t(channel_Ants[ants[antId_in].canal].list_Ants_R);
        }

    }
    if (ants[antId_in].sentHome) {
        sendHome(antId_in, ants[antId_in].side);
        return 1;
        //continue;
    }

    int hormigasEspearando = countAntsWaiting(ants[antId_in].canal, ants[antId_in].side);
//    printf("\nHormigas %i y side %c\n", hormigasEspearando, ants[antId_in].side);
//    printf("canal esperando %li", channel_Ants[ants[antId_in].canal].countAntsWait);
    if (channel_Ants[ants[antId_in].canal].countAntsWait <= hormigasEspearando) {

        if (ants[antId_in].side == 'r' && channel_Ants[ants[antId_in].canal].sideFlag == 1) {

            channel_Ants[ants[antId_in].canal].semaforoActive_R = 1;
            channel_Ants[ants[antId_in].canal].semaforoActive_L = 0;

        }
        if (ants[antId_in].side == 'l' && channel_Ants[ants[antId_in].canal].sideFlag == 0) {
            channel_Ants[ants[antId_in].canal].semaforoActive_L = 1;
            channel_Ants[ants[antId_in].canal].semaforoActive_R = 0;

        }
    }


    if (channel_Ants[ants[antId_in].canal].passedAnts == channel_Ants[ants[antId_in].canal].parametroW_Fixed) {
        channel_Ants[ants[antId_in].canal].passedAnts = 0;
        channel_Ants[ants[antId_in].canal].count_W = channel_Ants[ants[antId_in].canal].parametroW_Fixed;
        if (channel_Ants[ants[antId_in].canal].sideFlag == 1){
//            unblock_all_threads_ants(ants[antId_in].fila);
            int id = getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_L,0)->antId;
            printf("Se va a desbloquear el lado izquierdo de la fila: %i \n",ants[id].fila);
            unblock_all_threads_ants(ants[id].fila);
        }
        if (channel_Ants[ants[antId_in].canal].sideFlag == 0){
//            unblock_all_threads_ants(ants[antId_in].fila);
            int id  =getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_R,0)->antId;
            printf("Se va a desbloquear el lado derecho de la fila: %i \n",ants[id].fila);
            unblock_all_threads_ants(ants[id].fila);
        }
        //unblock_all_threads_ants(ants[antId_in].fila);

    }

    if (channel_Ants[ants[antId_in].canal].semaforoActive_L == 1
        //channel_Ants[ants[antId_in].canal].semaforoActive_L == 1
        && channel_Ants[ants[antId_in].canal].spacesInBridge != 0
        && channel_Ants[ants[antId_in].canal].list_Ants_L != NULL &&
            ants[antId_in].fila % 2 == 0 &&
        channel_Ants[ants[antId_in].canal].list_Ants_L->dataInfo != NULL
        && channel_Ants[ants[antId_in].canal].count_W != 0) {
        if (channel_Ants[ants[antId_in].canal].count_W == channel_Ants[ants[antId_in].canal].parametroW_Fixed) {
            listNode_t* temp_ants_list = copyList(channel_Ants[ants[antId_in].canal].list_Ants_L);
           // CEThread_mutex_lock(&mutex);
            init_scheduler(temp_ants_list, channel_Ants[ants[antId_in].canal].parametroW_Fixed, channel_Ants[ants[antId_in].canal].scheduler_selected,ants[antId_in].fila);
            block_threads_from_list(ants[antId_in].fila);
            unblock_threads_from_list_ants(ants[antId_in].fila);
        }
        channel_Ants[ants[antId_in].canal].count_W--;
//        printf("\nspacesInBrigde %li\n", channel_Ants[ants[antId_in].canal].spacesInBridge);

        if (channel_Ants[ants[antId_in].canal].count_W == 0) {
            //ants_Waiting_2_Terminated(ants[antId_in].canal, ants[antId_in].side);
            channel_Ants[ants[antId_in].canal].semaforoActive_L = 0;
            channel_Ants[ants[antId_in].canal].sideFlag = 1;
            channel_Ants[ants[antId_in].canal].semaforoActive_R = 0;
//            unblock_all_threads_ants(ants[antId_in].fila);
        }
        // pthread_mutex_lock(&mutex);
        //printList_t(list_Ant_L_Canal1);

//        printList_t(channel_Ants[ants[antId_in].canal].list_Ants_L);
        int id = getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_L, 0)->antId;
        ants[id].passingBridge = 1;

//        printList_t(channel_Ants[ants[antId_in].canal].list_Ants_L);



        deleteNodePosition(&channel_Ants[ants[antId_in].canal].list_Ants_L, 0);
        if (getFront_t(channel_Ants[ants[antId_in].canal].list_Ants_L) == NULL
                ) {
            channel_Ants[ants[antId_in].canal].list_Ants_L = NULL;
        }


        if (channel_Ants[ants[antId_in].canal].list_Ants_L != NULL) {
            channel_Ants[ants[antId_in].canal].spacesInBridge -= 1;
            setMovingAnts();
            schedulerSort(channel_Ants[ants[antId_in].canal].list_Ants_L);
            postionAllAnt(*channel_Ants[ants[antId_in].canal].list_Ants_L, filas);

        }
//        printf("CrossAnt hormiga: %i\n", id);
        crossAnt(id);
    }
    if (channel_Ants[ants[antId_in].canal].semaforoActive_R == 1
        //channel_Ants[ants[antId_in].canal].semaforoActive_L == 1
        && channel_Ants[ants[antId_in].canal].spacesInBridge != 0
        && channel_Ants[ants[antId_in].canal].list_Ants_R != NULL &&
            ants[antId_in].fila % 2 == 1 &&
        channel_Ants[ants[antId_in].canal].list_Ants_R->dataInfo != NULL
        && channel_Ants[ants[antId_in].canal].count_W != 0) {
        if (channel_Ants[ants[antId_in].canal].count_W == channel_Ants[ants[antId_in].canal].parametroW_Fixed) {
            listNode_t* temp_ants_list = copyList(channel_Ants[ants[antId_in].canal].list_Ants_R);
            // CEThread_mutex_lock(&mutex);
            printf("\n LA FILA ES %i \n", ants[antId_in].fila);

            init_scheduler(temp_ants_list, channel_Ants[ants[antId_in].canal].parametroW_Fixed,channel_Ants[ants[antId_in].canal].scheduler_selected,ants[antId_in].fila);
            block_threads_from_list(ants[antId_in].fila);
            unblock_threads_from_list_ants(ants[antId_in].fila);




        }
        channel_Ants[ants[antId_in].canal].count_W--;
//        printf("\nspacesInBrigde %li\n", channel_Ants[ants[antId_in].canal].spacesInBridge);

        if (channel_Ants[ants[antId_in].canal].count_W == 0) {
            //ants_Waiting_2_Terminated(ants[antId_in].canal, ants[antId_in].side);
            channel_Ants[ants[antId_in].canal].semaforoActive_R = 0;
            channel_Ants[ants[antId_in].canal].sideFlag = 0;
//            unblock_all_threads_ants(ants[antId_in].fila);
            channel_Ants[ants[antId_in].canal].semaforoActive_L = 0;
        }
        // pthread_mutex_lock(&mutex);
        //printList_t(list_Ant_L_Canal1);

//        printList_t(channel_Ants[ants[antId_in].canal].list_Ants_R);
        int id = getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_R, 0)->antId;
        ants[id].passingBridge = 1;

//        printList_t(channel_Ants[ants[antId_in].canal].list_Ants_R);



        deleteNodePosition(&channel_Ants[ants[antId_in].canal].list_Ants_R, 0);
        if (getFront_t(channel_Ants[ants[antId_in].canal].list_Ants_R) == NULL
                ) {
            channel_Ants[ants[antId_in].canal].list_Ants_R = NULL;
        }


        if (channel_Ants[ants[antId_in].canal].list_Ants_R != NULL) {
            channel_Ants[ants[antId_in].canal].spacesInBridge -= 1;
            setMovingAnts();
            schedulerSort(channel_Ants[ants[antId_in].canal].list_Ants_R);
            postionAllAnt(*channel_Ants[ants[antId_in].canal].list_Ants_R, filas);

        }
//        printf("CrossAnt hormiga: %i\n", id);
        crossAnt(id);
    }


}

void *startAntMotion(void *params) {
    struct Params *p = params;
    if (alpha++ == 0){
        CEThread_mutex_init(&mutex);
    }
//    printf("La fila de la hormiga es: %i \n", ants[p->antId].fila_act);
    while (1) {
//        printf("Ejecutando movimiento de hormiga %i \n", p->antId);
        //nanosleep(&tiempo, &tiempo);
        CEThread_yield();

        if (ants[p->antId].destroy) {
            return NULL;
        }

        bool continueFlag = antsFlowBridge(p->antId, p->filas);
        if (continueFlag) {
            continue;
        }


        if (positionInInitialRow(p->antId, ants[p->antId].side)) {

            if (ants[p->antId].fila_act % 2 == 0 && ants[p->antId].sorted == 0 &&
                channel_Ants[ants[p->antId].canal].list_Ants_L != NULL) {
                setMovingAnts();
//                printList_t(channel_Ants[ants[p->antId].canal].list_Ants_L);
                schedulerSort(channel_Ants[ants[p->antId].canal].list_Ants_L);
//                printList_t(channel_Ants[ants[p->antId].canal].list_Ants_L);
                postionAllAnt(*channel_Ants[ants[p->antId].canal].list_Ants_L, p->filas);

                ants[p->antId].sorted = 1;
            }
            if (ants[p->antId].fila_act % 2 != 0 && ants[p->antId].sorted == 0 &&
                       channel_Ants[ants[p->antId].canal].list_Ants_R != NULL) {
                setMovingAnts();
//                printList_t(channel_Ants[ants[p->antId].canal].list_Ants_R);
                schedulerSort(channel_Ants[ants[p->antId].canal].list_Ants_R);
//                printList_t(channel_Ants[ants[p->antId].canal].list_Ants_R);
                postionAllAnt(*channel_Ants[ants[p->antId].canal].list_Ants_R, p->filas);
                ants[p->antId].sorted = 1;
            } else {
                moveAntInStack(p->antId, p->filas);
                continue;
            }


            continue;
        } else {
            continue;
        }

    }
}

void postionInitialAnt(listNode_t list, int antCount) {
    int listSize = getCount_t(&list) + 1;
//    printf("\nCANTIDAD DE HORMIGAS %i\n", listSize);
    for (int i = 0; i < listSize; i++) {
//        printf("El elemento de la lista es : %i \n", getNode_t(list_Ant_A_Canal1, i)->antId);

        if (getNode_t(&list, i)->antId == antCount) {
//                printf("Se le va a asignar una columna a la hormiga %i  a la hormiga numero %i \n", (STACKMAX - 1) - i, j);
            if (ants[antCount].side == 'l') {
                ants[antCount].col_dest = (STACKMAX - 1) - i;
//                    printf("Columna asignada %i \n", ants[antCount].col_dest);
                return;
            } else if (ants[antCount].side == 'r') {
                ants[antCount].col_dest = (STACKMAX + channel_Ants[ants[antCount].canal].largoCanal) + i;
                return;
            }
        }


    }
}

int verifySpaceInStack(int fila, char side) {
    int canal;

    if (fila == 0 || fila == 1) {
        canal = 0;

    } else if (fila == 2 || fila == 3) {
        canal = 1;
    } else if (fila == 4 || fila == 5) {
        canal = 2;
    }
    if (side == 'r') {
        return getCount_t(channel_Ants[canal].list_Ants_R) + 1 == channel_Ants[canal].countAntsWait;
    } else if (side == 'l') {
        return getCount_t(channel_Ants[canal].list_Ants_L) + 1 == channel_Ants[canal].countAntsWait;
    }


}

void spawnAnt(int fila, enum antType type, char side, Matrix *filas[6]) {

    if (verifySpaceInStack(fila, side)) return;

    if (antCounter < maxAnts) {
        dataItem *hormiga0 = malloc(sizeof(dataItem));
        switch (type) {
            //dataItem  * dataItem ;

            case black:
                ants[antCounter].speed = 1;

                hormiga0->state = 0;
                hormiga0->priority = 30;
                hormiga0->var_SJF = 7;
//                hormiga0->scheduler_Selected = 0; // TODO meter el archivo de configuracion segun el canal
                hormiga0->rms_C = 6;
                hormiga0->rms_P = 18;
                //hormiga0->tid = toledoAnt_1;
                hormiga0->column = 0;
                hormiga0->row = 0;
                //dataItem  * dataInfo ;
                hormiga0->antId = antCounter;
                ants[antCounter].dataItem = *hormiga0;



                // ants[antCounter].speed = 1;
                //ants[antCounter].dataItem = ;
                if (side == 'r') {
                    SDL_QueryTexture(antSprites.blackLeft1, NULL, NULL, &(ants[antCounter].size.w),
                                     &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = blackLeft1;
                    ants[antCounter].size.x = antHill_x + distanceBetweenHills - 20;


                }
                if (side == 'l') {

                    SDL_QueryTexture(antSprites.blackRight1, NULL, NULL, &(ants[antCounter].size.w),
                                     &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = blackRight1;
                    ants[antCounter].size.x = x_start_road - 100;
                    

                }
                break;
            case red:
                ants[antCounter].speed = 2;

                hormiga0->state = 0;
                hormiga0->priority = 20;
                hormiga0->var_SJF = 6;
//                hormiga0->scheduler_Selected = 0; // TODO meter el archivo de configuracion segun el canal
                hormiga0->rms_C = 2;
                hormiga0->rms_P = 9;
                //hormiga0->tid = toledoAnt_1;
                hormiga0->column = 0;
                hormiga0->row = 0;
                //dataItem  * dataInfo ;
                hormiga0->antId = antCounter;
                ants[antCounter].dataItem = *hormiga0;

                if (side == 'r') {

                    SDL_QueryTexture(antSprites.redLeft1, NULL, NULL, &(ants[antCounter].size.w),
                                     &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = redLeft1;
                    ants[antCounter].size.x = antHill_x + distanceBetweenHills - 20;

                }
                if (side == 'l') {

                    SDL_QueryTexture(antSprites.redRight1, NULL, NULL, &(ants[antCounter].size.w),
                                     &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = redRight1;
                    ants[antCounter].size.x = x_start_road - 100;

                }
                break;
            case queen:
                ants[antCounter].speed = 3;

                hormiga0->state = 0;
                hormiga0->priority = 10;
                hormiga0->var_SJF = 4;
//                hormiga0->scheduler_Selected = 0; // TODO meter el archivo de configuracion segun el canal
                hormiga0->rms_C = 1;
                hormiga0->rms_P = 6;
                //hormiga0->tid = toledoAnt_1;
                hormiga0->column = 0;
                hormiga0->row = 0;
                //dataItem  * dataInfo ;
                hormiga0->antId = antCounter;
                ants[antCounter].dataItem = *hormiga0;

                if (side == 'r') {

                    SDL_QueryTexture(antSprites.queenLeft1, NULL, NULL, &(ants[antCounter].size.w),
                                     &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = queenLeft1;
                    ants[antCounter].size.x = antHill_x + distanceBetweenHills - 20;

                }
                if (side == 'l') {

                    SDL_QueryTexture(antSprites.queenRight1, NULL, NULL, &(ants[antCounter].size.w),
                                     &(ants[antCounter].size.h));
                    ants[antCounter].currentSprite = queenRight1;
                    ants[antCounter].size.x = x_start_road - 100;

                }
                break;

        }
        ants[antCounter].size.y = antHill_y + 100;
        ants[antCounter].fila_act = fila;
        ants[antCounter].passingBridge = 0;

//        ants[antCounter].col_dest = STACKMAX;

        ants[antCounter].size.h *= 0.06;
        ants[antCounter].size.w *= 0.03;
        ants[antCounter].side = side;
        ants[antCounter].notSorting = 1;
        ants[antCounter].colitionFlag = 1;

        ants[antCounter].type = type;
        ants[antCounter].antId = antCounter;
        ants[antCounter].sentHome = 0;
        ants[antCounter].destroy = 0;
        ants[antCounter].passedBridge = 0;
        ants[antCounter].sorted = 0;




        if (fila == 0) {
            ants[antCounter].canal = 0;
            //ants[antCounter].dataItem.scheduler_Selected = 0;
            ants[antCounter].dataItem.scheduler_Selected = channel_Ants[ants[antCounter].canal].scheduler_selected;
            append(&channel_Ants[ants[antCounter].canal].list_Ants_L, &ants[antCounter].dataItem);
            postionInitialAnt(*channel_Ants[ants[antCounter].canal].list_Ants_L, antCounter);
        } else if (fila == 1) {
            ants[antCounter].canal = 0;
            ants[antCounter].dataItem.scheduler_Selected = channel_Ants[ants[antCounter].canal].scheduler_selected;
            append(&channel_Ants[ants[antCounter].canal].list_Ants_R, &ants[antCounter].dataItem);
            postionInitialAnt(*channel_Ants[ants[antCounter].canal].list_Ants_R, antCounter);


        } else if (fila == 2) {

            ants[antCounter].canal = 1;

            ants[antCounter].dataItem.scheduler_Selected = channel_Ants[ants[antCounter].canal].scheduler_selected;
            append(&channel_Ants[ants[antCounter].canal].list_Ants_L, &ants[antCounter].dataItem);
            postionInitialAnt(*channel_Ants[ants[antCounter].canal].list_Ants_L, antCounter);

        } else if (fila == 3) {
            ants[antCounter].canal = 1;
            ants[antCounter].dataItem.scheduler_Selected = channel_Ants[ants[antCounter].canal].scheduler_selected;
            append(&channel_Ants[ants[antCounter].canal].list_Ants_R, &ants[antCounter].dataItem);
            postionInitialAnt(*channel_Ants[ants[antCounter].canal].list_Ants_R, antCounter);

        } else if (fila == 4) {
            ants[antCounter].canal = 2;
            ants[antCounter].dataItem.scheduler_Selected = channel_Ants[ants[antCounter].canal].scheduler_selected;
            append(&channel_Ants[ants[antCounter].canal].list_Ants_L, &ants[antCounter].dataItem);
            postionInitialAnt(*channel_Ants[ants[antCounter].canal].list_Ants_L, antCounter);

        } else if (fila == 5) {
            ants[antCounter].canal = 2;
            ants[antCounter].dataItem.scheduler_Selected = channel_Ants[ants[antCounter].canal].scheduler_selected;
            append(&channel_Ants[ants[antCounter].canal].list_Ants_R, &ants[antCounter].dataItem);
            postionInitialAnt(*channel_Ants[ants[antCounter].canal].list_Ants_R, antCounter);
        }

        ants[antCounter].dataItem.scheduler_Selected = channel_Ants[ants[antCounter].canal].scheduler_selected;
        ants[antCounter].scheduler_selected = channel_Ants[ants[antCounter].canal].scheduler_selected;
//        printf("Col: %i \n", ants[antCounter].col_dest);
        ants[antCounter].x_dest = filas[fila][ants[antCounter].col_dest]->x;
        ants[antCounter].y_dest = filas[fila][ants[antCounter].col_dest]->y;


//        if (ants[antCounter].fila_act % 2 == 0 && ants[antCounter].sorted == 0 && channel_Ants[ants[antCounter].canal].list_Ants_L != NULL) {
//            setMovingAnts();
//            printList_t(channel_Ants[ants[antCounter].canal].list_Ants_L);
//            schedulerSort(channel_Ants[ants[antCounter].canal].list_Ants_L);
//            printList_t(channel_Ants[ants[antCounter].canal].list_Ants_L);
//            postionAllAnt(*channel_Ants[ants[antCounter].canal].list_Ants_L, filas);
//
//            ants[antCounter].sorted = 1;
//        } else if (ants[antCounter].fila_act % 2 != 0 && ants[antCounter].sorted == 0 && channel_Ants[ants[antCounter].canal].list_Ants_R != NULL) {
//            setMovingAnts();
//            printList_t(channel_Ants[ants[antCounter].canal].list_Ants_R);
//            schedulerSort(channel_Ants[ants[antCounter].canal].list_Ants_R);
//            printList_t(channel_Ants[ants[antCounter].canal].list_Ants_R);
//            postionAllAnt(*channel_Ants[ants[antCounter].canal].list_Ants_R, filas);
//            ants[antCounter].sorted = 1;
//        }
        int col;
        if (side == 'l') {
            ants[antCounter].col_act = 0;
            col = ants[antCounter].col_act;
        } else if (side == 'r') {

            ants[antCounter].col_act = channel_Ants[ants[antCounter].canal].largoCanal + STACKMAX * 2 - 1;
            col = ants[antCounter].col_act;
        }

        ants[antCounter].finalX = filas[fila][col]->x;
        ants[antCounter].finalY = filas[fila][col]->y;

        CEThread_t * thread1 = malloc (sizeof(CEThread_t));
        //pthread_t thread1;
        struct Params *param;
        param = malloc(sizeof(struct Params));
        param->antId = antCounter;
        param->filas = filas;
        ants[antCounter].tid = thread1;
        ants[antCounter].dataItem.tid = thread1 ;
        ants[antCounter].fila = fila;

//        printf("Canal Scheduler:%i \n",scheduler->canalNumber);
        CEThread_create(thread1, NULL, startAntMotion, param);
        hay_threads = 1;
        CEThread_detach(*thread1); //Auto frees

        antCounter++;
    }
}

void updateNPC(SDL_Renderer *rend, Matrix *filas[6]) {

    //Ants
    int counter;
    for (counter = 0; counter < antCounter; counter++) {



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
        detectIfAntCross(counter, ants[counter].side);

        if (!ants[counter].destroy) SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);


    }
}

