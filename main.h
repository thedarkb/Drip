#define SH 10 //Screen Height
#define SW 15 //Screen Width
#define TS 64 //Tile Size

#define ENABLESCROLL 0

#define ELIMIT 100
#define FLIMIT 32
#define INVLIMIT 8
#define ENTFRAMES 32
#define TILECOUNT 182 //Set this to the actual number of sprites for best performance.
#define FRAMERATE 30

#define SHEETX 128 //Tile sheet size
#define SHEETY 480

#define BIT(x) (1<<x)

#define ANIMPARSE entSet[i].frame[entSet[i].direction+entSet[i].animation]
#define PRESENTENT entSet[i]


SDL_Window* w = NULL;
SDL_Surface* s = NULL;
SDL_Surface* loader = NULL;
SDL_Renderer* r = NULL;
SDL_Texture* t = NULL;
SDL_Event keyIn;
uint8_t* keyboard = NULL;

SDL_Surface* tileset[TILECOUNT]; 
SDL_Surface* bgLayer = NULL;
SDL_Surface* scrollLayer = NULL;

typedef struct entity {
	unsigned int x;
	unsigned int y;
	unsigned char xSub;
	unsigned char ySub;
	unsigned int behaviourId;
	unsigned char direction;
	unsigned char animation;
	unsigned char collisionClass;
	unsigned char attack;
	uint32_t status;
	uint16_t health;
	unsigned char inventory[INVLIMIT];
	unsigned char frame[FLIMIT];
	unsigned char setframe;
} entity;

typedef struct screen {
	unsigned char* tileLayer;
	unsigned char* collisionLayer;
	unsigned char* entities;
} screen;

unsigned char spawnSlot=1;
unsigned char nspawnSlot=1;

uint16_t sX = 1;
uint16_t sY = 1;
char scroll = 0;

unsigned char animationG=0;

unsigned char cScreen[SW][SH];
unsigned char nScreen[SW][SH];
unsigned char layers[SW*TS][SH*TS];
unsigned char nlayers[SW*TS][SH*TS];


unsigned char refresh = 1;

char mode = 0;

entity entSet[ELIMIT];
entity nentSet[ELIMIT];

SDL_Surface* sf1 = NULL;

void entityInitialise();
void entityReset();
void entitySpawn(entity in);
void nentityReset();
void nentitySpawn(entity in);
void mapLoader(char entities[SW][SH], char collisions[SW][SH]);
SDL_Surface* surfLoader (SDL_Surface* imgIn, unsigned int sizeX, unsigned int sizeY, unsigned char inSize, unsigned char outSize, unsigned char tNum);
unsigned int get_diff (int val1, int val2);
uint32_t lfsr (uint32_t shift);
void setCollision(int iX, int iY, char stat);
void worldgen(uint16_t xPos, uint16_t yPos);
void scrollMap();
void image(SDL_Surface* imgIn, int x, int y, int w, int h);
void bgBlit(SDL_Surface* imgIn, int x, int y, int w, int h);
void bgDraw ();
void flip();
char collisionCheck(int x, int y);
void moveX(entity* movEnt, char amount);
void moveY(entity* movEnt, char amount);
void snapToGrid(entity* movEnt);
void entityLogic();
void loop();