//
// Created by leahycarlos21 on 13/5/21.
//

#include "synchronizer.h"

int synchronizerInit(){
    valuesChannel = malloc(sizeof (pConfig)*3);
    char pathName[30];

    for(int i = 0;i<channelCount;i++){
        //CEThread_mutex_init(&mutexes[i],NULL);
        cfg_opt_t opts[] = {
                CFG_SIMPLE_INT("Calendarizador", &valuesChannel[i].calendarizador),
                CFG_SIMPLE_INT("MetodoControlFlujo", &valuesChannel[i].metodoControlFlujo),
                CFG_SIMPLE_INT("LargoCanal", &valuesChannel[i].largoCanal),
                CFG_SIMPLE_INT("CantidadHormigasOrdenadas", &valuesChannel[i].cantidadHormigasOrdenadas),
                CFG_SIMPLE_INT("TiempoCambioLetrero", &valuesChannel[i].timeCambioLetrero),
                CFG_SIMPLE_INT("ParametroW", &valuesChannel[i].parametroW),
                CFG_SIMPLE_INT("TiempoMaximoRMS", &valuesChannel[i].tiempoMaximoRMS),
                CFG_END()
        };
        cfg_t *cfg;
        cfg = cfg_init(opts, 0);
        snprintf(pathName, 30, "/etc/Project2_SO/Canal%i.conf", i+1);
        cfg_parse(cfg, pathName);
        cfg_free(cfg);
        printf("\n CALENDARIZADOR%li", valuesChannel[i].calendarizador);
        printf("\n%li", valuesChannel[i].tiempoMaximoRMS);

    }
    return 0;
}

typedef struct flujoCana{
    int valorW_L;
    int valorW_R;
    int flagLado; // Si es 0 va de Izquierda a Derecha, Si es 1 va de Derecha a Izquierda

}flujoCana;

int flowControl(listNode_t * listIn){

        if (getCount_t(listIn)  == -1){
            return NULL;
        }

        switch (listIn->dataInfo->scheduler_Selected)
        {
            case 0:
                printf("\nRR");  // Apropiativo

                break;

            case 1:
            printf("Prioridad"); // No Apropiativo
               // bubbleSort_t(listResult,PRIORITY);

                break;
            case 2:
                printf("SJF"); // No apropiativo
             //   bubbleSort_t(listResult,SJF);

                break;
            case 3:
                printf("FCFS"); // NO apropiativo
                //listResult = FCFS(listResult);
                break;
            case 4:
                printf("\nTiempo Real"); //No apropiativo
               // tiempoReal_Init(listResult);
                break;

            default:
                printf("Doesnt found");

        }




    }


/*

int equidad_Init(scheduler_t * scheduler){
    if(scheduler->canalNumber==0) {
        scheduler->scheduler_Selected = valuesChannel[0].calendarizador;
        scheduler->flowControl = valuesChannel[0].metodoControlFlujo;
        scheduler->side_Flag = 0;
        scheduler->parameterW_A = valuesChannel[0].parametroW;
        scheduler->parameterW_B = 0;
    }
    else if(scheduler->canalNumber==1) {
        scheduler->scheduler_Selected = valuesChannel[1].calendarizador;
        scheduler->flowControl = valuesChannel[1].metodoControlFlujo;
        scheduler->side_Flag = 0;
        scheduler->parameterW_A = valuesChannel[1].parametroW;
        scheduler->parameterW_B = 0;
    }
    else {
        scheduler->scheduler_Selected = valuesChannel[2].calendarizador;
        scheduler->flowControl = valuesChannel[2].metodoControlFlujo;
        scheduler->side_Flag = 0;
        scheduler->parameterW_A = valuesChannel[2].parametroW;
        scheduler->parameterW_B = 0;
    }
    return 0;
}*/
/*
int letrero_Init(scheduler_t * scheduler) {
    if(scheduler->canalNumber==0) {
        scheduler->timeCambioLetrero = valuesChannel[0].timeCambioLetrero;
        scheduler->side_Flag = 0;

    }
    else if(scheduler->canalNumber==1) {
        scheduler->timeCambioLetrero = valuesChannel[1].timeCambioLetrero;
        scheduler->side_Flag = 0;
    }
    else {
        scheduler->timeCambioLetrero = valuesChannel[2].timeCambioLetrero;
        scheduler->side_Flag = 0;
    }
    return 0;

}

int tico_Init(scheduler_t * scheduler) {
    if(scheduler->canalNumber==0) {
        scheduler->side_Flag = 1;

    }
    else if(scheduler->canalNumber==1) {
        scheduler->side_Flag = 1;
    }
    else {
        scheduler->side_Flag = 1;
    }
    return 0;

}

*/
