# define MAX 2
#define WINDOW_WIDTH (1850)
#define WINDOW_HEIGHT (1050)
#define x_start_road (400)
#define y_start_road (100)
#define gap_roads (55)
#define w_horizontal_road (400)
#define w_vertical_road (50)
#define h_horizontal_road (100)
#define h_vertical_road (800)
#define antHill_x (30)
#define antHill_y (400)
#define sizeOfCanal (MAX)
#define regularSpeed (10)
#define distanceBetweenHills (1550)
#define distanceBetweenRoads ( distanceBetweenHills - 700)



typedef struct {
    char antName[21];
    int x;
    int y;
} Matrix[MAX];




enum antType {black = 0, red = 1,queen = 2};
enum spritesEnum {blackRight1=0 , redRight1=1, blackLeft1 = 2, redLeft1 = 3, queenRight1 = 4, queenLeft1 = 5};


struct Ant{
    enum antType type;
    enum spritesEnum currentSprite;
    SDL_Rect size;
    int speed;
    char side;
    int x_dest;
    int y_dest;
    int waiting;
    int antId;
    int sentHome;
};

struct AntSprites{
    SDL_Texture *blackRight1; //0
    SDL_Texture *blackLeft1; //0
    SDL_Texture *redRight1; //0
    SDL_Texture *redLeft1; //0
    SDL_Texture *queenRight1; //0
    SDL_Texture *queenLeft1; //0
} antSprites;

const int maxAnts = MAX;
struct Ant ants[MAX];
int antCounter = 0;