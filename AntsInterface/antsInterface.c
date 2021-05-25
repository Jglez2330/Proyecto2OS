
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../Scheduler/Scheduler.h"
#include "../CEThread/CEThread.h"
#include "../Synchronizer/synchronizer.h"
#include <unistd.h>
#include <time.h>
//#include "../Scheduler/LinkedList.h"


int alpha = 0;
int hay_threads;
#define UP 2
#define LOW 1
static struct sigaction letrero_alarm[3];
static sigset_t timer_signal[3];

int type_channel[3] = {0,0,0};
long default_time[3] = {0,0,0};
int ready_to_change_channel[3] = {0,0,0};
time_t timer[3];
struct Params {

    int antId;
    Matrix **filas;
};
CEThread_mutex_t mutex;
void init_alarm(long time_var, int channel, long type){
    if (type == 1){
        default_time[channel] = time_var;
        timer[channel] = time(NULL);
        type_channel[channel] = channel;
    }else if(type == 2){
        default_time[channel] = (rand() % (UP - LOW + 1)) + LOW;
        timer[channel] = time(NULL);
        type_channel[channel] = channel;
    }

}

int check_timer(int channel){
    time_t now = time(NULL);
    time_t seconds = now - timer[channel];

    if (seconds > default_time[channel]){
        timer[channel] = time(NULL);
        ready_to_change_channel[channel] = 1;
    }
    if (type_channel[channel] == 2){
        default_time[channel] = (rand() % (UP - LOW + 1)) + LOW;
    }
    return ready_to_change_channel[channel];
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
    int listSize = getCount_t(&list) + 1;
    for (int i = 0; i < listSize; i++) {
        for (int j = 0; j < antCounter; j++) {
            if (getNode_t(&list, i)->antId == ants[j].antId) {
                if (ants[j].side == 'l') {
                    ants[j].col_dest = (STACKMAX - 1) - i;


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

bool letrero_ant_flow(int antId_in, Matrix **filas) {

    if (ants[antId_in].passedBridge == 1) {
        channel_Ants[ants[antId_in].canal].spacesInBridge += 1;
        ants[antId_in].passedBridge = 2;
        unblock_threads_from_list_ants(ants[antId_in].fila);    //Desbloquea los de atras
    }
    if (ants[antId_in].sentHome) {
        sendHome(antId_in, ants[antId_in].side);
        return 1;
    }

    int hormigasEsperando = countAntsWaiting(ants[antId_in].canal, ants[antId_in].side);

    int filaLlena = channel_Ants[ants[antId_in].canal].countAntsWait <= hormigasEsperando;

    if (filaLlena) { //Si se alcanzo la cantidad de hormigas esperando necesarias



        int filaVaciaL = getCount_t(channel_Ants[ants[antId_in].canal].list_Ants_L) == -1;
        if(filaVaciaL){
            channel_Ants[ants[antId_in].canal].sideFlag = 1;
        }
        int filaVaciaR = getCount_t(channel_Ants[ants[antId_in].canal].list_Ants_R) == -1;
        if(filaVaciaR){
            channel_Ants[ants[antId_in].canal].sideFlag = 0;
        }

        if (ants[antId_in].side == 'r' && channel_Ants[ants[antId_in].canal].sideFlag == 1) {

            channel_Ants[ants[antId_in].canal].semaforoActive_R = 1;
            channel_Ants[ants[antId_in].canal].semaforoActive_L = 0;

        }
        if (ants[antId_in].side == 'l' && channel_Ants[ants[antId_in].canal].sideFlag == 0) {
            channel_Ants[ants[antId_in].canal].semaforoActive_L = 1;
            channel_Ants[ants[antId_in].canal].semaforoActive_R = 0;

        }

    }
    int hormigasEnPipe = channel_Ants[ants[antId_in].canal].largoCanal - channel_Ants[ants[antId_in].canal].spacesInBridge;
    bool noHayHormigas =  hormigasEnPipe == 0;
    //No haya horgimas en el pipe && el tiempo expiro
    if (check_timer(ants[antId_in].canal)&&noHayHormigas) {
        channel_Ants[ants[antId_in].canal].passedAnts = 0;
        ready_to_change_channel[ants[antId_in].canal] = 0;
        channel_Ants[ants[antId_in].canal].count_W = channel_Ants[ants[antId_in].canal].parametroW_Fixed;
        if (channel_Ants[ants[antId_in].canal].sideFlag == 1){
            unblock_all_threads_ants(ants[antId_in].canal * 2 + 1);
            channel_Ants[ants[antId_in].canal].sideFlag = 0;
            //Colocar el cambio de bandera
        }
        else if (channel_Ants[ants[antId_in].canal].sideFlag == 0){
            unblock_all_threads_ants(ants[antId_in].canal * 2);
            channel_Ants[ants[antId_in].canal].sideFlag = 1;
        }

    }

    if (channel_Ants[ants[antId_in].canal].semaforoActive_L == 1
        && channel_Ants[ants[antId_in].canal].spacesInBridge != 0
        && channel_Ants[ants[antId_in].canal].list_Ants_L != NULL &&
        ants[antId_in].fila % 2 == 0 &&
        channel_Ants[ants[antId_in].canal].list_Ants_L->dataInfo != NULL
        && channel_Ants[ants[antId_in].canal].count_W != 0) {
        if (channel_Ants[ants[antId_in].canal].count_W == channel_Ants[ants[antId_in].canal].parametroW_Fixed) {
            listNode_t* temp_ants_list = copyList(channel_Ants[ants[antId_in].canal].list_Ants_L);
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
        }

        int id = getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_L, 0)->antId;
        ants[id].passingBridge = 1;




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
        && channel_Ants[ants[antId_in].canal].spacesInBridge != 0
        && channel_Ants[ants[antId_in].canal].list_Ants_R != NULL &&
        ants[antId_in].fila % 2 == 1 &&
        channel_Ants[ants[antId_in].canal].list_Ants_R->dataInfo != NULL
        && channel_Ants[ants[antId_in].canal].count_W != 0) {
        if (channel_Ants[ants[antId_in].canal].count_W == channel_Ants[ants[antId_in].canal].parametroW_Fixed) {
            listNode_t* temp_ants_list = copyList(channel_Ants[ants[antId_in].canal].list_Ants_R);


            init_scheduler(temp_ants_list, channel_Ants[ants[antId_in].canal].parametroW_Fixed,channel_Ants[ants[antId_in].canal].scheduler_selected,ants[antId_in].fila);
            block_threads_from_list(ants[antId_in].fila);
            unblock_threads_from_list_ants(ants[antId_in].fila);




        }
        channel_Ants[ants[antId_in].canal].count_W--;

        if (channel_Ants[ants[antId_in].canal].count_W == 0) {
            channel_Ants[ants[antId_in].canal].semaforoActive_R = 0;

            channel_Ants[ants[antId_in].canal].sideFlag = 0;
            channel_Ants[ants[antId_in].canal].semaforoActive_L = 0;
        }
        int id;
        if (getFront_t(channel_Ants[ants[antId_in].canal].list_Ants_R) != NULL) {
            id = getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_R, 0)->antId;
            ants[id].passingBridge = 1;

        }


        deleteNodePosition(&channel_Ants[ants[antId_in].canal].list_Ants_R, 0);
        if (getFront_t(channel_Ants[ants[antId_in].canal].list_Ants_R) == NULL) {
            channel_Ants[ants[antId_in].canal].list_Ants_R = NULL;
        }


        if (channel_Ants[ants[antId_in].canal].list_Ants_R != NULL) {
            channel_Ants[ants[antId_in].canal].spacesInBridge -= 1;
            setMovingAnts();
            schedulerSort(channel_Ants[ants[antId_in].canal].list_Ants_R);
            postionAllAnt(*channel_Ants[ants[antId_in].canal].list_Ants_R, filas);

        }
        crossAnt(id);
    }


}

bool equidad_ant_flow(int antId_in, Matrix **filas) {

    if (ants[antId_in].passedBridge == 1) {


        channel_Ants[ants[antId_in].canal].spacesInBridge += 1;
        ants[antId_in].passedBridge = 2;

        unblock_threads_from_list_ants(ants[antId_in].fila);

    }
    if (ants[antId_in].sentHome) {
        sendHome(antId_in, ants[antId_in].side);
        return 1;
        //continue;
    }

    int hormigasEspearando = countAntsWaiting(ants[antId_in].canal, ants[antId_in].side);
    if (channel_Ants[ants[antId_in].canal].countAntsWait <= hormigasEspearando) {
        if(getCount_t(channel_Ants[ants[antId_in].canal].list_Ants_L) == -1){
            channel_Ants[ants[antId_in].canal].sideFlag = 1;
        }
        if(getCount_t(channel_Ants[ants[antId_in].canal].list_Ants_R) == -1){
            channel_Ants[ants[antId_in].canal].sideFlag = 0;
        }

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
            int id = getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_L,0)->antId;
            unblock_all_threads_ants(ants[id].fila);
        }
        if (channel_Ants[ants[antId_in].canal].sideFlag == 0){
            int id  =getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_R,0)->antId;
            unblock_all_threads_ants(ants[id].fila);
        }

    }

    if (channel_Ants[ants[antId_in].canal].semaforoActive_L == 1
        && channel_Ants[ants[antId_in].canal].spacesInBridge != 0
        && channel_Ants[ants[antId_in].canal].list_Ants_L != NULL &&
        ants[antId_in].fila % 2 == 0 &&
        channel_Ants[ants[antId_in].canal].list_Ants_L->dataInfo != NULL
        && channel_Ants[ants[antId_in].canal].count_W != 0) {
        if (channel_Ants[ants[antId_in].canal].count_W == channel_Ants[ants[antId_in].canal].parametroW_Fixed) {
            listNode_t* temp_ants_list = copyList(channel_Ants[ants[antId_in].canal].list_Ants_L);
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
        }

        int id = getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_L, 0)->antId;
        ants[id].passingBridge = 1;




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
        && channel_Ants[ants[antId_in].canal].spacesInBridge != 0
        && channel_Ants[ants[antId_in].canal].list_Ants_R != NULL &&
        ants[antId_in].fila % 2 == 1 &&
        channel_Ants[ants[antId_in].canal].list_Ants_R->dataInfo != NULL
        && channel_Ants[ants[antId_in].canal].count_W != 0) {
        if (channel_Ants[ants[antId_in].canal].count_W == channel_Ants[ants[antId_in].canal].parametroW_Fixed) {
            listNode_t* temp_ants_list = copyList(channel_Ants[ants[antId_in].canal].list_Ants_R);


            init_scheduler(temp_ants_list, channel_Ants[ants[antId_in].canal].parametroW_Fixed,channel_Ants[ants[antId_in].canal].scheduler_selected,ants[antId_in].fila);
            block_threads_from_list(ants[antId_in].fila);
            unblock_threads_from_list_ants(ants[antId_in].fila);




        }
        channel_Ants[ants[antId_in].canal].count_W--;

        if (channel_Ants[ants[antId_in].canal].count_W == 0) {
            channel_Ants[ants[antId_in].canal].semaforoActive_R = 0;

            channel_Ants[ants[antId_in].canal].sideFlag = 0;
            channel_Ants[ants[antId_in].canal].semaforoActive_L = 0;
        }

        int id = getNode_t(channel_Ants[ants[antId_in].canal].list_Ants_R, 0)->antId;
        ants[id].passingBridge = 1;




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
        crossAnt(id);
    }


}

