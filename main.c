#ifdef WEB
#include <emscripten.h>
#endif
#include <SDL2/SDL_image.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "main.h"
#include "entities.h"
#include "entityLogic.h"
#include "items.c"
#include "dialogue.c"
#include "clothes.c"
#include "entities.c"
#include "factions.c"
#include "maps.c"
#include "worldgen.c"
#include "entityLogic.c"

void pathfind(entity* in, int x, int y, int speed) {
	for(int i=0; i<speed; i++) {
		in->pathX=in->x;
		in->pathY=in->y;

		int correctedX=0;
		int correctedY=0;

		if(in->x>SW*TS) correctedX=in->x-(SW*TS);
		else if(x<0) correctedX=in->x+(SW*TS);
		else correctedX=in->x;

		if(in->y>SW*TS) correctedY=in->y-(SH*TS);
		else if(in->y<0) correctedY=in->y+(SH*TS);
		else correctedY=in->y;


		switch(in->pathType) {
			case 0:
			if(x > in->x) moveX(in, 1); //Chase target at x,y
			if(x < in->x) moveX(in, -1);
			if(y > in->y) moveY(in, 1);
			if(y < in->y) moveY(in, -1);
			break;

			case 1:
			if(correctedX<(SW*TS)/2) moveX(in,1); //Move toward clear area in room centre.
			if(correctedX>(SW*TS)/2) moveX(in,-1);
			if(correctedY<(SH*TS)/2) moveY(in,1);
			if(correctedY>(SH*TS)/2) moveY(in,-1);
			break;
		}
		if(in->x==in->pathX && in->y==in->pathY) in->pathType=1;//If no movement has occurred, return to clear area.
		if(correctedX>TS*3 && correctedX<(SW*TS)-(TS*3) && correctedY>TS*3 && correctedY<(SH*TS)-(TS*3)) in->pathType=0; //If in clear area, chase target.

	}
}

void drawClothes(entity* in) {
	if(!in) return;
	if(!in->clothes.colourFrame[0]) return;
	if(!in->faction) tintedImage(hwtileset[in->clothes.colourFrame[in->direction+in->animation]],in->x,in->y,TS,TS,0xFFFFFF);
	else tintedImage(hwtileset[in->clothes.colourFrame[in->direction+in->animation]],in->x,in->y,TS,TS,lfsr(in->faction->baseAlignment));
}

faction* attachFac(faction newFac) { //Adds a new faction to the linked list.
	faction* position=rootFaction;
	printf("Attaching faction\n");
	while(position->next){
		printf("Counting factions...\n");
		position=position->next;
	}
	position->next=malloc(sizeof newFac);
	position=position->next;
	*position=newFac;
	(*position).next=NULL;
	return position;
}

void destroyFac(faction* whigs) {
	faction* position=rootFaction;
	while(position) {
		if(position->next==whigs) {
			position->next=position->next->next;
			free(whigs);
			for(int i=0; i<ELIMIT;i++){
				if(entSet[i].faction==whigs) entSet[i].faction=NULL;
			}
			return;
		}
		position=position->next;
	}	
}

void facFrag() {
	printf("Fragging factions...\n");
	faction* position=rootFaction;
	while(position){
		if(get_diff(position->maxAg, position->minAg)>position->aggroThreshold) {
			printf("A faction is dividing.\n");
			faction* d1=attachFac(fac_fragment(*position, position->minAg));
			faction* d2=attachFac(fac_fragment(*position, position->maxAg));
			for(int i=0; i<ELIMIT; i++) {
				if(entSet[i].faction=position) {
					if(entSet[i].alignment<(position->minAg+position->maxAg)/2) entSet[i].faction=d1;
					else entSet[i].faction=d2;
				}
			}
			faction* parent=position;
			position=position->next;
			destroyFac(parent);
		} else position=position->next;
	}
}

void entityInitialise() { //Clears entity array, spawns player.
	for (int i=0; i<ELIMIT; i++) {
		memset(&entSet[i], 0, sizeof entSet[i]);
	}
	entSet[0]=ent_playerM();
	memset(&pInv, 0, sizeof pInv);
	pInv.items[0].type=2;
}

