#ifdef WEB
#include <emscripten.h>
#endif
#include <SDL2/SDL_image.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "main.h"
#include "items.c"
#include "entities.c"
#include "maps.c"
#include "worldgen.c"
#include "entityLogic.c"

void entityInitialise() {
	for (int i=0; i<ELIMIT; i++) {
		memset(&entSet[i], 0, sizeof entSet[i]);
	}
	entSet[0]=ent_player();
	memset(&pInv, 0, sizeof pInv);
}

void entityScroll(int x, int y) {
	for (int i=0; i<spawnSlot; i++) {
		entSet[i].x=entSet[i].x+((SW*TS)*x);
		entSet[i].y=entSet[i].y+((SH*TS)*y);
		if (entSet[i].x < -SW*TS || entSet[i].x > (SW*TS)*2) memset(&entSet[i], 0, sizeof entSet[i]);
		if (entSet[i].y < -SH*TS || entSet[i].y > (SH*TS)*2) memset(&entSet[i], 0, sizeof entSet[i]);
	}
}

void entityReset() {
	for (int i=1; i<ELIMIT; i++) {
		memset(&entSet[i], 0, sizeof entSet[i]);
	}
	spawnSlot=1;
}

void entitySpawn(entity in, int x, int y) {
	if(in.behaviourId) {
		if (spawnSlot > ELIMIT-1) {
			printf("Spawn limit exceeded.\n");
			corpseDisposal();
		}
		entSet[spawnSlot]=in;
		entSet[spawnSlot].x+=x;
		entSet[spawnSlot].y+=y;
		spawnSlot++;
	}
}

void nentityReset() {
	for (int i=1; i<ELIMIT; i++) {
		memset(&nentSet[i], 0, sizeof entSet[i]);
	}
	nspawnSlot=1;
}

void nentitySpawn(entity in) {
	if (nspawnSlot > ELIMIT-1) {
		printf("Spawn failed, limit exceeded. (Off the current screen.)\n");
		return;
	}
	nentSet[nspawnSlot]=in;
	nspawnSlot++;
}

void deadEntityKiller() {
	for (int i=0; i<spawnSlot; i++) {
		if(entSet[i].health==0) {
			entSet[i].behaviourId=255;
			entSet[i].collisionClass=0;
			if(entSet[i].drop[0] != 0) {
				entitySpawn(ent_item(entSet[i].x, entSet[i].y, entSet[i].drop[0], 255),0,0);
				memset(&entSet[i].drop, 0, sizeof entSet[i].drop);
			}
		}
	}
}

void corpseDisposal() {
	printf("Despawning dead entities.\n");
	memcpy(&nentSet, &entSet, sizeof entSet);
	entityReset();
	for (int i=1; i<ELIMIT; i++) {
		if (nentSet[i].health>0) {
			entitySpawn(nentSet[i],0,0);
			printf("Live entity found in position: %u\n", i);
		}
	}
	spawnSlot=nspawnSlot;
}

int overlap(unsigned char i, unsigned char j){
	if (!entSet[j].collisionClass) return 0;
	if (entSet[j].collisionClass>128) return 0;
	if (entSet[i].collisionClass == entSet[j].collisionClass) return 0;
	if (get_diff(entSet[i].x+TS/2,entSet[j].x+TS/2) < 10) {
		if (get_diff(entSet[i].y+TS/2,entSet[j].y+TS/2) < 10) return 1;
	}
	return 0;
}

