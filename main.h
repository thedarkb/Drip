#define SH 10 //Screen Height
#define SW 15 //Screen Width
#define TS 16 //Tile Size
#define WW 10
#define WH 10

#define ELIMIT 512 //Entity limit must not exceed 256
#define MAPELIMIT 8
#define FLIMIT 32
#define ENTVARIETY 1
#define FACTIONLIMIT 2
#define TLIMIT 16
#define SPRITELAYERS 3
#define INVLIMIT 8
#define ENTFRAMES 32
#define TILECOUNT 239 //Set this to the actual number of sprites for best performance.
#define FRAMERATE 30
#define HUDHEIGHT 20
#define MSGDEPTH 32
#define MSGTIME 30

#define SHEETX 128 //Tile sheet size
#define SHEETY 480

#define BIT(x) (1<<x)

#define ANIMPARSE entSet[i].frame[entSet[i].direction+entSet[i].animation]
#define THIS entSet[i]
							
#define LOADVIEW(p, x) \
							_V=x;\
							p=malloc(sizeof _V);\
							*p=_V
#define DIST(x1,y1,x2,y2) ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))
#define PAIR(x,y) (((x)<<16)|(y)) //This is very dirty and only designed for use with uint16_t sX/sY pairs.
#define TITLE "Drip"


SDL_Window* w = NULL;
SDL_Surface* s = NULL;
SDL_Surface* lOverlay=NULL;
SDL_Surface* loader = NULL;
SDL_Renderer* r = NULL;
SDL_Texture* t = NULL;
SDL_Rect hudStripper = {0,HUDHEIGHT, SW*TS, SH*TS};
SDL_Event keyIn;
const uint8_t* keyboard = NULL;


SDL_Surface* swtileset[TILECOUNT]; 
SDL_Texture* hwtileset[TILECOUNT];
SDL_Surface* font[128];
SDL_Texture* hwfont[128];
SDL_Surface* bgLayer=NULL;
SDL_Texture* bgTex[3][3];
SDL_Surface* scrollLayer = NULL;

//typedef struct faction faction;

typedef struct outfit {
	unsigned char frame[FLIMIT];
	unsigned char colourFrame[FLIMIT];
} outfit;

typedef struct entity {
	int x; 
	int y;
	unsigned char xSub; //Defines hitbox
	unsigned char ySub; //^
	short hitX; //Determines the centre of the hitbox during sprite on sprite collision calculations.
	short hitY; //^
	void(*behaviour)(int); //Holds the pointer to the behaviour in entityLogic.c
	void(*prevState)(int); //Used to hold the main behaviour while a temporary behaviour is in use.
	unsigned int freezeFrames; //Can be used by weapons to slow player, other uses may vary by entity.
	unsigned char visible; //1 if entity is visible to NPCs
	unsigned char partisan; //1 if entity is part of the faction system.
	unsigned char direction; //Facing direction
	unsigned char animation; //Animation frame
	unsigned char collisionClass; //Collision classes above 128 are not themselves susceptible to collisions.
	unsigned char layer; //Where they are in the sprite stack.
	unsigned char attack; //RPG style attack stat.
	unsigned int status[4]; //General purpose entity specific variables.
	uint16_t health;
	unsigned int frame[FLIMIT]; //Array of animation frames.
	unsigned char setframe; //Current frame
	unsigned char deathframe; //Death sprite
	unsigned char drop[4]; //Array of items, drop table.
	unsigned char lastHit; //The last entity to strike them.
	void(*dialogue)(); //Points to the dialogue, stored in dialogue.c; the domain of the man, the myth, the Merlin.
	outfit clothes;
	int pathX; //Used by the pathfinding function to check if the NPC is stuck.
	int pathY;
	unsigned char pathType; //Pathfinding mode.
	unsigned char brightness;
} entity;

typedef struct item {
	unsigned char type;
	unsigned char status;
} item;

typedef struct inventory {
	item items[INVLIMIT];
	unsigned char selection;
	unsigned char weapon;
} inventory;

typedef struct view {
	unsigned char screen[SW][SH]; //Tile data.
	unsigned char layers[SW][SH]; //Collision data.
	unsigned char flag; //Tells worldgen that it must refresh the entities in a room.
	unsigned char room; //Tells loadspawn to reset data.
	void(*spawnFunc)(unsigned int xIn, unsigned int yIn);
} view;

typedef struct collisionWrap {
	unsigned char layers[SW*TS][SH*TS];	
} collisionWrap;

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

unsigned char speaker; //Holds entity number which started conversation.

view tilewrapper[3][3]; //Holds all of the visible view structs
collisionWrap cwrapper[3][3]; //Holds the corresponding collision data
void(*mapLoader[705][610])(view* in, unsigned int xPos, unsigned int yPos);

uint16_t flags=0;

unsigned char lastSlot=0;

uint16_t sX = 300; //View struct currently occupied by the player.
uint16_t sY = 300; //^
unsigned char scroll = 0; //Triggers a screen transition on the next frame.

unsigned char pMaxHealth=100;
unsigned char swordOut=0;
unsigned char dialogueOut=0;

unsigned char animationG=0;

location entranceStack[10]; //Holds the last 10 warp locations for return from interior warps.
unsigned char entrySlot=0;

char* msgBuffer[MSGDEPTH];
unsigned char msgSlot=0;
unsigned char msgOut=0;
unsigned int msgTimeout=0;

unsigned char refresh=1;

char* menuText;
char mode=0;
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
void mapEntitySpawn(entity in, uint16_t xIn, uint16_t yIn, int x, int y);
void entitySpawn(entity in, int x, int y);
void nentityReset();
void nentitySpawn(entity in);
//void factionSpawn(faction* theboys,int x,int y);
void deadEntityKiller();
void corpseDisposal();
//void mapLoader(char entities[SW][SH], char collisions[SW][SH]);
SDL_Surface* surfLoader (SDL_Surface* imgIn, unsigned int sizeX, unsigned int sizeY, unsigned char inSize, unsigned char outSize, unsigned char tNum);
void menu();
void pushMsg(char* inStr);
unsigned int get_diff (int val1, int val2);
uint32_t lfsr (uint32_t shift);
void reroll();
int intersect(unsigned int x, unsigned int y);
void generateTunnels();
int32_t getrandom();
void setCollision(view* in, int iX, int iY, char stat);
void scaleCollision(collisionWrap* out, view* in);
void worldgen(view* in, uint16_t xPos, uint16_t yPos);
void scrollMap();
void image(SDL_Texture* imgIn, int x, int y, int w, int h);
void tintedImage(SDL_Texture* imgIn, int x, int y, int w, int h, uint32_t colour);
void simage(SDL_Surface* imgIn, int x, int y, int w, int h);
void bgBlit(SDL_Surface* imgIn, int x, int y, int w, int h);
void bgDraw ();
void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour);
void drawRectAlpha(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour, uint8_t alpha);
void emptyRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour);
void hudRefresh();
void flip();
char collisionCheck(int x, int y);
void moveX(entity* movEnt, short amount);
void moveY(entity* movEnt, short amount);
void fastMoveX(entity* movEnt, short direction, short speed);
void fastMoveY(entity* movEnt, short direction, short speed);
void snapToGrid(entity* movEnt);
void entityLogic();
void loop();