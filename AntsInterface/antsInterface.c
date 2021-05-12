
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


int initializeNPC(SDL_Renderer* rend, SDL_Window *win){

    //Cargar imagenes
    SDL_Surface *surface_BlackAntR = IMG_Load("../Resources/blackAntR.png");
    SDL_Surface *surface_RedAntR = IMG_Load("../Resources/redAntR.png");
    SDL_Surface *surface_QueenAntR = IMG_Load("../Resources/queenAntR.png");
    SDL_Surface *surface_BlackAntL = IMG_Load("../Resources/blackAntL.png");
    SDL_Surface *surface_RedAntL = IMG_Load("../Resources/redAntL.png");
    SDL_Surface *surface_QueenAntL = IMG_Load("../Resources/queenAntR.png");

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

void spawnAnt(int fila, int columna, enum antType type, char side, Matrix *filas[6]){
    if(antCounter < maxAnts) {
        switch(type){
            case black:
                ants[antCounter].speed = 1;
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


        ants[antCounter].size.h *= 0.06;
        ants[antCounter].size.w *= 0.03;
        ants[antCounter].side = side;
        ants[antCounter].x_dest = filas[fila][columna]->x;
        ants[antCounter].y_dest = filas[fila][columna]->y;
        ants[antCounter].type = type;
        ants[antCounter].antId = antCounter;
        ants[antCounter].sentHome = 0;

        antCounter++;
    }
}

void updateNPC(SDL_Renderer *rend) {

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
        if(ants[counter].side == 'r'){
            if (ants[counter].sentHome){
                int entrance = antHill_y + 100;
                int disty = ants[counter].size.y - entrance;
                printf("Disty: %i\n",disty);
                if(disty == 0) {

                    if (ants[counter].size.x > antHill_x - 100) {
                        ants[counter].size.x -= regularSpeed * ants[counter].speed;
                        SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                        continue;
                    }
                    if (ants[counter].size.x <= antHill_x - 25 ) continue;
                }
                if(ants[counter].size.x > x_start_road) {       //Movemos las hormigas hasta el camino vertical de la izquierda

                    int distx = ants[counter].size.x - x_start_road;
                    if(abs(distx) <= 10){
                        ants[counter].size.x -= 1;
                    }
                    if(abs(distx) > 10){
                        ants[counter].size.x -= regularSpeed * ants[counter].speed;
                    }

                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }

                if(ants[counter].size.y < entrance) {

                    if (abs(disty) <= 10) ants[counter].size.y += 1;
                    if (abs(disty) > 10) ants[counter].size.y += regularSpeed * ants[counter].speed;

                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }
                if(ants[counter].size.y > entrance) {

                    if (abs(disty) <= 10) ants[counter].size.y -= 1;
                    if (abs(disty) > 10) ants[counter].size.y -= regularSpeed * ants[counter].speed;

                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }


            }

            if (ants[counter].waiting){
                SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                continue;
            }
            int secondVerticalRoad_x = vertical_road3_x + 30;

            if(ants[counter].size.x >= secondVerticalRoad_x) {
                ants[counter].size.x -= regularSpeed * ants[counter].speed;
                SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                continue;
            }
            int disty = ants[counter].size.y - ants[counter].y_dest;
            if(disty > 0){
                if(abs(disty) < 10) ants[counter].size.y -= 1;
                else ants[counter].size.y -= regularSpeed * ants[counter].speed;

            }
            if(disty < 0){
                if(abs(disty) < 10) ants[counter].size.y += 1;
                else ants[counter].size.y += regularSpeed * ants[counter].speed;

            }
            int distx = ants[counter].size.x - ants[counter].x_dest;


            if (disty == 0){
                if (distx == 0){
                    ants[counter].waiting = 1;
                }

                if(distx > 0){
                    if(abs(distx) < 10) ants[counter].size.x -= 1;
                    if(abs(distx) > 10) ants[counter].size.x -= regularSpeed * ants[counter].speed;
                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }
                if(distx < 0){
                    if(abs(distx) < 10) ants[counter].size.x += 1;
                    if(abs(distx) > 10) ants[counter].size.x += regularSpeed * ants[counter].speed;
                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }
            }




            SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);

        }
        if(ants[counter].side == 'l'){
            if (ants[counter].sentHome){
                int entrance = antHill_y + 100;
                int disty = ants[counter].size.y - entrance;
                int secondRoadLeft = vertical_road3_x;
                if(disty == 0) { //Aca metemos a la hormiga en el hormiguero

                    if (ants[counter].size.x < secondRoadLeft + 300) { //Metemos la hormiga en el hormiguero derecho
                        ants[counter].size.x += regularSpeed * ants[counter].speed;
                        SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                        continue;
                    }
                    if (ants[counter].size.x <= secondRoadLeft + 300) continue; //Si ya sobrepaso el hormiguero entonces desaparecer la hormiga
                }
                if(ants[counter].size.x < secondRoadLeft) {       //Movemos las hormigas hasta el camino vertical de la derecha

                    int distx = x_start_road -  ants[counter].size.x;
                    if(abs(distx) <= 10){
                        ants[counter].size.x += 1;
                    }
                    if(abs(distx) > 10){
                        ants[counter].size.x += regularSpeed * ants[counter].speed;
                    }

                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }

                if(ants[counter].size.y < entrance) {

                    if (abs(disty) <= 10) ants[counter].size.y += 1;
                    if (abs(disty) > 10) ants[counter].size.y += regularSpeed * ants[counter].speed;

                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }
                if(ants[counter].size.y > entrance) {

                    if (abs(disty) <= 10) ants[counter].size.y -= 1;
                    if (abs(disty) > 10) ants[counter].size.y -= regularSpeed * ants[counter].speed;

                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }


            }




            if (ants[counter].waiting){
                SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                continue;
            }


            if(ants[counter].size.x <= x_start_road) {
                ants[counter].size.x += regularSpeed * ants[counter].speed;
                SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                continue;
            }
            int disty = ants[counter].size.y - ants[counter].y_dest;
            if(disty > 0){
                if(abs(disty) < 10) ants[counter].size.y -= 1;
                else ants[counter].size.y -= regularSpeed * ants[counter].speed;
//                SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
//                return;
            }
            if(disty < 0){
                if(abs(disty) < 10) ants[counter].size.y += 1;
                else ants[counter].size.y += regularSpeed * ants[counter].speed;
//                SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
//                return;
            }
            int distx = ants[counter].size.x - ants[counter].x_dest;
//            SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);

            if (disty == 0){
                if (distx == 0){
                    ants[counter].waiting = 1;
                }
                ants[counter].size.x += regularSpeed * ants[counter].speed;
                if(distx > 0){
                    if(abs(distx) < 10) ants[counter].size.x -= 1;
                    if(abs(distx) > 10) ants[counter].size.x -= regularSpeed * ants[counter].speed;
                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }
                if(distx < 0){
                    if(abs(distx) < 10) ants[counter].size.x += 1;
                    if(abs(distx) > 10) ants[counter].size.x += regularSpeed * ants[counter].speed;
                    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
                    continue;
                }
            }




            SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
        }


        //Render to screen

    }
}
void sendHome (){
    for(int counter = 0; counter < antCounter; counter++) {
        ants[counter].sentHome = 1;
    }
}