void *startAntMotion(void *params) {
    struct Params *p = params;
    if (alpha++ == 0){
        CEThread_mutex_init(&mutex);
    }
    bool continueFlag;
    while (1) {

        CEThread_yield();

        if (ants[p->antId].destroy) {
            return NULL;
        }
        if(channel_Ants[ants[p->antId].canal].controlFLow == 0) {
            continueFlag = equidad_ant_flow(p->antId, p->filas);
            if (continueFlag) {
                continue;
            }
        }
        if(channel_Ants[ants[p->antId].canal].controlFLow == 1) {
            continueFlag = letrero_ant_flow(p->antId, p->filas);
            if (continueFlag) {
                continue;
            }
        }

        if(channel_Ants[ants[p->antId].canal].controlFLow == 2) {
            continueFlag = letrero_ant_flow(p->antId, p->filas);
            if (continueFlag) {
                continue;
            }
        }


        if (positionInInitialRow(p->antId, ants[p->antId].side)) {

            if (ants[p->antId].fila_act % 2 == 0 && ants[p->antId].sorted == 0 &&
                channel_Ants[ants[p->antId].canal].list_Ants_L != NULL) {
                setMovingAnts();
                schedulerSort(channel_Ants[ants[p->antId].canal].list_Ants_L);
                postionAllAnt(*channel_Ants[ants[p->antId].canal].list_Ants_L, p->filas);

                ants[p->antId].sorted = 1;
            }
            if (ants[p->antId].fila_act % 2 != 0 && ants[p->antId].sorted == 0 &&
                channel_Ants[ants[p->antId].canal].list_Ants_R != NULL) {
                setMovingAnts();
                schedulerSort(channel_Ants[ants[p->antId].canal].list_Ants_R);
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
    for (int i = 0; i < listSize; i++) {
        if (getNode_t(&list, i)->antId == antCount) {
            if (ants[antCount].side == 'l') {
                ants[antCount].col_dest = (STACKMAX - 1) - i;
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
bool verifyRealTime(char side,int fila, dataItem *hormiga0){
    int canal;

    if (fila == 0 || fila == 1) {
        canal = 0;

    } else if (fila == 2 || fila == 3) {
        canal = 1;
    } else if (fila == 4 || fila == 5) {
        canal = 2;
    }
    if(side == 'r'){
        if ( !tiempoReal_Check(channel_Ants[canal].list_Ants_R,hormiga0)) return 1;
        else return 0;
    }
    else if(side == 'l'){
        if ( !tiempoReal_Check(channel_Ants[canal].list_Ants_L,hormiga0)) return 1;
        else return 0;
    }
}
int getCanalWithRow(int fila){
    int canal;

    if (fila == 0 || fila == 1) {
        canal = 0;

    } else if (fila == 2 || fila == 3) {
        canal = 1;
    } else if (fila == 4 || fila == 5) {
        canal = 2;
    }
    return canal;
}
void spawnAnt(int fila, enum antType type, char side, Matrix *filas[6]) {

    if (verifySpaceInStack(fila, side)){
        printf("No hay espacio para ingresar la hormiga\n");
        return;
    }


    if (antCounter < maxAnts) {
        dataItem *hormiga0 = malloc(sizeof(dataItem));
        switch (type) {
            //dataItem  * dataItem ;

            case black:
                ants[antCounter].speed = 1;

                hormiga0->state = 0;
                hormiga0->column = 0;
                hormiga0->row = 0;
                hormiga0->priority = (int)antsValues->blackAnt_priority;
                hormiga0->var_SJF = (int)antsValues->blackAnt_var_SJF;
                hormiga0->rms_C = (float)antsValues->blackAnt_rms_C;
                hormiga0->rms_P = (float)antsValues->blackAnt_rms_P;


                hormiga0->antId = antCounter;
                ants[antCounter].dataItem = *hormiga0;



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
                hormiga0->priority = (int)antsValues->redAnt_priority;
                hormiga0->var_SJF = (int)antsValues->redAnt_var_SJF;
                hormiga0->rms_C = (float)antsValues->redAnt_rms_C;
                hormiga0->rms_P = (float)antsValues->redAnt_rms_P;
                hormiga0->column = 0;
                hormiga0->row = 0;
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

                hormiga0->priority = (int)antsValues->queenAnt_priority;
                hormiga0->var_SJF = (int)antsValues->queenAnt_var_SJF;
                hormiga0->rms_C = (float)antsValues->queenAnt_rms_C;
                hormiga0->rms_P = (float)antsValues->queenAnt_rms_P;
                hormiga0->state = 0;
                hormiga0->column = 0;
                hormiga0->row = 0;
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


        if(verifyRealTime(side,fila,hormiga0) && channel_Ants[getCanalWithRow(fila)].scheduler_selected == 4) return;

        if (fila == 0) {
            ants[antCounter].canal = 0;
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
        ants[antCounter].fila = fila;




        CEThread_t * thread1 = malloc (sizeof(CEThread_t));
        //pthread_t thread1;
        struct Params *param;
        param = malloc(sizeof(struct Params));
        param->antId = antCounter;
        param->filas = filas;
        ants[antCounter].tid = thread1;
        ants[antCounter].dataItem.tid = thread1 ;


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

