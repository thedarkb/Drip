#ifdef WEB
#include <emscripten.h>
#endif
#include <SDL2/SDL_image.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <tcl/tcl.h>

#define SH 10 //Screen Height
#define SW 15 //Screen Width
#define TS 16 //Tile Size

#define WORLDWIDTH 700 //The size of the editable (not necessarily visible) world.
#define WORLDHEIGHT 605 //The world is, in practice, larger due to the presence of a procedurally generated dungeon.
#define SEED 420

#define WORLDFILENAME "world.map"

#define RESX 180
#define RESY 120

#define ELIMIT 512 //Entity limit must not exceed 256
#define MAPELIMIT 16
#define FLIMIT 32
#define ENTVARIETY 1
#define FACTIONLIMIT 2
#define TLIMIT 255
#define SPRITELAYERS 3
#define INVLIMIT 8
#define ENTFRAMES 32
#define TILECOUNT 239 //Set this to the actual number of sprites for best performance.
#define FRAMERATE 60
#define HUDHEIGHT 20
#define MSGDEPTH 32
#define MSGTIME 30

#define SHEETX 128 //Tile sheet size
#define SHEETY 480

#define BIT(x) (1<<x)

#define ANIMPARSE entSet[i].frame[entSet[i].direction+entSet[i].animation]
#define THIS entSet[i]
							
#define DIST(x1,y1,x2,y2) ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))
#define PAIR(x,y) (((x)<<16)|(y)) //This is very dirty and only designed for use with uint16_t sX/sY pairs.
#define CX RESX/2-cameraX-TS/2
#define CY RESY/2+HUDHEIGHT-cameraY-TS/2
#define TITLE "Drip"


SDL_Window* w = NULL;
SDL_Surface* s = NULL;
SDL_Surface* lOverlay=NULL;
SDL_Surface* loader = NULL;
SDL_Renderer* r = NULL;
SDL_Texture* t = NULL;
SDL_Texture* screenBackup=NULL;
SDL_Rect hudStripper = {0,HUDHEIGHT, SW*TS, SH*TS};
SDL_Event keyIn;
const uint8_t* keyboard = NULL;

Tcl_Interp* gameState=NULL;

const SDL_Rect clipRect={
	0,
	0,
	RESX,
	RESY+HUDHEIGHT
};


SDL_Surface* swtileset[TILECOUNT]; 
SDL_Texture* hwtileset[TILECOUNT];
SDL_Surface* font[128];
SDL_Texture* hwfont[128];

enum direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
} direction;

typedef struct entity entity;

typedef struct entity {
	char state[512];
} entity;

typedef struct item {
	unsigned int sprite;
	void(*behaviour)();
	void* state;
} item;

typedef struct inventory {
	item* items[INVLIMIT];
	unsigned char selection;
	unsigned char weapon;
} inventory;

typedef struct view {
	uint8_t screen[SH][SW]; //Bottom layer tile data.
	uint8_t layers[SH][SW]; //Collision data.
	uint8_t tScreen[SH][SW];//Top layer tile data.
	uint32_t sX;
	uint32_t sY;
	uint8_t flag; //Tells worldgen that it must refresh the entities in a room.
} view;

typedef struct location {
	uint16_t sX;
	uint16_t sY;
	int x;
	int y;
} location;

typedef struct tunnel {
	int m; //Slope
	int c; //Y intercept
	unsigned char type; //Not yet used.
} tunnel;

union rng { //All of these variables are refreshed with reroll()
	int32_t i32;
	uint32_t ui32;
	char c;
	unsigned char uc;
} rng;

void(*options[6])(); //Menu options.

view cornerRoom;

unsigned int speaker; //Holds index of the entity which started conversation.

view tilewrapper[3][3]; //Holds all of the visible view structs
view* mapLoader[WORLDWIDTH][WORLDHEIGHT];
char dungeonRooms[WORLDWIDTH][WORLDHEIGHT];

view emptyView;

unsigned char lastSlot=0;

unsigned int sX = 400; //View struct currently occupied by the player.
unsigned int sY = 601; //^
unsigned char scroll = 0; //Triggers a screen transition on the next frame.

unsigned char pMaxHealth=100;
unsigned char swordOut=0;
unsigned char dialogueOut=0;

unsigned char animationG=0;

unsigned char entrySlot=0;

char* msgBuffer[MSGDEPTH];
unsigned char msgSlot=0;
unsigned char msgOut=0;
unsigned int msgTimeout=0;

unsigned char refresh=1;

#ifdef DEV
char mapEditorEnable=0;
#endif


char* menuText;
char mode=4;
char menuFlag=0;
char menuFirstCall=0;
char collisionReset=0;

entity entSet[ELIMIT];
tunnel tunnels[TLIMIT];
//faction* rootFaction; //Points to the start of the faction linked list, do not destroy.
view _V;

//faction* guineaPig; //Temporary, remove

inventory pInv;

SDL_Surface* sf1 = NULL;

int cameraX=0;
int cameraY=0;

int frameTotal=0;

unsigned int weightedRand(int i, uint32_t in);
view offsetBlendMap(view blayer, view tlayer, int xOff, int yOff);
view blendMap(view blayer, view tlayer);
void pathfind(entity* in, int x, int y, int speed);
void drawClothes(entity* in);
//faction* attachFac(faction newFac);
//void destroyFac(faction* whigs);
void facFrag();
void entityInitialise();
void entityScroll(int x, int y);
void entityReset();
void mapEntitySpawn(char* in, uint16_t xIn, uint16_t yIn, int x, int y);
void entitySpawn(char* in, int x, int y);
void nentityReset();
void nentitySpawn(entity in);
//void factionSpawn(faction* theboys,int x,int y);
int overlap(unsigned int i, unsigned int j);
int euclideanDistance(unsigned int i, unsigned int j, unsigned int distance);
void corpseDisposal();
SDL_Surface* surfLoader (SDL_Surface* imgIn, unsigned int sizeX, unsigned int sizeY, unsigned char inSize, unsigned char outSize, unsigned char tNum);
void text(char* inStr, int x, int y);
void menu();
void pushMsg(char* inStr);
unsigned int get_diff (int val1, int val2);
uint32_t lfsr (uint32_t shift);
void reroll();
int intersect(unsigned int x, unsigned int y);
void generateTunnels();
int32_t getrandom();
void setCollision(view* in, int iX, int iY, char stat);
void worldgen(view* in, uint16_t xPos, uint16_t yPos);
void scrollMap();
void image(SDL_Texture* imgIn, int x, int y, int w, int h);
void tintedImage(SDL_Texture* imgIn, int x, int y, int w, int h, uint32_t colour);
void simage(SDL_Surface* imgIn, int x, int y, int w, int h);
void hudDraw(SDL_Texture* imgIn, int x, int y, int w, int h);
void bgBlit(SDL_Surface* imgIn, int x, int y, int w, int h);
void bgDraw ();
void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour);
void drawRectAlpha(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour, uint8_t alpha);
void drawRectAlphaTrack(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour, uint8_t alpha);
void emptyRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour);
void hudRefresh();
void flip();
char collisionCheck(int x, int y);
void playerMoveX(entity* movEnt, short amount);
void playerMoveY(entity* movEnt, short amount);
void moveX(entity* movEnt, short amount);
void moveY(entity* movEnt, short amount);
void fastMoveX(entity* movEnt, short direction, short speed);
void fastMoveY(entity* movEnt, short direction, short speed);
void snapToGrid(entity* movEnt);
void entityLogic();
void loop();