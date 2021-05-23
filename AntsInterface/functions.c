#include "../Scheduler/LinkedList.h"
#include "../CEThread/CEThread.h"
bool verifyXColition(int x1, int x2) {
    int distance = abs(x1 - x2);
    if (distance < w_ant) {
        return true;
    } else {
        return false;
    }
}

bool verifyYColition(int y1, int y2) {
    int distance = abs(y1 - y2);

    if (distance < w_ant) {
        return true;
    } else {
        return false;
    }

}


bool colitionDetect(int counter, char movDir, int distanceMoving) {

    int x1;
    int x2;
    int y1;
    int y2;

    for (int i = 0; i < antCounter; i++) {

        if (i == counter) {
            continue;
        }
        if (movDir == 'u') {  //Verificamos si hay alguien arriba
            int safeDistance = h_ant;
            y1 = ants[counter].size.y - distanceMoving - safeDistance;
            y2 = ants[i].size.y;
            x1 = ants[counter].size.x;
            x2 = ants[i].size.x;

        }
        if (movDir == 'd') {  //Verificamos si hay alguien abajo
            int safeDistance = h_ant;
            y1 = ants[counter].size.y + distanceMoving + safeDistance;
            y2 = ants[i].size.y;
            x1 = ants[counter].size.x;
            x2 = ants[i].size.x;

        }
        if (movDir == 'r') {  //Verificamos si hay alguien a la derecha
            int safeDistance = w_ant;
            int nextX = ants[counter].size.x + distanceMoving + safeDistance;
            x1 = nextX;
            x2 = ants[i].size.x;
            y1 = ants[counter].size.y;
            y2 = ants[i].size.y;
        }
        if (movDir == 'l') {  //Verificamos si hay alguien a la izquierda
            int safeDistance = w_ant ;
            x1 = ants[counter].size.x - distanceMoving - safeDistance;
            x2 = ants[i].size.x;
            y1 = ants[counter].size.y;
            y2 = ants[i].size.y;

        }
//        printf("Hormiga numero %i \n", counter);
        if (verifyYColition(y1, y2) && verifyXColition(x1, x2) && !ants[i].destroy) {
//           printf("Hormiga numero colisiona %i %c %i %i %i \n", counter,movDir, ants[counter].finalY,ants[counter].size.y, ants[counter].type);
            return 1;           //No se puede mover porque se translapa con hormiga de arriba
        }
    }
    return 0;
}


void moveInX(int counter, int finalX) {

    int distx = ants[counter].size.x - finalX;
//    printf("MoveInX %i \n", distx);
    if (distx > 0) {
        if (abs(distx) <= 5) {
            if (colitionDetect(counter, 'l', 1) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x -= 1;
            }
        } else if (abs(distx) > 5 && abs(distx) <= 10) {
            if (colitionDetect(counter, 'l', 5) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x -= 5;
            }
        } else if (abs(distx) > 10 && abs(distx) <= 20) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'l', 10) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x -= 10;
            }
        }
        else if (abs(distx) > 20 && abs(distx) <= 30) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'l', 20) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x -= 20;
            }
        }
        else if ( abs(distx) > 30) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'l', regularSpeed * ants[counter].speed) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x -= regularSpeed * ants[counter].speed;
            }
        }
    }
    else if (distx < 0) {
        if (abs(distx) <= 5) {
            if (colitionDetect(counter, 'r', 1) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x += 1;
            }
        } else if (abs(distx) > 5 && abs(distx) <= 10) {
            if (colitionDetect(counter, 'r', 5) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x += 5;
            }

        } else if (abs(distx) > 10 && abs(distx) <= 20) {
            if (colitionDetect(counter, 'r', 10) * ants[counter].colitionFlag) {
                return;
            } else {
                ants[counter].size.x += 10;

            }
        }
        else if (abs(distx) > 20 && abs(distx) <= 30) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'r', 20) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x += 20;
            }
        }
        else if ( abs(distx) > 30) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'r', regularSpeed * ants[counter].speed) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.x += regularSpeed * ants[counter].speed;
            }
        }
    } else {
        return;
    }
}