void spriteCollisions() {
	for (int i=0; i<spawnSlot; i++) {
		for (int j=0; j<spawnSlot; j++) {
			if(overlap(i,j)){
				switch (entSet[i].collisionClass) {
					case 2:
					if (entSet[j].health>20) entSet[j].health-=20;
					else entSet[j].health=0;
					if (entSet[j].y+TS/2 < entSet[i].y+TS/2) {
						if(entSet[j].x+TS/2<entSet[i].x+TS/2) {
							entSet[j].status[2]=entSet[j].collisionClass;
							entSet[j].status[1]=10;
							entSet[j].status[0]=entSet[j].behaviourId;
							entSet[j].behaviourId=5; //up left
							entSet[j].collisionClass=entSet[i].collisionClass;
						}
						else {
							entSet[j].status[2]=entSet[j].collisionClass;
							entSet[j].status[1]=10;
							entSet[j].status[0]=entSet[j].behaviourId;
							entSet[j].behaviourId=6; //up right
							entSet[j].collisionClass=entSet[i].collisionClass;
						}
					}
					else {
						if(entSet[j].x+TS/2<entSet[i].x+TS/2) {
							entSet[j].status[2]=entSet[j].collisionClass;
							entSet[j].status[1]=10;
							entSet[j].status[0]=entSet[j].behaviourId;
							entSet[j].behaviourId=7; //down left
							entSet[j].collisionClass=entSet[i].collisionClass;
						}
						else {
							entSet[j].status[2]=entSet[j].collisionClass;
							entSet[j].status[1]=10;
							entSet[j].status[0]=entSet[j].behaviourId;
							entSet[j].behaviourId=8; //down right
							entSet[j].collisionClass=entSet[i].collisionClass;
						}
					}
					break;
					case 129:
						printf("Collision\n");
						printf("Direction: %u\n", entSet[i].direction);
						switch(entSet[entSet[i].status[1]].direction) {
							case 0:
								entSet[j].status[2]=entSet[j].collisionClass;
								entSet[j].status[1]=10;
								entSet[j].status[0]=entSet[j].behaviourId;
								entSet[j].behaviourId=11; //up
								entSet[j].collisionClass=entSet[i].collisionClass;
							break;
							case 1:
								entSet[j].status[2]=entSet[j].collisionClass;
								entSet[j].status[1]=10;
								entSet[j].status[0]=entSet[j].behaviourId;
								entSet[j].behaviourId=12; //down
								entSet[j].collisionClass=entSet[i].collisionClass;
							break;
							case 2:
								entSet[j].status[2]=entSet[j].collisionClass;
								entSet[j].status[1]=10;
								entSet[j].status[0]=entSet[j].behaviourId;
								entSet[j].behaviourId=13; //left
								entSet[j].collisionClass=entSet[i].collisionClass;
							break;
							case 3:
								entSet[j].status[2]=entSet[j].collisionClass;
								entSet[j].status[1]=10;
								entSet[j].status[0]=entSet[j].behaviourId;
								entSet[j].behaviourId=14; //right
								entSet[j].collisionClass=entSet[i].collisionClass;
							break;
							if (entSet[entSet[i].status[1]].attack > entSet[j].health) entSet[j].health-=entSet[entSet[i].status[1]].attack;
							else entSet[j].health=0;									
						}
						if (entSet[entSet[i].status[1]].attack < entSet[j].health) entSet[j].health-=entSet[entSet[i].status[1]].attack;
						else entSet[j].health=0;
					break;
					case 130:
						if (j) break;
						for (int k=0; k<INVLIMIT; k++) {
							if (!pInv.items[k].type) {
								pInv.items[k].type=entSet[i].status[0];
								entSet[i].health=0;
								break;
							}
						}
					break;							
				}
			}
		}
	}
}

void mapLoader(char entities[SW][SH], char collisions[SW][SH]) {
	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			if(collisions[x][y]>0) setCollision(&tilewrapper[1][1],x,y,collisions[x][y]);
		}
	}
	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			printf("X: %u\n", x);
			printf("Y: %u\n", y);
			printf("Entity: %u\n", entities[x][y]);
			if(entities[x][y]>0) nentitySpawn(ent_nonsolid(x,y,entities[x][y]));
		}
	}
}

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
	for (int i = 0; i<TILECOUNT; i++) {
		swtileset[i] = surfLoader(loader, SHEETX, SHEETY, 16, TS, i);
		SDL_SetColorKey(swtileset[i], SDL_TRUE, 0x00FF00);
	}
	for (int i=0; i<TILECOUNT; i++) {
		hwtileset[i]=SDL_CreateTextureFromSurface(r, swtileset[i]);
	}
	SDL_FreeSurface(loader);
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
	memset(&entSet, 0, sizeof entSet);
	entityInitialise();
	memset(&tilewrapper[1][1],0,sizeof tilewrapper[1][1]);
	
	
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
	
	
	#ifndef WEB
	while(1) {
		if (SDL_GetTicks()-timer < 1000/FRAMERATE) SDL_Delay(1000/FRAMERATE);
		else printf("Frames dropped: %u\n", (SDL_GetTicks()-timer)/(1000/FRAMERATE)+1);
		timer = SDL_GetTicks();
		SDL_PollEvent(&keyIn);
		if (keyIn.type == SDL_QUIT) goto quit;
		loop();
	}
	quit:
	#endif
	#ifdef WEB
	emscripten_set_main_loop(loop, 30, 1);
	#endif
	return 0;
}

