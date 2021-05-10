
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"




int initializeNPC(SDL_Renderer* rend, SDL_Window *win){

    //Cargar imagenes
    SDL_Surface *surface_BlackAnt = IMG_Load("../Resources/blackAnt.png");
    SDL_Surface *surface_RedAnt = IMG_Load("../Resources/redAnt.png");
    //Manejo de error al cargar
    if (
            !surface_BlackAnt || !surface_RedAnt

            ) {
        printf("Error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    //Crear texturas desde la imagen
    antSprites.blackRight1 = SDL_CreateTextureFromSurface(rend, surface_BlackAnt);
    SDL_FreeSurface(surface_BlackAnt);
    antSprites.redRight1 = SDL_CreateTextureFromSurface(rend, surface_RedAnt);
    SDL_FreeSurface(surface_RedAnt);

    //Manejo de errores al crear las texturas
    if(
            !antSprites.blackRight1 || !antSprites.redRight1
            ){
        printf("Error creating texture: %s\n",SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
}

void spawnAnt(int fila, int columna, enum antType type, char side){
    if(antCounter < maxAnts) {
        switch(type){
            case black:
                printf("Creando hormiga negra");

                SDL_QueryTexture(antSprites.blackRight1, NULL, NULL,&(ants[antCounter].size.w), &(ants[antCounter].size.h));
                ants[antCounter].currentSprite = blackRight1;
                break;
            case red:
                printf("Creando hormiga roja");
                SDL_QueryTexture(antSprites.redRight1, NULL, NULL, &(ants[antCounter].size.w), &(ants[antCounter].size.h));
                ants[antCounter].currentSprite = redRight1;
                break;

        }

        ants[antCounter].size.x = x_start_road - 90;
        ants[antCounter].size.y = antHill_y + 50;
        ants[antCounter].size.h *= 0.06;
        ants[antCounter].size.w *= 0.03;
        ants[antCounter].side = side;


        ants[antCounter].type = type;

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
                }
                break;
            case red:
                //Movimiento

                //Animacion
                switch (ants[counter].currentSprite) {
                    case redRight1:
                        sprite = antSprites.redRight1;
                        break;
                }
                break;
        }
        if(ants[counter].side == 'r'){
            if(ants[counter].size.x <= x_start_road) ants[counter].size.x += 10;
        }
        if(ants[counter].side == 'l'){
            if(ants[counter].size.x <= x_start_road) ants[counter].size.x += 10;
        }


        //Render to screen
        SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);
    }
}