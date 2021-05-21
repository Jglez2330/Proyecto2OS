//
// Created by leahycarlos21 on 13/5/21.
//

#ifndef PROYECTO2OS_SYNCHRONIZER_H
#define PROYECTO2OS_SYNCHRONIZER_H

#include "confuse.h"
#define channelCount 3

typedef struct pConfig {
    long calendarizador;
    long metodoControlFlujo;
    long largoCanal;
    long cantidadHormigasOrdenadas;
    long timeCambioLetrero;
    long parametroW;
    long tiempoMaximoRMS;
}pConfig;

pConfig * fileValues;


int synchronizerInit();/*
int equidad_Init(scheduler_t * scheduler);
int letrero_Init(scheduler_t * scheduler);
int tico_Init(scheduler_t * scheduler);*/
#endif //PROYECTO2OS_SYNCHRONIZER_H