void entityScroll(int x, int y) { //Corrects entity positions when player moves to a new area.
	for (int i=0; i<ELIMIT; i++) {
		entSet[i].x=entSet[i].x+((SW*TS)*x);
		entSet[i].y=entSet[i].y+((SH*TS)*y);
		if (entSet[i].x < -SW*TS || entSet[i].x > (SW*TS)*2) memset(&entSet[i], 0, sizeof entSet[i]);
		if (entSet[i].y < -SH*TS || entSet[i].y > (SH*TS)*2) memset(&entSet[i], 0, sizeof entSet[i]);
	}
}

void entitySpawn(entity in, int x, int y) {
	if(in.behaviour) {
		printf("Attempting to spawn...\n");
		for(int i=0; i<ELIMIT; i++) {
			if (!entSet[i].behaviour) {
				entSet[i]=in;
				entSet[i].x=x;
				entSet[i].y=y;
				lastSlot=i;
				return;
			}
		}
		printf("Entity table full, culling.\n");
		for(int i=0; i<ELIMIT; i++) {
			if(entSet[i].health<=0) {
				entSet[i]=in;
				entSet[i].x=x;
				entSet[i].y=y;
				lastSlot=i;
				break;
			}
		}
	}
}

void factionSpawn(faction* position,int x,int y) {
	entitySpawn(position->entPlates[0],x,y);
	reroll();
	entSet[lastSlot].faction=position;
	entSet[lastSlot].alignment=position->baseAlignment+(rng.i32%position->alignmentFuzz); //Sets the entity's alignment to the faction's, accounting for fuzz.
	entSet[lastSlot].aggroThreshold=position->aggroThreshold;
	if(entSet[lastSlot].alignment<=position->minAg) entSet[lastSlot].faction->minAg=entSet[lastSlot].alignment;
	else if(entSet[lastSlot].alignment>position->maxAg) entSet[lastSlot].faction->maxAg=entSet[lastSlot].alignment;
}

void deadEntityKiller() {
	for (int i=0; i<ELIMIT; i++) {
		if(entSet[i].health==0) {
			if(entSet[i].behaviour && entSet[i].behaviour != &behav_dead) {
				entSet[i].behaviour=behav_dead;
				printf("Killing %d\n with deathframe %u\n", i, entSet[i].deathframe);
				if(entSet[i].lastHit!=255) {
					printf("Realigning %u\n", entSet[i].lastHit);
					if(entSet[entSet[i].lastHit].alignment>entSet[i].alignment) entSet[entSet[i].lastHit].alignment+=40;
					else entSet[entSet[i].lastHit].alignment-=40;
				}
			}

			entSet[i].collisionClass=0;
			entSet[i].layer=0;
			if(entSet[i].drop[0] != 0) {
				printf("Spawning drop...\n");
				entitySpawn(ent_item(0, 0, entSet[i].drop[0], 255),entSet[i].x,entSet[i].y);
				memset(&entSet[i].drop, 0, sizeof entSet[i].drop);
			}
		}
	}
}

int overlap(unsigned char i, unsigned char j){
	if (!entSet[j].collisionClass) return 0;
	if (!entSet[i].collisionClass) return 0;
	if (i==j) return 0;
	if (entSet[j].collisionClass>128) return 0;
	if (get_diff(entSet[i].x+entSet[i].hitX+TS/2,entSet[j].x+entSet[j].hitX+TS/2) < entSet[i].xSub) {
		if (get_diff(entSet[i].y+entSet[i].hitY+TS/2,entSet[j].y+entSet[j].hitY+TS/2) < entSet[i].ySub) return 1;
	}
	return 0;
}