void moveInY(int counter, int finalY) {

    int disty = ants[counter].size.y - finalY;
    if (disty > 0) {

        if (abs(disty) <= 5) {
            if (colitionDetect(counter, 'u', 1) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y -= 1;
            }
        } else if (abs(disty) > 5 && abs(disty) <= 10) {
            if (colitionDetect(counter, 'u', 5) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y -= 5;
            }
        } else if (abs(disty) > 10 && abs(disty) <= 20) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'u', 10) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y -= 10;
            }
        }
        else if (abs(disty) > 20 && abs(disty) <= 30) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'u', 20) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y -= 20;
            }
        }
        else if ( abs(disty) > 30) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'u', regularSpeed * ants[counter].speed) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y -= regularSpeed * ants[counter].speed;
            }
        }

    }
    if (disty < 0) {

        if (abs(disty) <= 5) {
            if (colitionDetect(counter, 'd', 1) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y += 1;
            }
        } else if (abs(disty) > 5 && abs(disty) <= 10) {
            if (colitionDetect(counter, 'd', 5) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y += 5;
            }
        } else if (abs(disty) > 10 && abs(disty) <= 20) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'd', 10) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y += 10;
            }
        }
        else if (abs(disty) > 20 && abs(disty) <= 30) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'd', 20) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y += 20;
            }
        }
        else if ( abs(disty) > 30) {
//            printf("Se va a mover 10 en X a la izquierda\n");
            if (colitionDetect(counter, 'd', regularSpeed * ants[counter].speed) * ants[counter].colitionFlag) return;
            else {
                ants[counter].size.y += regularSpeed * ants[counter].speed;
            }
        }

    } else {
//        printf("No se logro mover en Y \n");
        return;
    }
}


//void drawAnts(SDL_Renderer *rend, Matrix *filas[6], SDL_Rect blackAnt_r, SDL_Texture *blackAnt_t) {
//    for (int i = 0; i < 6; ++i) {
//        for (int j = 0; j < sizeOfStack * 2 + sizeOfCanal; ++j) {
//            blackAnt_r.x = filas[i][j]->x;
//            blackAnt_r.y = filas[i][j]->y;
//
//            SDL_RenderCopy(rend, blackAnt_t, NULL, &blackAnt_r);
//
//        }
//    }
//}

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

