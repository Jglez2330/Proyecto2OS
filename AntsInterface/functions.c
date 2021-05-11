







void drawAnts(SDL_Renderer *rend, Matrix *filas[6], SDL_Rect blackAnt_r, SDL_Texture *blackAnt_t){
    for (int i = 0; i < 6; ++i){
        for (int j = 0; j < sizeOfCanal * 2; ++j) {
            blackAnt_r.x = filas[i][j]->x;
            blackAnt_r.y = filas[i][j]->y;

            SDL_RenderCopy(rend, blackAnt_t, NULL, &blackAnt_r);

        }
    }
}
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

void initialize_AntPos(int x_start, int y_start, Matrix *f1,Matrix *f2,Matrix *f3,Matrix *f4,Matrix *f5,Matrix *f6) {

    int x1 = x_start + 2 * w_vertical_road;
    int y1 = y_start;
    int sizeOfCell = w_horizontal_road / sizeOfCanal;

    Matrix *array[6] = {f1,f2,f3,f4,f5,f6};
    int gabInHorizontal = h_vertical_road / 3 - h_horizontal_road / 3.5;

    for (int i = 0; i != 6; i++) {
        if (i == 0 || i == 1) {
            x1 = x_start + 2 * w_vertical_road;
            for (int j = 0; j < sizeOfCanal; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 += 55;
            for (int j = sizeOfCanal; j < sizeOfCanal*2; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 = x_start + 2 * w_vertical_road;
            y1 += gap_roads;

        }

        if (i == 2) { y1 += gabInHorizontal;}
        if (i == 2 || i == 3) {
            for (int j = 0; j < sizeOfCanal; j++) {

                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 += 55;
            for (int j = sizeOfCanal; j < sizeOfCanal*2; j++) {
                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 = x_start + 2 * w_vertical_road;
            y1 += gap_roads;

        }
        if (i == 4) { y1 += gabInHorizontal; }
        if (i == 4 || i == 5) {
            for (int j = 0; j < sizeOfCanal; j++) {

                array[i][j]->x = x1;
                array[i][j]->y = y1;
                x1 += sizeOfCell;
            }
            x1 += 55;
            for (int j = sizeOfCanal; j < sizeOfCanal*2; j++) {
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
    horizontal_roads.w = distanceBetweenRoads / 2 - gap_roads / 2;
    horizontal_roads.h = 50;


    vertical_roads.w = 50;
    vertical_roads.h = h_vertical_road;

    drawHorizontalRoads(rend, horizontal_roads, x_start_road, y_start_road);


    int x_startPoint = x_start_road + distanceBetweenRoads / 2 + gap_roads / 2;


    drawHorizontalRoads(rend, horizontal_roads, x_startPoint, y_start_road);


    drawVerticalRoads(rend, vertical_roads, x_start_road, y_start_road);
    drawVerticalRoads(rend, vertical_roads, x_start_road + distanceBetweenRoads + w_vertical_road * 2, y_start_road);

}
void drawCells(SDL_Renderer *rend) {
    drawLines(rend, sizeOfCanal, x_start_road, y_start_road);
    int x_startPoint = x_start_road + distanceBetweenRoads / 2 + gap_roads / 2;
    drawLines(rend, sizeOfCanal, x_startPoint, y_start_road);
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
void drawBackground(SDL_Renderer *rend, SDL_Texture *background_t, SDL_Texture *anthill_t, SDL_Rect antHill1_r,
                    SDL_Rect antHill2_r, SDL_Rect horizontal_roads, SDL_Rect vertical_roads, SDL_Rect mini_roads) {
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, background_t, NULL, NULL);
    drawHills(rend, anthill_t, antHill1_r, antHill2_r, mini_roads);
    drawRoads(rend, horizontal_roads, vertical_roads);
}