void spriteCollisions() {
	static int direction=1;
	direction*=-1;
	int start;
	if(direction==1) start=0;
	else start=ELIMIT-1;
	for (int i=start; i<ELIMIT && i>=0; i+=direction) {
		for (int j=start; j<ELIMIT && j>=0; j+=direction) {
			if(overlap(i,j)){
				//printf("Collision detected between %d and %d\n",i,j);
				switch (entSet[i].collisionClass) {
					case 2:
					if(get_diff(entSet[i].alignment, entSet[j].alignment) < entSet[i].aggroThreshold) continue;
					//Stops NPCs of the same faction murdering eachother.
					case 3: //Generic bouncy collisions.
					if (entSet[j].health>20) entSet[j].health-=20;
					else entSet[j].health=0;
					if (entSet[j].y+TS/2 < entSet[i].y+TS/2) {
						if(entSet[j].x+TS/2<entSet[i].x+TS/2) {
							entSet[j].status[2]=entSet[j].collisionClass;
							entSet[j].status[1]=10;
							if(entSet[j].behaviour!=behav_upLeft) entSet[j].prevState=entSet[j].behaviour;
							entSet[j].behaviour=behav_upLeft; //up left
							entSet[j].collisionClass=0;
						}
						else {
							entSet[j].status[2]=entSet[j].collisionClass;
							entSet[j].status[1]=10;
							if(entSet[j].behaviour!=behav_upRight) entSet[j].prevState=entSet[j].behaviour;
							entSet[j].behaviour=behav_upRight; //up right
							entSet[j].collisionClass=0;
						}
					}
					else {
						if(entSet[j].x+TS/2<entSet[i].x+TS/2) {
							entSet[j].status[2]=entSet[j].collisionClass;
							entSet[j].status[1]=10;
							if(entSet[j].behaviour!=behav_downLeft) entSet[j].prevState=entSet[j].behaviour;
							entSet[j].behaviour=behav_downLeft; //down left
							entSet[j].collisionClass=0;
						}
						else {
							entSet[j].status[2]=entSet[j].collisionClass;
							entSet[j].status[1]=10;
							if(entSet[j].behaviour!=behav_downRight) entSet[j].prevState=entSet[j].behaviour;
							entSet[j].behaviour=behav_downRight; //down right
							entSet[j].collisionClass=0;
						}
					}
					break;
					case 129: //Sword colliding with enemy.
						//printf("Collision\n");
						//printf("Direction: %u\n", entSet[i].direction);
						switch(entSet[entSet[i].status[1]].direction) {
							case 0:
								entSet[j].status[2]=entSet[j].collisionClass;
								entSet[j].status[1]=10;
								entSet[j].prevState=entSet[j].behaviour;
								entSet[j].behaviour=behav_up; //up
								entSet[j].collisionClass=entSet[i].collisionClass;
							break;
							case 1:
								entSet[j].status[2]=entSet[j].collisionClass;
								entSet[j].status[1]=10;
								entSet[j].prevState=entSet[j].behaviour;
								entSet[j].behaviour=behav_down; //down
								entSet[j].collisionClass=entSet[i].collisionClass;
							break;
							case 2:
								entSet[j].status[2]=entSet[j].collisionClass;
								entSet[j].status[1]=10;
								entSet[j].prevState=entSet[j].behaviour;
								entSet[j].behaviour=behav_left; //left
								entSet[j].collisionClass=entSet[i].collisionClass;
							break;
							case 3:
								entSet[j].status[2]=entSet[j].collisionClass;
								entSet[j].status[1]=10;
								entSet[j].prevState=entSet[j].behaviour;
								entSet[j].behaviour=behav_right; //right
								entSet[j].collisionClass=entSet[i].collisionClass;
							break;
							entSet[j].lastHit=i;
						}
						if (entSet[entSet[i].status[1]].attack < entSet[j].health) entSet[j].health-=entSet[entSet[i].status[1]].attack;
						else entSet[j].health=0;
					break;
					case 130: //For when the player collides with an item.
						if (j) break;
						for (int k=0; k<INVLIMIT; k++) {
							if (!pInv.items[k].type) {
								pInv.items[k].type=entSet[i].status[0];
								entSet[i].health=0;
								break;
							}
						}
					break;
					case 131: //Entity dialogue.
						speaker=j;
						if(entSet[j].dialogue) entSet[j].dialogue();
						else pushMsg("No dialogue found for entity.\n");
						entSet[i].health=0;
						entSet[i].behaviour=NULL;
						entSet[i].collisionClass=0;
					break;												
				}
			}
		}
	}
}

//Reads tiles from sheet
SDL_Surface* surfLoader (SDL_Surface* imgIn, unsigned int sizeX, unsigned int sizeY, unsigned char inSize, unsigned char outSize, unsigned char tNum) {
	SDL_Surface* tileOut;
	SDL_Rect tile;
	SDL_Rect scaler = {0,0,outSize,outSize};

	tile.x = (tNum % (sizeX/inSize))*inSize;
	tile.y = (tNum / (sizeX/inSize))*inSize;
	tile.w = inSize;
	tile.h = inSize;

	tileOut = SDL_CreateRGBSurface(0,outSize,outSize,32,0,0,0,0);
	SDL_BlitScaled(imgIn, &tile, tileOut, &scaler);
	return tileOut;
}

