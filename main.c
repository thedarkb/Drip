#ifdef WEB
#include <emscripten.h>
#endif
#include <SDL2/SDL_image.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "main.h"
#include "rawents.h"
#include "entityLogic.h"
#include "entities.h"
#ifdef DEV
#include "mapeditor.c"
#endif
#include "items.c"
#include "dialogue.c"
#include "clothes.c"
#include "collision.c"
#include "rawents.c"
#include "entities.c"
//#include "factions.c"
#include "maps.c"
#include "worldgen.c"
#include "entityLogic.c"

unsigned int weightedRand(int i) {
	if(i<0) return 0;
	reroll();
	if(!(rng.ui32%4)) return weightedRand(++i);
	return i;
}

view offsetBlendMap(view blayer, view tlayer, int xOff, int yOff) {
	view me;
	me=blayer;
	for(int x=0;x<SW;x++){
		for(int y=0;y<SH;y++) {
			if(tlayer.screen[y][x] && x+xOff < SW && y+yOff < SH && x+xOff >= 0 && y+yOff >= 0) {
				me.screen[y+yOff][x+xOff]=tlayer.screen[y][x];
				me.layers[y+yOff][x+xOff]=tlayer.layers[y][x];
			}
		}
	}
	return me;
}

view blendMap(view blayer, view tlayer) {
	view me;
	me=blayer;
	for(int x=0;x<SW;x++){
		for(int y=0;y<SH;y++) {
			if(tlayer.screen[y][x]) {
				me.screen[y][x]=tlayer.screen[y][x];
				me.layers[y][x]=tlayer.layers[y][x];
			}
		}
	}
	return me;
}

void light() {
	/*unsigned char lightLevel=255;
	for(int i=0; i<ELIMIT; i++) {
		if(entSet[i].behaviour && lightLevel-entSet[i].brightness>0) lightLevel-=entSet[i].brightness;
		else if(entSet[i].behaviour){
			lightLevel=0;
			break;
		}
	}
	drawRectAlpha(0,HUDHEIGHT,SW*TS,SH*TS,0x282600,lightLevel);*/
}

void pathfind(entity* in, int x, int y, int speed) {
	for(int i=0; i<speed; i++) {
		if(x > in->x) moveX(in, 1); //Chase target at x,y
		if(x < in->x) moveX(in, -1);
		if(y > in->y) moveY(in, 1);
		if(y < in->y) moveY(in, -1);
	}
}

void drawClothes(entity* in) {
	if(!in) return;
	if(!in->clothes.colourFrame[0]) return;
	tintedImage(hwtileset[in->clothes.colourFrame[in->direction+in->animation]],in->x,in->y,TS,TS,0xFFFFFF);
}

void entityInitialise() { //Clears entity array, spawns player.
	for (int i=0; i<ELIMIT; i++) {
		memset(&entSet[i], 0, sizeof entSet[i]);
	}
	entSet[0]=ent_playerM();
	memset(&pInv, 0, sizeof pInv);
	//pInv.items[0].type=2;
	//pInv.items[1].type=3;
	//pInv.items[2].type=4;
}

void entityScroll(int x, int y) { //Corrects entity positions when player moves to a new area.
	for (int i=0; i<ELIMIT; i++) {
		entSet[i].x=entSet[i].x+((SW*TS)*x);
		entSet[i].y=entSet[i].y+((SH*TS)*y);
		if (entSet[i].x < -SW*TS || entSet[i].x > (SW*TS)*2) memset(&entSet[i], 0, sizeof entSet[i]);
		if (entSet[i].y < -SH*TS || entSet[i].y > (SH*TS)*2) memset(&entSet[i], 0, sizeof entSet[i]);
	}
}

void mapEntitySpawn(entity in, uint16_t xIn, uint16_t yIn, int x, int y) {
	int eX=(xIn-sX)*(SW*TS);
	int eY=(yIn-sY)*(SH*TS);
	entitySpawn(in,eX+x,eY+y);
}

