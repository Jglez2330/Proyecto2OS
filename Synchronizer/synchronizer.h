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

typedef struct ants_Config {
    long blackAnt_priority;
    long blackAnt_var_SJF;
    long blackAnt_rms_C;
    long blackAnt_rms_P;

    long redAnt_priority;
    long redAnt_var_SJF;
    long redAnt_rms_C;
    long redAnt_rms_P;

    long queenAnt_priority;
    long queenAnt_var_SJF;
    long queenAnt_rms_C;
    long queenAnt_rms_P;

}ants_Config;

ants_Config * antsValues;



pConfig * fileValues;

int synchronizer_Ants_Init();
int synchronizerInit();
#endif //PROYECTO2OS_SYNCHRONIZER_H