int main () {
	SDL_Init(SDL_INIT_VIDEO);
	w = SDL_CreateWindow(TITLE, 0, 0, SW*TS*4, (SH*TS+HUDHEIGHT)*4, SDL_WINDOW_OPENGL);
	r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(r, TS*SW,TS*SH+HUDHEIGHT);
	SDL_RenderSetScale(r,4,4);
	s = SDL_GetWindowSurface(w);
	rng.ui32=4; //SEEDS THE MAIN RNG
	generateTunnels();
	initialiseFactions();
	tunnels[0].m=1;
	tunnels[0].c=0;
	memset(&tilewrapper, 0, sizeof tilewrapper);
	memset(&bgTex, 0, sizeof bgTex);
	bgLayer=SDL_CreateRGBSurface(0,SW*TS,SH*TS,32,0,0,0,0);
	scrollLayer=SDL_CreateRGBSurface(0,SW*TS,SH*TS,32,0,0,0,0);

	keyboard = SDL_GetKeyboardState(NULL);

	loader = IMG_Load("sheet.png"); //tilesheet


	/*surfLoader: First arg is the width of the tilesheet, second is height, third is tile size on sheet, fourth is for the
	tile size as stored, fifth is for the tile number. It reads from the */
	for (int i = 0; i<TILECOUNT; i++) { //Loads all of the tiles into memory
		swtileset[i] = surfLoader(loader, SHEETX, SHEETY, 16, TS, i);
		SDL_SetColorKey(swtileset[i], SDL_TRUE, 0x00FF00);
	}
	for (int i=0; i<TILECOUNT; i++) { //Streams the tiles from memory into VRAM
		hwtileset[i]=SDL_CreateTextureFromSurface(r, swtileset[i]);
	}
	SDL_FreeSurface(loader); //Don't want leaks
	loader=IMG_Load("font.png");
	for (int i=0; i<127; i++) {
		font[i]=surfLoader(loader, 889, 7, 7, 7, i);
		SDL_SetColorKey(font[i], SDL_TRUE, 0x00FF00);
	}
	for (int i=0; i<127; i++) {
		hwfont[i]=SDL_CreateTextureFromSurface(r, font[i]);
		//SDL_FreeSurface(font[i]);
	}
	SDL_FreeSurface(loader);
	unsigned int timer=0;
	memset(&entSet, 0, sizeof entSet); //Zeroes out the entity table.
	entityInitialise(); //Loads the player
	memset(&tilewrapper[1][1],0,sizeof tilewrapper[1][1]); //Resets the player's spawn area
	
	
	/*Corner Room definition.*/
	memset(&cornerRoom.screen, 22, sizeof cornerRoom.screen);
	cornerRoom.screen[0][0]=15;
	cornerRoom.screen[0][9]=15;
	cornerRoom.screen[14][0]=15;
	cornerRoom.screen[14][9]=15;
	for (int x=0; x<SW; x++) {
		for(int y=0; y<SH; y++) {
			setCollision(&cornerRoom,x,y,1);
		}
	}
	setCollision(&cornerRoom,0,0,0);
	setCollision(&cornerRoom,0,9,0);
	setCollision(&cornerRoom,14,0,0);
	setCollision(&cornerRoom,14,9,0);	
	/*End of Corner Room definition.*/
	loadmap();
	
	
	#ifndef WEB
	while(1) { //The timing loop leaves a little to be desired.
		if (SDL_GetTicks()-timer < 1000/FRAMERATE) SDL_Delay(1000/FRAMERATE);
		else printf("Frames dropped: %u\n", (SDL_GetTicks()-timer)/(1000/FRAMERATE)+1);
		timer = SDL_GetTicks();
		SDL_PollEvent(&keyIn);
		if (keyIn.type == SDL_QUIT) break;
		loop();
	}
	#endif
	#ifdef WEB
	emscripten_set_main_loop(loop, 30, 1); //For those of you on WebOS
	#endif
	return 0;
}

