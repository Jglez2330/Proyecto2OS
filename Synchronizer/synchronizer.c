//
// Created by leahycarlos21 on 13/5/21.
//

#include <malloc.h>
#include "synchronizer.h"

int synchronizerInit() {
    fileValues = malloc(sizeof(pConfig) * 3);
    char pathName[30];

    for (int i = 0; i < channelCount; i++) {
        //CEThread_mutex_init(&mutexes[i],NULL);
        cfg_opt_t opts[] = {
                CFG_SIMPLE_INT("Calendarizador", &fileValues[i].calendarizador),
                CFG_SIMPLE_INT("MetodoControlFlujo", &fileValues[i].metodoControlFlujo),
                CFG_SIMPLE_INT("LargoCanal", &fileValues[i].largoCanal),
                CFG_SIMPLE_INT("CantidadHormigasOrdenadas", &fileValues[i].cantidadHormigasOrdenadas),
                CFG_SIMPLE_INT("TiempoCambioLetrero", &fileValues[i].timeCambioLetrero),
                CFG_SIMPLE_INT("ParametroW", &fileValues[i].parametroW),
                CFG_SIMPLE_INT("TiempoMaximoRMS", &fileValues[i].tiempoMaximoRMS),
                CFG_END()
        };
        cfg_t *cfg;
        cfg = cfg_init(opts, 0);
        snprintf(pathName, 30, "/etc/Project2_SO/Canal%i.conf", i + 1);
        cfg_parse(cfg, pathName);
        cfg_free(cfg);
        //printf("\n CALENDARIZADOR%li", valuesChannel[i].calendarizador);
        //printf("\n%li", valuesChannel[i].tiempoMaximoRMS);

    }
    return 0;
}

int synchronizer_Ants_Init() {
    antsValues = malloc(sizeof(ants_Config));
    char pathName[30];
    cfg_opt_t opts[] = {
            CFG_SIMPLE_INT("BlackAnt_priority", &antsValues->blackAnt_priority),
            CFG_SIMPLE_INT("BlackAnt_var_SJF", &antsValues->blackAnt_var_SJF),
            CFG_SIMPLE_INT("BlackAnt_rms_C", &antsValues->blackAnt_rms_C),
            CFG_SIMPLE_INT("BlackAnt_rms_P", &antsValues->blackAnt_rms_P),

            CFG_SIMPLE_INT("RedAnt_priority", &antsValues->redAnt_priority),
            CFG_SIMPLE_INT("RedAnt_var_SJF", &antsValues->redAnt_var_SJF),
            CFG_SIMPLE_INT("RedAnt_rms_C", &antsValues->redAnt_rms_C),
            CFG_SIMPLE_INT("RedAnt_rms_P", &antsValues->redAnt_rms_P),

            CFG_SIMPLE_INT("QueenAnt_priority", &antsValues->queenAnt_priority),
            CFG_SIMPLE_INT("QueenAnt_var_SJF", &antsValues->queenAnt_var_SJF),
            CFG_SIMPLE_INT("QueenAnt_rms_C", &antsValues->queenAnt_rms_C),
            CFG_SIMPLE_INT("QueenAnt_rms_P", &antsValues->queenAnt_rms_P),

            CFG_END()
    };
    cfg_t *cfg;
    cfg = cfg_init(opts, 0);
    snprintf(pathName, 31, "/etc/Project2_SO/hormigas.conf");
    cfg_parse(cfg, pathName);
    cfg_free(cfg);

    return 0;
}