void entitySpawn(entity in, int x, int y) {
	if(in.behaviour) {
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

void deadEntityKiller() {
	for (int i=0; i<ELIMIT; i++) {
		if(entSet[i].health==0) {
			if(entSet[i].behaviour && entSet[i].behaviour != &behav_dead) {
				entSet[i].behaviour=behav_dead;
				printf("Killing %d\n with deathframe %u\n", i, entSet[i].deathframe);
			}

			entSet[i].collisionClass=0;
			entSet[i].layer=0;
			if(entSet[i].drop[0] != 0) {
				printf("Spawning drop...\n");
				entitySpawn(ent_item(entSet[i].drop[0], 255,0,0),entSet[i].x,entSet[i].y);
				memset(&entSet[i].drop, 0, sizeof entSet[i].drop);
			}
		}
	}
}

int euclideanDistance(unsigned int i, unsigned int j, unsigned int distance) {
	if(DIST(entSet[i].x,entSet[i].y, entSet[j].x, entSet[j].y)<distance*distance) return 1;
	return 0;
}

int overlap(unsigned int i, unsigned int j){
	if (!entSet[j].collisionClass) return 0;
	if (!entSet[i].collisionClass) return 0;
	if (i==j) return 0;
	if (entSet[j].collisionClass>128) return 0;
	if (DIST(entSet[i].x,entSet[i].y,entSet[j].x,entSet[j].y)<entSet[i].radius*entSet[i].radius) return 1;
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
				printf("OVERLAP\n");
				if(entSet[i].collider && entSet[j].collisionClass<128) entSet[i].collider(i,j);
				if(entSet[j].collider && entSet[i].collisionClass<128) entSet[j].collider(j,i);
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
	drawRect(2,74,236,58,0);
	int optCount=0;
	static int optSel=0;

	for(int i=0;menuText[i];) if(menuText[i++]==10) optCount++; //Counts the number of lines in the menu text.

	if(optSel>optCount)optSel=0;
	text(menuText,11,56); //Displays menu text.
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

	text(">", 2, 56+(optSel*10));  //Draws the cursor.
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

void imageNoTrack(SDL_Texture* imgIn, int x, int y, int w, int h) { //Copies an image from the hardware buffer to the screen.
	SDL_Rect scaler = {x,y+HUDHEIGHT,w,h}; //Accounts for HUD but NOT the camera.
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}

void image(SDL_Texture* imgIn, int x, int y, int w, int h) { //Copies an image from the hardware buffer to the screen.
	SDL_Rect scaler = {x+CX,y+CY,w,h}; //Accounts for HUD and camera.
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}

void tintedImage(SDL_Texture* imgIn, int x, int y, int w, int h, uint32_t colour) { //Copies an image from the hardware buffer to the screen.
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	if(SDL_SetTextureColorMod(imgIn, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0])) printf("Colour Mod not supported.\n");
	SDL_Rect scaler = {x+CX,y+CY,w,h}; //Accounts for HUD and camera.
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
	//SDL_BlitSurface(imgIn, NULL, bgLayer, &scaler);
}

void bgDraw (view* in) { //Takes a tileset and blits it to the background layer.
	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			bgBlit(swtileset[in->screen[y][x]],x*TS,y*TS,TS,TS);
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

void drawRectAlpha(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour, uint8_t alpha) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], alpha);
	SDL_Rect scaler={x,y,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void drawRectAlphaTrack(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour, uint8_t alpha) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], alpha);
	SDL_Rect scaler = {x+CX,y+CY,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void drawRectTrack(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) { //Draws filled rectangle
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler = {x+CX,y+CY,w,h};
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
	//SDL_RenderClear(r);
}

char collisionCheck(int x, int y) { //Collision detection between map layer and entity.
	#ifdef DEV
	if(mapEditorEnable) return 0;
	#endif
	
	int wrapperX=(x+TS*SW)/(TS*SW);
	int wrapperY=(y+TS*SH)/(TS*SH);
	int microX=(x+TS*SW)%(TS*SW);
	int microY=(y+TS*SH)%(TS*SH);
	//return 0;
	if(wrapperX>2 || wrapperY>2) return 1;
	//assert(wrapperX<3);
	if(microX==0 && microY==0 ) return 0;
	return tilewrapper[wrapperX][wrapperY].layers[microY/TS][microX/TS];
	return 0;
}

void moveX(entity* movEnt, short amount) { //Pretty self explanatory
	if (amount>0) movEnt->direction = 3;
	else movEnt->direction=2;

	movEnt->animation = ((animationG/15)*4)+4;
	if (movEnt->animation > 8) movEnt->animation=8;

	unsigned int check = (*movEnt).x + amount;
	unsigned int checkY=movEnt->y+TS/2-movEnt->ySub;
	//if (check >TS*SW-TS) return;
	if (collisionCheck(check+(*movEnt).xSub, checkY+TS/8)) return; //Checks all the corners, plus a bit in the middle.
	if (collisionCheck(check+(*movEnt).xSub, checkY+TS/2)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, checkY+TS/8)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, checkY+TS/2)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, checkY+TS/2-(*movEnt).ySub)) return;
	if (collisionCheck(check+(*movEnt).xSub, checkY+TS/2-(*movEnt).ySub)) return;
	(*movEnt).x = check;
}