void text(char* inStr, int x, int y) {
	int i=0;
	int xb=x;
	while (inStr[i]) { //Breaks if it hits a null terminator.
		if (inStr[i]>37) simage(font[inStr[i]], x, y, 7, 7); //If character is printing, print character.
		x+=8; //Moves the cursor
		if(inStr[i]==10) { //If line feed is detected, reset cursor to left side of the screen.
			y+=10; //And move the cursor down one line.
			x=xb;
		}
		i++;
	}
	
}

void menu() {
	drawRect(2,120,236,58,0);
	int i=0;
	int optCount=0;
	static int optSel=0;

	while(menuText[i]) { //Counts the number of lines in the menu text.
		if (menuText[i]==10) optCount++;
		i++;
	}

	text(menuText,11,102); //Displays menu text.
	static int keyPress=1;
	static int zPress=1;
	if(!keyboard[SDL_SCANCODE_UP] && !keyboard[SDL_SCANCODE_DOWN]) keyPress=0;

	if(keyboard[SDL_SCANCODE_UP] && !keyPress && optSel>0) {
		keyPress=1;
		optSel--;
	}
	if(keyboard[SDL_SCANCODE_DOWN] && !keyPress && optSel<optCount) {
		keyPress=1;
		optSel++;
	}
	if(!keyboard[SDL_SCANCODE_Z]) {
		zPress=0;
		menuFirstCall=0;
	}
	if(keyboard[SDL_SCANCODE_Z] && !zPress && !menuFirstCall) {
		menuFlag=0;
		zPress=1;
		if(options[optSel]) options[optSel](); //Options are just function pointers.
	}

	text(">", 2, 102+(optSel*10));  //Draws the cursor.
}

void pushMsg(char* inStr) { //Adds a message to the stack.
	mode=1;
	msgBuffer[msgSlot]=inStr;
	msgSlot++;
}

void popMsg(){
	static char keyPressed=0;
	drawRect(2,120,236,58,0); //Background.
	text(msgBuffer[msgOut],2,102); //Renders text
	if (!keyPressed) { //Pops message off of the stack if you hit Z
		if (keyboard[SDL_SCANCODE_Z] && msgTimeout>10) {
			printf("Next slot:\n");
			msgOut++;
			if (msgSlot==msgOut) { //Breaks out of message mode when the bottom of the stack is hit.
				mode=2;
				msgSlot=0;
				msgOut=0;
			}
			msgTimeout=0;
			return;
		}
		if (keyboard[SDL_SCANCODE_Z]) keyPressed=1;
	}
	if(!keyboard[SDL_SCANCODE_Z]) keyPressed=0;
	msgTimeout++;
}

unsigned int get_diff (int val1, int val2) {
	if (val1>val2) return val1-val2;
	else return val2-val1;
	return 0;
}

uint32_t lfsr (uint32_t shift) { //Pseudo-random number generator.
	for (int i=0; i<33; i++) {
		shift ^= (shift >> 31);
		shift ^= (shift >> 31) << 4;
		shift ^= (shift >> 31) << 5;
		shift ^= (shift >> 31) << 30;
		shift = shift << 1;
	}
	return shift >> 1;
}

void reroll() {
	rng.ui32=lfsr(rng.ui32);
}

int intersect(unsigned int x, unsigned int y) {
	for (int i=0; i<TLIMIT; i++) {
		if (get_diff(y*100, ((tunnels[i].m*x)+tunnels[i].c)*100)<200) return get_diff(y*100, ((tunnels[i].m*x)+tunnels[i].c)*100);
	}
	return -1;
}

int32_t getrandom() {
	union signedOut {
		uint32_t in;
		int32_t out;
	} signedOut;
	signedOut.in=lfsr(SDL_GetTicks()) >> 1;
	return signedOut.out;
}

void setCollision(view* in, int iX, int iY, char stat) { //Leaves a 1 pixel border to allow for slight sprite overlap.
	if(iX>SW-1) return;
	if(iY>SH-1) return;
	for (int x=0; x<TS-1; x++) {
		for (int y=0; y<TS-1; y++) {
			if(y==0 || x==0 || x==TS-1 || y==TS-1) in->layers[(iX*TS)+x][(iY*TS)+y]=0;
			else in->layers[(iX*TS)+x][(iY*TS)+y] = stat; 
		}
	}
}