void text(char* inStr, int x, int y) {
	int i=0;
	int xb=x;
	while (inStr[i]) {
		if (inStr[i]>37) simage(font[inStr[i]], x, y, 7, 7);
		x+=8;
		if(inStr[i]==10) {
			y+=8;
			x=xb;
		}
		i++;
	}
		
}	

void pushMsg(char* inStr) {
	mode=1;
	msgBuffer[msgSlot]=inStr;
	msgSlot++;
}

void popMsg(){
	static char keyPressed=0;
	drawRect(2,120,236,58,0);
	text(msgBuffer[msgSlot-1],2,102);
	if (!keyPressed) { 	
		if (keyboard[SDL_SCANCODE_Z] && msgTimeout>10) {
			printf("Next slot:\n");
			msgSlot--;
			if (!msgSlot) mode=0;
			msgTimeout=0;
			return;
		}
		if (keyboard[SDL_SCANCODE_Z]) keyPressed=1;
	}
	if(!keyboard[SDL_SCANCODE_Z]) keyPressed=0;
	msgTimeout++;
	printf("Reading message slot: %u\n", msgSlot);
}

unsigned int get_diff (int val1, int val2) {
	if (val1>val2) return val1-val2;
	else return val2-val1;
	return 0;
}

uint32_t lfsr (uint32_t shift) {
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

void generateTunnels() {
	for(char i=0; i<TLIMIT; i++) {
		duplicate://Dirty, I know.
		reroll();
		tunnels[i].m=rng.c>>5;
		if (!tunnels[i].m) tunnels[i].m++;
		reroll();
		tunnels[i].c=((rng.c/8)*8)/2;
		if (tunnels[i].m>1 || tunnels[i].m<-1) tunnels[i].c *= tunnels[i].m;
		if (tunnels[i].c>0 && tunnels[i].m>0) tunnels[i].m*=-1;
		if (tunnels[i].c<0 && tunnels[i].m<0) tunnels[i].m*=-1;
		for(int j=0; j<i; j++){if(tunnels[i].m == tunnels[j].m && tunnels[i].c == tunnels[j].c) goto duplicate;}
		//Goes back to the top of the for loop if a duplicate line is detected.
		printf("Tunnel %d, m=%d; c=%d\n", i, tunnels[i].m, tunnels[i].c);
	}
}

uint32_t getrandom() {
	union signedOut {
		uint32_t in;
		int32_t out;
	} signedOut;
	signedOut.in=lfsr(SDL_GetTicks()) >> 1;
	return signedOut.out;
}

void setCollision(view* in, int iX, int iY, char stat) { //Leaves a 1 pixel border to allow for slight sprite overlap.
	for (int x=1; x<TS-1; x++) {
		for (int y=1; y<TS-1; y++) {
			in->layers[(iX*TS)+x][(iY*TS)+y] = stat; 
		}
	}
}

void image(SDL_Texture* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x+(TS*SW)/2-cameraX,y+(TS*SH)/2+HUDHEIGHT-cameraY,w,h};
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}

void timage(SDL_Texture* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x,y,w,h};
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}

