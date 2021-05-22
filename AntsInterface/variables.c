
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include "../Scheduler/LinkedList.h"

# define COLAMAX 5
# define STACKMAX 5
# define MAXANTS 1000
#define WINDOW_WIDTH (1850)
#define WINDOW_HEIGHT (1050)
#define x_start_road (400)
#define y_start_road (100)
#define gap_roads (55)
#define w_horizontal_road (350)

#define w_vertical_road (50)
#define h_horizontal_road (100)
#define h_vertical_road (800)
#define antHill_x (30)
#define antHill_y (400)
#define sizeOfStack (STACKMAX)
#define sizeOfCanal (COLAMAX)
#define regularSpeed (10)
#define distanceBetweenHills (1550)
#define distanceBetweenRoads ( distanceBetweenHills - 1000)
#define vertical_road3_x (antHill_x + distanceBetweenHills - 2 * w_vertical_road - 125)
#define horizontal_road3_x (vertical_road3_x - 2 * w_vertical_road - w_horizontal_road )
#define x_finalH_road (x_start_road + 2 * w_vertical_road + w_horizontal_road)
#define w_canal_road ((horizontal_road3_x + 2 * w_vertical_road) - x_finalH_road)



typedef struct {
    char antName[21];
    int x;
    int y;
} Matrix[COLAMAX];




enum antType {black = 0, red = 1, queen = 2};
enum spritesEnum {blackRight1=0 , redRight1=1, blackLeft1 = 2, redLeft1 = 3, queenRight1 = 4, queenLeft1 = 5};


typedef struct Ant{
    enum antType type;
    enum spritesEnum currentSprite;
    SDL_Rect size;
    int speed;
    char side;
    int x_dest;
    int y_dest;
    int fila_act;
    int col_act;
    int fila_dest;
    int col_dest;
    int waiting;
    int antId;
    int sentHome;
    int inStack;
    int notSorting;
    int passedBridge;
    int passingBridge;
    int finalX;
    int finalY;
    int sorted;
    long canal;
    long scheduler_selected;
    dataItem dataItem;
    int destroy;


}Ant;

struct AntSprites{
    SDL_Texture *blackRight1; //0
    SDL_Texture *blackLeft1; //0
    SDL_Texture *redRight1; //0
    SDL_Texture *redLeft1; //0
    SDL_Texture *queenRight1; //0
    SDL_Texture *queenLeft1; //0
} antSprites;

const int maxAnts = MAXANTS;
struct Ant ants[MAXANTS];
int antCounter = 0;
int w_ant;
int h_ant;
int cellSize;
int colitionsFlag = 0;



typedef struct channelAnts{

    long count_W; //Cuenta cuantos pasan por lado
    long largoCanal; // largoCanal
    long spacesInBridge;
    long timer;  //AL acabarse el timer, cambia de lado
    long sideFlag; // Señala cual lado debe moverse
    long channelNumber;
    long valueRMS;
    long countAntsWait;
    long starFlow;
    long semaforoActive_L;
    long semaforoActive_R;
    int passedAnts;
    long parametroW_Fixed;


    listNode_t *list_Ants_L; //lista izquier
    listNode_t *list_Ants_R; //lista derecha
    long controlFLow;
    long scheduler_selected;
}channelAnts;
channelAnts * channel_Ants;