void image(SDL_Texture* imgIn, int x, int y, int w, int h) { //Copies an image from the hardware buffer to the screen.
	SDL_Rect scaler = {x+(TS*SW)/2-cameraX,y+(TS*SH)/2+HUDHEIGHT-cameraY,w,h}; //Accounts for HUD and camera.
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}

void tintedImage(SDL_Texture* imgIn, int x, int y, int w, int h, uint32_t colour) { //Copies an image from the hardware buffer to the screen.
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	if(SDL_SetTextureColorMod(imgIn, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0])) printf("Colour Mod not supported.\n");
	SDL_Rect scaler = {x+(TS*SW)/2-cameraX,y+(TS*SH)/2+HUDHEIGHT-cameraY,w,h}; //Accounts for HUD and camera.
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
	SDL_SetTextureColorMod(imgIn, 255, 255, 255);
}

/*
void timage(SDL_Texture* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x,y,w,h};
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}*/

void simage(SDL_Surface* imgIn, int x, int y, int w, int h) { //Ditto above, but from software buffer.
	SDL_Rect scaler = {x,y+HUDHEIGHT,w,h};
	SDL_Texture* imgOut = SDL_CreateTextureFromSurface(r, imgIn);
	SDL_RenderCopy(r, imgOut, NULL, &scaler);
	SDL_DestroyTexture(imgOut);
}

void hudDraw(SDL_Texture* imgIn, int x, int y, int w, int h) { //For drawing to the HUD
	SDL_Rect scaler = {x,y,w,h};
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}

void bgBlit(SDL_Surface* imgIn, int x, int y, int w, int h) { //For drawing to the background layer.
	SDL_Rect scaler = {x,y,w,h};
	SDL_BlitSurface(imgIn, NULL, bgLayer, &scaler);
}

void bgDraw (view* in) { //Takes a tileset and blits it to the background layer.
	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			bgBlit(swtileset[in->screen[x][y]],x*TS,y*TS,TS,TS);
		}
	}
}

void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler={x,y,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void drawRectTrack(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler = {x+(TS*SW)/2-cameraX,y+(TS*SH)/2+HUDHEIGHT-cameraY,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void emptyRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) { //Draws empty rectangle.
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler={x,y,w,h};
	SDL_RenderDrawRect(r, &scaler);
}

void hudRefresh() { //Redraws HUD
	drawRect(0,0,TS*SW,HUDHEIGHT,0);
	drawRect(132,4,entSet[0].health*(105/pMaxHealth),4,0x6DAA2C);
	drawRect(132,8,105,4,0xd04648);
	drawRect(132,12,105,4,0x597dce);
	for (int i=0; i<INVLIMIT; i++) {
		if (pInv.items[i].type) hudDraw(hwtileset[getItemSprite(pInv.items[i].type)], TS*i, 2, TS, TS);
		hudDraw(hwtileset[83], TS*i, 2, TS, TS);
	}
	drawRect((pInv.selection*TS)+1,2,TS-2,1,0xFFFF00);
	drawRect((pInv.weapon*TS)+1,TS+1,TS-2,1,0x00FF00);
}

void flip() { //Updates screen.
	//t = SDL_CreateTextureFromSurface(r, s);
	//SDL_RenderCopy(r,t,NULL,NULL);
	//SDL_DestroyTexture(t);
	SDL_RenderPresent(r);
}

char collisionCheck(int x, int y) { //Collision detection between map layer and entity.
	int wrapperX=(x+TS*SW)/(TS*SW);
	int wrapperY=(y+TS*SH)/(TS*SH);
	int microX=(x+TS*SW)%(TS*SW);
	int microY=(y+TS*SH)%(TS*SH);
	if(wrapperX>2 || wrapperY>2) return 1;
	//assert(wrapperX<3);
	if(microX<TS && microY<TS ) return 0;
	return tilewrapper[wrapperX][wrapperY].layers[microX][microY];
	return 0;
}

void moveX(entity* movEnt, short amount) { //Pretty self explanatory
	if (amount>0) movEnt->direction = 3;
	else movEnt->direction=2;

	movEnt->animation = ((animationG/15)*4)+4;
	if (movEnt->animation > 8) movEnt->animation=8;

	unsigned int check = (*movEnt).x + amount;
	//if (check >TS*SW-TS) return;
	if (collisionCheck(check+(*movEnt).xSub, (*movEnt).y+(*movEnt).ySub)) return; //Checks all the corners, plus a bit in the middle.
	if (collisionCheck(check+(*movEnt).xSub, (*movEnt).y+TS/2)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, (*movEnt).y+(*movEnt).ySub)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, (*movEnt).y+TS/2)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, (*movEnt).y+TS-(*movEnt).ySub)) return;
	if (collisionCheck(check+(*movEnt).xSub, (*movEnt).y+TS-(*movEnt).ySub)) return;
	(*movEnt).x = check;
}