void simage(SDL_Surface* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x,y+HUDHEIGHT,w,h};
	SDL_Texture* imgOut = SDL_CreateTextureFromSurface(r, imgIn);
	SDL_RenderCopy(r, imgOut, NULL, &scaler);
	SDL_DestroyTexture(imgOut);
}

void hudDraw(SDL_Texture* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x,y,w,h};
	SDL_RenderCopy(r, imgIn, NULL, &scaler);
}

void bgBlit(SDL_Surface* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x,y,w,h};
	SDL_BlitSurface(imgIn, NULL, bgLayer, &scaler);
}

void bgDraw (view* in) {
	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			bgBlit(swtileset[in->screen[x][y]],x*TS,y*TS,TS,TS);
		}
	}
}

void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) {
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler={x,y,w,h};
	SDL_RenderFillRect(r, &scaler);
}

void emptyRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, uint32_t colour) {
	union htmlDecode {
		uint32_t htmlCode;
		unsigned char rgb[3];
	} htmlDecode;
	htmlDecode.htmlCode=colour;
	SDL_SetRenderDrawColor(r, htmlDecode.rgb[2], htmlDecode.rgb[1], htmlDecode.rgb[0], 255);
	SDL_Rect scaler={x,y,w,h};
	SDL_RenderDrawRect(r, &scaler);
}

void hudRefresh() {
	drawRect(0,0,TS*SW,HUDHEIGHT,0);
	drawRect(132,4,entSet[0].health*(105/pMaxHealth),4,0x6DAA2C);
	drawRect(132,8,105,4,0xd04648);
	drawRect(132,12,105,4,0x597dce);
	for (int i=0; i<INVLIMIT; i++) {
		if (pInv.items[i].type) hudDraw(hwtileset[getItemSprite(pInv.items[i].type)], TS*i, 2, TS, TS);
		hudDraw(hwtileset[83], TS*i, 2, TS, TS);
	}
	drawRect((pInv.selection*TS)+1,2,TS-2,1,0xFFFF00);
}

void flip() {
	//t = SDL_CreateTextureFromSurface(r, s);
	//SDL_RenderCopy(r,t,NULL,NULL);
	//SDL_DestroyTexture(t);
	SDL_RenderPresent(r);
}

char collisionCheck(int x, int y) {
	int wrapperX=(x+TS*SW)/(TS*SW);
	int wrapperY=(y+TS*SH)/(TS*SH);
	int microX=(x+TS*SW)%(TS*SW);
	int microY=(y+TS*SH)%(TS*SH);
	assert(wrapperX<3);
	return tilewrapper[wrapperX][wrapperY].layers[microX][microY];
	return 0;
}

void moveX(entity* movEnt, char amount) {
	if (amount>0) movEnt->direction = 3;
	else movEnt->direction=2;

	movEnt->animation = ((animationG/15)*4)+4;
	if (movEnt->animation > 8) movEnt->animation=8;

	unsigned int check = (*movEnt).x + amount;
	//if (check >TS*SW-TS) return;
	if (collisionCheck(check+(*movEnt).xSub, (*movEnt).y+(*movEnt).ySub)) return;
	if (collisionCheck(check+(*movEnt).xSub, (*movEnt).y+TS/2)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, (*movEnt).y+(*movEnt).ySub)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, (*movEnt).y+TS/2)) return;
	if (collisionCheck(check+TS-(*movEnt).xSub, (*movEnt).y+TS-(*movEnt).ySub)) return;
	if (collisionCheck(check+(*movEnt).xSub, (*movEnt).y+TS-(*movEnt).ySub)) return;
	(*movEnt).x = check;
}

void moveY(entity* movEnt, char amount) {
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

void fastMoveX(entity* movEnt, char direction, char speed) {
	for (int i=0; i<speed; i++) {
		moveX(movEnt, direction);
	}
}

void fastMoveY(entity* movEnt, char direction, char speed) {
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
		spriteCollisions();
		deadEntityKiller();
		entityLogic();
	} else popMsg();
	cameraX=entSet[0].x;
	cameraY=entSet[0].y;
	hudRefresh();
	flip();
	if(animationG<30) animationG+=2;
	else animationG=0;
}