void moveY(entity* movEnt, short amount) {
	if (amount>0) movEnt->direction = 1;
	else movEnt->direction=0;

	movEnt->animation = ((animationG/15)*4)+4;
	if (movEnt->animation > 8) movEnt->animation=8;

	unsigned int check = (*movEnt).y+amount+TS/2;
	unsigned int checkY=movEnt->y+TS/2;

	if (collisionCheck((*movEnt).x+(*movEnt).xSub, check+(*movEnt).ySub)) return;
	if (collisionCheck((*movEnt).x+TS-(*movEnt).xSub, check+(*movEnt).ySub)) return;
	if (collisionCheck((*movEnt).x+TS/2, check+(*movEnt).ySub)) return;
	if (collisionCheck((*movEnt).x+TS-(*movEnt).xSub, check+TS/2-(*movEnt).ySub)) return;
	if (collisionCheck((*movEnt).x+TS/2, check+TS/2-(*movEnt).ySub)) return;	
	if (collisionCheck((*movEnt).x+(*movEnt).xSub, check+TS/2-(*movEnt).ySub)) return;	
	(*movEnt).y = check-TS/2;
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
	SDL_SetRenderDrawColor(r,0,0,0,255);
	SDL_RenderClear(r);
	if (scroll) scrollMap();

	if (refresh) {		
		for(int i=1;i<ELIMIT;i++) memset(&entSet[i],0,sizeof entSet[i]);
		memset(&tilewrapper,0,sizeof tilewrapper);
		for (int x=0; x<3; x++) {
			for (int y=0; y<3; y++) {
				worldgen(&tilewrapper[x][y],(sX-1)+x,(sY-1)+y);
				bgDraw(&tilewrapper[x][y]);
			}
		}
		refresh=0;
	}

	if(!frameTotal) {
		menuReset;
		menuText="Boy\nGirl\0";
		options[0]=diag_boy;
		options[1]=diag_girl;
		menuCall;
	}

	char xStart=0;
	char yStart=0;
	char xEnd=3;
	char yEnd=3;

	if(cameraX<(TS*SW)/2) xEnd=2;
	else xStart=1;

	if(cameraY<(TS*SH)/2) yEnd=2;
	else yStart=1;

	if(cameraX==(TS*SW)/2) {
		xStart=1;
		xEnd=2;
	}
	if(cameraY==(TS*SH)/2) {
		yStart=1;
		yEnd=2;
	}

	int offsetX=(RESX)/2-cameraX-TS/2;
	int offsetY=(RESY)/2-cameraY-TS/2;
	for(int wx=xStart;wx<xEnd;wx++){
		int worldX=(wx*SW*TS-SW*TS);
		for(int wy=yStart;wy<yEnd;wy++) {
			int worldY=(wy*SH*TS-SH*TS);
			for(int x=0;x<SW;x++) {
				for(int y=0;y<SH;y++) {
					int writeX=(x*TS)+worldX+offsetX;
					int writeY=(y*TS)+worldY+offsetY;
					if(writeX>0-TS && writeX<RESX && writeY>0-TS && writeY<RESY) {
						imageNoTrack(hwtileset[tilewrapper[wx][wy].screen[y][x]],writeX,writeY,TS,TS);
					}
				}
			}
		}
	}
	deadEntityKiller();
	spriteCollisions();
	entityLogic();

	for(int wx=xStart;wx<xEnd;wx++){
		int worldX=(wx*SW*TS-SW*TS);
		for(int wy=yStart;wy<yEnd;wy++) {
			int worldY=(wy*SH*TS-SH*TS);
			for(int x=0;x<SW;x++) {
				for(int y=0;y<SH;y++) {
					#ifdef DEV
					if(mapEditorEnable && keyboard[SDL_SCANCODE_V]) continue;
					#endif
					int writeX=(x*TS)+worldX+offsetX;
					int writeY=(y*TS)+worldY+offsetY;
					if(writeX>0-TS && writeX<SW*TS && writeY>0-TS && writeY<SH*TS && tilewrapper[wx][wy].tScreen[y][x]){
						imageNoTrack(hwtileset[tilewrapper[wx][wy].tScreen[y][x]],writeX,writeY,TS,TS);
					}
				}
			}
		}
	}

	if (mode==1) popMsg();
	else if (menuFlag) menu();
	else mode=0;

	//light();

	#ifndef DEV
	hudRefresh();
	#endif

	#ifdef DEV
	if(mapEditorEnable)drawEditorOverlay();
	else hudRefresh();
	#endif

	flip();
	cameraX=entSet[0].x;
	cameraY=entSet[0].y;

	if(animationG<30) animationG+=2;
	else animationG=0;
	
	frameTotal++;
	//assert(frameTotal<300);
}