void moveY(entity* movEnt, short amount) {
	if (amount>0) movEnt->direction = 1;
	else movEnt->direction=0;

	movEnt->animation = ((animationG/15)*4)+4;
	if (movEnt->animation > 8) movEnt->animation=8;

	unsigned int check = (*movEnt).y + amount;
	//if (check > TS*SH) return;
	if (collisionCheck((*movEnt).x+(*movEnt).xSub, check+(*movEnt).ySub)) return;
	if (collisionCheck((*movEnt).x+TS-(*movEnt).xSub, check+(*movEnt).ySub)) return;
	if (collisionCheck((*movEnt).x+TS/2, check+(*movEnt).ySub)) return;
	if (collisionCheck((*movEnt).x+TS-(*movEnt).xSub, check+TS-(*movEnt).ySub)) return;
	if (collisionCheck((*movEnt).x+TS/2, check+TS-(*movEnt).ySub)) return;	
	if (collisionCheck((*movEnt).x+(*movEnt).xSub, check+TS-(*movEnt).ySub)) return;	
	(*movEnt).y = check;
}

void fastMoveX(entity* movEnt, short direction, short speed) { //Use these for high speed movement to prevent clipping.
	for (int i=0; i<speed; i++) {
		moveX(movEnt, direction);
	}
}

void fastMoveY(entity* movEnt, short direction, short speed) {
	for (int i=0; i<speed; i++) {
		moveY(movEnt, direction);
	}
}

void snapToGrid(entity* movEnt) {
	if (((*movEnt).x % TS) > (TS/2)) (*movEnt).x+=(TS-((*movEnt).x % TS));
	else (*movEnt).x = ((*movEnt).x/TS)*TS;
	if (((*movEnt).y % TS) > (TS/2)) (*movEnt).y+=(TS-((*movEnt).y % TS));
	else (*movEnt).y = ((*movEnt).y/TS)*TS;
}

void loop() {
	if (scroll) scrollMap();
	if (refresh) {
		entityInitialise();
		assert(!entSet[0].alignment);
		for (int x=0; x<3; x++) {
			for (int y=0; y<3; y++) {
				worldgen(&tilewrapper[x][y],(sX-1)+x,(sY-1)+y);
				bgDraw(&tilewrapper[x][y]);
				bgTex[x][y]=SDL_CreateTextureFromSurface(r, bgLayer);
			}
		}
		refresh=0;
	}
	if (!mode) {
		image(bgTex[0][0],-SW*TS,-SH*TS,SW*TS,SH*TS);
		image(bgTex[1][0],0,-SH*TS,SW*TS,SH*TS);
		image(bgTex[2][0],SW*TS,-SH*TS,SW*TS,SH*TS);

		image(bgTex[0][1],-SW*TS,0,SW*TS,SH*TS);
		image(bgTex[1][1],0,0,SW*TS,SH*TS);
		image(bgTex[2][1],SW*TS,0,SW*TS,SH*TS);

		image(bgTex[0][2],-SW*TS,SH*TS,SW*TS,SH*TS);
		image(bgTex[1][2],0,SH*TS,SW*TS,SH*TS);
		image(bgTex[2][2],SW*TS,SH*TS,SW*TS,SH*TS);

		deadEntityKiller();
		spriteCollisions();
		entityLogic();
	} else if (mode==1) popMsg();
	else if (menuFlag) menu();
	else mode=0;

	cameraX=entSet[0].x;
	cameraY=entSet[0].y;
	hudRefresh();
	flip();
	if(animationG<30) animationG+=2;
	else animationG=0;
	
	if(!frameTotal%200) facFrag();
	frameTotal++;
}