void
initialize_AntPos(int x_start, int y_start, Matrix *f1, Matrix *f2, Matrix *f3, Matrix *f4, Matrix *f5, Matrix *f6) {
    int x1 = x_start + 2 * w_vertical_road;
    int y1 = y_start;
    int sizeOfCell = w_horizontal_road / sizeOfStack;
    long sizeOfCanal0 = channel_Ants[0].largoCanal;
    long sizeOfCanal1 = channel_Ants[1].largoCanal;
    long sizeOfCanal2 = channel_Ants[2].largoCanal;
    int sizeOfCanalCell0 = w_canal_road / sizeOfCanal0;
    int sizeOfCanalCell1 = w_canal_road / sizeOfCanal1;
    int sizeOfCanalCell2 = w_canal_road / sizeOfCanal2;


    Matrix *array[6] = {f1, f2, f3, f4, f5, f6};
    int gabInHorizontal = h_vertical_road / 3 - h_horizontal_road / 3.5;

    int c1_y = y_start + 25;
    int c2_y = c1_y + 2 * gap_roads + gabInHorizontal;
    int c3_y = c2_y + 2 * gap_roads + gabInHorizontal;

    for (int i = 0; i != 6; i++) {
        if (i == 0 || i == 1) {
            int j;
            x1 = x_start + 2 * w_vertical_road;
            for (j = 0; j < sizeOfStack; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }

            for (j; j < sizeOfStack + sizeOfCanal0; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = c1_y;
                x1 += sizeOfCanalCell0;
            }

            x1 = horizontal_road3_x + 2 * w_vertical_road;
            for (int j = sizeOfStack + sizeOfCanal0; j < sizeOfStack * 2 + sizeOfCanal0; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 = x_start + 2 * w_vertical_road;
            y1 += gap_roads;

        }

        if (i == 2) { y1 += gabInHorizontal; }
        if (i == 2 || i == 3) {
            int j;
            for (j = 0; j < sizeOfStack; j++) {

                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            for (j; j < sizeOfStack + sizeOfCanal1; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = c2_y;
                x1 += sizeOfCanalCell1;
            }
            x1 = horizontal_road3_x + 2 * w_vertical_road;
            for (int j = sizeOfStack + sizeOfCanal1; j < sizeOfStack * 2 + sizeOfCanal1; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 = x_start + 2 * w_vertical_road;
            y1 += gap_roads;

        }
        if (i == 4) { y1 += gabInHorizontal; }
        if (i == 4 || i == 5) {
            int j;
            for (j = 0; j < sizeOfStack; j++) {

                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            for (; j < sizeOfStack + sizeOfCanal2; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = c3_y;
                x1 += sizeOfCanalCell2;
            }
            x1 = horizontal_road3_x + 2 * w_vertical_road;
            for (int j = sizeOfStack + sizeOfCanal2; j < sizeOfStack * 2 + sizeOfCanal2; j++) {
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


void drawCanal(SDL_Renderer *rend, SDL_Rect canal_roads) {
    SDL_SetRenderDrawColor(rend, 193, 154, 107, 255);

    canal_roads.w = w_canal_road;
    canal_roads.x = x_finalH_road;
    canal_roads.y = y_start_road + 30;
    canal_roads.h = 50;

    int gabInHorizontal = h_vertical_road / 3 - h_horizontal_road / 3.5;
    int y = y_start_road + 30;
    for (int i = 0; i != 3; i++) {
        if (i == 0) {
            SDL_RenderFillRect(rend, &canal_roads);

        }
        if (i == 1) { canal_roads.y += 2 * gap_roads + gabInHorizontal; }
        if (i == 1) {
            SDL_RenderFillRect(rend, &canal_roads);
        }
        if (i == 2) { canal_roads.y += 2 * gap_roads + gabInHorizontal; }
        if (i == 2) {
            SDL_RenderFillRect(rend, &canal_roads);

        }
    }

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

void drawRoads(SDL_Renderer *rend, SDL_Rect horizontal_roads, SDL_Rect vertical_roads) {
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    horizontal_roads.w = w_horizontal_road;
    horizontal_roads.h = 50;


    vertical_roads.w = 50;
    vertical_roads.h = h_vertical_road;

    drawHorizontalRoads(rend, horizontal_roads, x_start_road, y_start_road);

    //printf("w_horizontal_road: %i", w_horizontal_road);

    int x_startPoint = horizontal_road3_x;


    drawHorizontalRoads(rend, horizontal_roads, x_startPoint, y_start_road);


    drawVerticalRoads(rend, vertical_roads, x_start_road, y_start_road);
    drawVerticalRoads(rend, vertical_roads, vertical_road3_x, y_start_road);

}

void drawCells(SDL_Renderer *rend) {
    drawLines(rend, sizeOfStack, x_start_road, y_start_road);
    int x_startPoint = horizontal_road3_x;
    drawLines(rend, sizeOfStack, x_startPoint, y_start_road);
}

void initAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
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
void drawSignBoard(SDL_Renderer *rend,SDL_Texture *letreroR_t,SDL_Texture *letreroL_t,SDL_Rect letrero1,SDL_Rect letrero2,SDL_Rect letrero3){
    letrero1.w *=0.14;
    letrero1.h *=0.14;

    letrero2.w *=0.14;
    letrero2.h *=0.14;

    letrero3.w *=0.14;
    letrero3.h *=0.14;
    if (channel_Ants[0].sideFlag == 0){
        SDL_RenderCopy(rend, letreroR_t, NULL, &letrero1);
    }
    else if (channel_Ants[0].sideFlag == 1){
        SDL_RenderCopy(rend, letreroL_t, NULL, &letrero1);
    }
    if (channel_Ants[1].sideFlag == 0){
        SDL_RenderCopy(rend, letreroR_t, NULL, &letrero2);
    }
    else if (channel_Ants[1].sideFlag == 1){
        SDL_RenderCopy(rend, letreroL_t, NULL, &letrero2);
    }

    if (channel_Ants[2].sideFlag == 0){
        SDL_RenderCopy(rend, letreroR_t, NULL, &letrero3);
    }
    else if (channel_Ants[2].sideFlag == 1){
        SDL_RenderCopy(rend, letreroL_t, NULL, &letrero3);
    }
//    SDL_RenderCopy(rend, letreroR_t, NULL, &letrero1);
//    SDL_RenderCopy(rend, letreroR_t, NULL, &letrero2);
//    SDL_RenderCopy(rend, letreroR_t, NULL, &letrero3);
}

void drawBackground(SDL_Renderer *rend, SDL_Texture *background_t, SDL_Texture *anthill_t, SDL_Rect antHill1_r,
                    SDL_Rect antHill2_r, SDL_Rect horizontal_roads, SDL_Rect vertical_roads, SDL_Rect mini_roads,
                    SDL_Rect canal_roads,SDL_Texture *letreroR_t,SDL_Texture *letreroL_t, SDL_Rect letrero1,SDL_Rect letrero2,SDL_Rect letrero3) {
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, background_t, NULL, NULL);
    drawHills(rend, anthill_t, antHill1_r, antHill2_r, mini_roads);
    drawRoads(rend, horizontal_roads, vertical_roads);
    drawCanal(rend, canal_roads);

    drawSignBoard(rend,letreroR_t,letreroL_t,letrero1,letrero2,letrero3);

//    SDL_RenderCopy(rend, sprite, NULL, &ants[counter].size);


}

void sendHome(int counter, char side) {

    int entrance = antHill_y + 50;
    int disty = ants[counter].size.y - entrance;  //Distancia en y que le falta a la hormiga para entrar

    if (disty == 0) {  //Si la distancia es igual a cero entonces metemos la hormiga al hormiguero
//        printf("Esta alineado con el hormiguero %i \n",counter);
        int antHillposX;
        if (ants[counter].side == 'r') antHillposX = antHill_x + 100;
        if (ants[counter].side == 'l') antHillposX = vertical_road3_x + 300;
        if (ants[counter].size.x != antHillposX) {
            moveInX(counter, antHillposX);
            return;
        } else{
            ants[counter].destroy = 1;
        };
    }
    int distx;
    if (ants[counter].side == 'r') distx = ants[counter].size.x - x_start_road;
    if (ants[counter].side == 'l') distx = vertical_road3_x - ants[counter].size.x;

    if (distx == 0) {
        moveInY(counter, entrance);
        ants[counter].passingBridge = 0;

        return;
    } else {       //Movemos las hormigas hasta el camino vertical de la izquierda

        int destRoadX;
        if (ants[counter].side == 'r') destRoadX = x_start_road;
        if (ants[counter].side == 'l') destRoadX = vertical_road3_x;
        moveInX(counter, destRoadX);
        return;
    }


}

bool positionInInitialRow(int counter, char side) {

    if (ants[counter].inStack == 1) return true;

    int direction;
    if (side == 'l') direction = 1;
    if (side == 'r') direction = -1;

    int x_position;
    if (side == 'l') x_position = x_start_road + w_vertical_road / 2;
    if (side == 'r') x_position = vertical_road3_x + w_vertical_road / 2;

    if (ants[counter].size.x < x_position && side == 'l') {

        moveInX(counter, x_position);


        return false;
    }
    if (ants[counter].size.x > x_position && side == 'r') {

        moveInX(counter, x_position);


        return false;
    }
    int disty = ants[counter].size.y - ants[counter].y_dest;


    if (disty ==
        0) { //Significa que está posicionado en las carreteras verticales laterales y listo para meterse en las filas
        ants[counter].inStack = 1;
        ants[counter].colitionFlag = 0;
        return true;
    } else {
        moveInY(counter, ants[counter].y_dest);
        return false;
    }
}


void moveAntInStack(int counter, Matrix *filas[6]) {
//    int fila = ;

    int finalX = ants[counter].finalX;
    int finalY = ants[counter].finalY;


    if (ants[counter].col_act == ants[counter].col_dest && ants[counter].size.x == finalX
        && ants[counter].size.y == finalY) {


        ants[counter].waiting = 1;
        ants[counter].dataItem.state = 1;
        return;
    } else {

        if (ants[counter].size.x != finalX
            || ants[counter].size.y != finalY) {

            if (ants[counter].size.x != finalX) {
                //printf("Se va a mover en X %i %i \n", ants[counter].size.x , ants[counter].finalX);
                moveInX(counter, finalX);
                return;
            }
            if (ants[counter].size.y != finalY) {
                //printf("Se va a mover en Y %i %i \n", ants[counter].size.y , ants[counter].finalY);
                moveInY(counter, finalY);
                return;
            }


        }
        if (ants[counter].size.x == finalX
            && ants[counter].size.y == finalY) {
            int colDif = ants[counter].col_dest - ants[counter].col_act;
            if (colDif > 0) {
                ants[counter].col_act += 1;

                ants[counter].finalX = filas[ants[counter].fila_act][ants[counter].col_act]->x;
                ants[counter].finalY = filas[ants[counter].fila_act][ants[counter].col_act]->y;
            } else if (colDif < 0) {
                ants[counter].col_act -= 1;

                ants[counter].finalX = filas[ants[counter].fila_act][ants[counter].col_act]->x;
                ants[counter].finalY = filas[ants[counter].fila_act][ants[counter].col_act]->y;
            }

            return;

        } else {
//            printf("Algo inesperado ocurrio");
        }
    }
}
//TODO implementar  calen
bool detectIfAntCross(int counter, char side) {
    int delCol;

    if (side == 'l') {
        delCol = STACKMAX + channel_Ants[ants[counter].canal].largoCanal;
        if (ants[counter].col_act > delCol) {
            ants[counter].sentHome = 1;
            if (ants[counter].passedBridge == 0){
                channel_Ants[ants[counter].canal].passedAnts++;
                ants[counter].passedBridge = 1;
//                printf("\nEL MAE YA PASO!\n");
                //unblock_threads_from_list_ants(ants[antId_in].fila)

            }
            return true;
        } else {
            return false;
        }
    }
    if (side == 'r') {
        delCol = STACKMAX - 1;
        if (ants[counter].col_act < delCol) {
            ants[counter].sentHome = 1;
            if (ants[counter].passedBridge == 0) {
                channel_Ants[ants[counter].canal].passedAnts++;
                ants[counter].passedBridge = 1;



            }

            return true;
        } else {
            return false;
        }

    }

}