int main () {
	memset(&emptyView,0,sizeof emptyView);
	memset(&flagArray,0,sizeof flagArray);
	SDL_Init(SDL_INIT_VIDEO);
	#ifdef WEB
	w = SDL_CreateWindow(TITLE, 0, 0, SW*TS*2, (SH*TS+HUDHEIGHT)*2, SDL_WINDOW_OPENGL);
	#else
	w = SDL_CreateWindow(TITLE, 0, 0, SW*TS*3, (SH*TS+HUDHEIGHT)*3, SDL_WINDOW_OPENGL);
	#endif
	r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(r, TS*SW,TS*SH+HUDHEIGHT);
	#ifdef WEB
	SDL_RenderSetScale(r,2,2);
	#else
	SDL_RenderSetScale(r,4,4);
	#endif
	SDL_RenderSetClipRect(r,&clipRect);
	SDL_SetRenderDrawBlendMode(r,SDL_BLENDMODE_BLEND);
	s = SDL_GetWindowSurface(w);
	rng.ui32=4; //SEEDS THE MAIN RNG
	//initialiseFactions();
	tunnels[0].m=1;
	tunnels[0].c=0;
	memset(&tilewrapper, 0, sizeof tilewrapper);
	lOverlay=SDL_CreateRGBSurface(0,SW*TS,SH*TS,32,0,0,0,0);
	SDL_SetSurfaceBlendMode(lOverlay,SDL_BLENDMODE_NONE);

	keyboard = SDL_GetKeyboardState(NULL);

	loader = IMG_Load("sheet.png"); //tilesheet

	#ifdef DEV
	initMapEditor();
	#endif


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
	generateTunnels();
	axiomLoad();//Loads the map function pointer matrix.
	memset(&tilewrapper[1][1],0,sizeof tilewrapper[1][1]); //Resets the player's spawn area
	memset(&tilewrapper,0,sizeof tilewrapper);
	entityInitialise();	
	
	#ifndef WEB
	while(1) { //The timing loop leaves a little to be desired.
		if (SDL_GetTicks()-timer < 1000/FRAMERATE && !keyboard[SDL_SCANCODE_P]) SDL_Delay(1000/FRAMERATE);
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