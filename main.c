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
}

void entityReset() {
	for (int i=1; i<ELIMIT; i++) {
		memset(&entSet[i], 0, sizeof entSet[i]);
	}
	spawnSlot=1;
}

void entitySpawn(entity in) {
	if (spawnSlot > ELIMIT) {
		printf("Spawn failed, limit exceeded.\n");
		return;
	}
	entSet[spawnSlot]=in;
	spawnSlot++;
}

void nentityReset() {
	for (int i=1; i<ELIMIT; i++) {
		memset(&nentSet[i], 0, sizeof entSet[i]);
	}
	nspawnSlot=1;
}

void nentitySpawn(entity in) {
	if (nspawnSlot > ELIMIT) {
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
				entitySpawn(ent_item(entSet[i].x, entSet[i].y, entSet[i].drop[0]));
				memset(&entSet[i].drop, 0, sizeof entSet[i].drop);
			}
		}
	}
}

void spriteCollisions() {
	for (int i=0; i<spawnSlot; i++) {
		for (int j=0; j<spawnSlot; j++) {
			if((entSet[j].x+TS/2 > entSet[i].x+entSet[i].xSub) && (entSet[j].x+TS/2 < entSet[i].x+TS-entSet[i].xSub) && entSet[j].collisionClass != entSet[i].collisionClass && entSet[j].collisionClass<128 && entSet[j].collisionClass!=0 ){ //Checks for X overlap and class conflicts.
				if((entSet[j].y+TS/2 > entSet[i].y+entSet[i].ySub) && (entSet[j].y+TS/2 < entSet[i].y+TS-entSet[i].ySub)) { //Checks for Y overlap.
					switch (entSet[i].collisionClass) {
						case 2:
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
							switch(entSet[i].direction) {
								case 0:
									entSet[j].status[2]=entSet[j].collisionClass;
									entSet[j].status[1]=10;
									entSet[j].status[0]=entSet[j].behaviourId;
									entSet[j].behaviourId=6; //up right
									entSet[j].collisionClass=entSet[i].collisionClass;
								break;
								case 1:
									entSet[j].status[2]=entSet[j].collisionClass;
									entSet[j].status[1]=10;
									entSet[j].status[0]=entSet[j].behaviourId;
									entSet[j].behaviourId=8; //down right
									entSet[j].collisionClass=entSet[i].collisionClass;
								break;
								case 2:
									entSet[j].status[2]=entSet[j].collisionClass;
									entSet[j].status[1]=10;
									entSet[j].status[0]=entSet[j].behaviourId;
									entSet[j].behaviourId=7; //down left
									entSet[j].collisionClass=entSet[i].collisionClass;
								break;
								case 3:
									entSet[j].status[2]=entSet[j].collisionClass;
									entSet[j].status[1]=10;
									entSet[j].status[0]=entSet[j].behaviourId;
									entSet[j].behaviourId=5; //up left
									entSet[j].collisionClass=entSet[i].collisionClass;
								break;
								if (entSet[entSet[i].status[1]].attack > entSet[j].health) entSet[j].health-=entSet[entSet[i].status[1]].attack;
								else entSet[j].health=0;									
							}
							if (entSet[entSet[i].status[1]].attack < entSet[j].health) entSet[j].health-=entSet[entSet[i].status[1]].attack;
							else entSet[j].health=0;
						break;
						case 130:
							printf("I executed\n");
							if (j != 0) break;
							for (int k=0; k<INVLIMIT; k++) {
								if (pInv.items[k] == 0) {
									pInv.items[k]=entSet[i].status[0];
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
}

void mapLoader(char entities[SW][SH], char collisions[SW][SH]) {
	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			if(collisions[x][y]>0) setCollision(x,y,collisions[x][y]);
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
	printf("Scroll status: %u\n", ENABLESCROLL);
	SDL_Init(SDL_INIT_VIDEO);
	w = SDL_CreateWindow(TITLE, 0, 0, SW*TS, SH*TS, SDL_WINDOW_OPENGL);
	r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
	s = SDL_GetWindowSurface(w);
	bgLayer=SDL_CreateRGBSurface(0,SW*TS,SH*TS,32,0,0,0,0);
	scrollLayer=SDL_CreateRGBSurface(0,SW*TS,SH*TS,32,0,0,0,0);

	keyboard = SDL_GetKeyboardState(NULL);

	loader = IMG_Load("sheet.png"); //tilesheet

	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			setCollision(x,y,0);
		}
	}

	/*surfLoader: First arg is the width of the tilesheet, second is height, third is tile size on sheet, fourth is for the
	tile size as stored, fifth is for the tile number. It reads from the */
	for (int i = 0; i<TILECOUNT; i++) {
		tileset[i] = surfLoader (loader, SHEETX, SHEETY, 16, 64, i);
		SDL_SetColorKey(tileset[i], SDL_TRUE, 0x00FF00);
	}

	SDL_FreeSurface(loader);
	unsigned int timer=0;
	entityInitialise();
	#ifndef WEB
	while(1) {
		if (SDL_GetTicks()-timer < 1000/FRAMERATE) SDL_Delay(1000/FRAMERATE);
		else printf("Frames dropped: %u\n", (SDL_GetTicks()-timer)/(1000/FRAMERATE)+1);
		timer = SDL_GetTicks();
		SDL_PollEvent(&keyIn);
		if (keyIn.type == SDL_QUIT) goto quit;
		loop();
		assert(ent_sword(1,1,1,0).behaviourId==9);
	}
	quit:
	#endif
	#ifdef WEB
	emscripten_set_main_loop(loop, 30, 1);
	#endif
	return 0;
}

unsigned int get_diff (int val1, int val2) {
	if (val1>val2) return val1-val2;
	else return val2-val1;
	return 0;
}

uint32_t lfsr (uint32_t shift) {
	for (int i=0; i<32; i++) {
		shift ^= (shift >> 31);
		shift ^= (shift >> 31) << 4;
		shift ^= (shift >> 31) << 5;
		shift ^= (shift >> 31) << 30;
		shift = shift << 1;
	}
	return shift;
}

uint32_t getrandom() {
	return lfsr(SDL_GetTicks()) >> 1;
}

void setCollision(int iX, int iY, char stat) { //Leaves a 1 pixel border to allow for slight sprite overlap.
	for (int x=1; x<TS-1; x++) {
		for (int y=1; y<TS-1; y++) {
			nlayers[(iX*TS)+x][(iY*TS)+y] = stat; 
		}
	}
}

void image(SDL_Surface* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x,y,w,h};
	SDL_BlitSurface(imgIn, NULL, s, &scaler);
}

void bgBlit(SDL_Surface* imgIn, int x, int y, int w, int h) {
	SDL_Rect scaler = {x,y,w,h};
	SDL_BlitSurface(imgIn, NULL, bgLayer, &scaler);
}

void bgDraw () {
	for (int x=0; x<SW; x++) {
		for (int y=0; y<SH; y++) {
			bgBlit(tileset[cScreen[x][y]],x*TS,y*TS,TS,TS);
		}
	}
}

void flip() {
	t = SDL_CreateTextureFromSurface(r, s);
	SDL_RenderCopy(r,t,NULL,NULL);
	SDL_DestroyTexture(t);
	SDL_RenderPresent(r);
}

char collisionCheck(int x, int y) {
	return layers[x][y];
}

void moveX(entity* movEnt, char amount) {
	if (amount>0) movEnt->direction = 3;
	else movEnt->direction=2;

	movEnt->animation = ((animationG/15)*4)+4;
	if (movEnt->animation > 8) movEnt->animation=8;

	unsigned int check = (*movEnt).x + amount;
	if (check >TS*SW-TS) return;
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
	if (check > TS*SH-TS) return;
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
	if (scroll != 0) scrollMap();
	
	if (refresh != 0) {
		worldgen(sX,sY);
		memcpy(&cScreen, &nScreen, sizeof nScreen);
		memcpy(&layers, &nlayers, sizeof nlayers);
		bgDraw();
		refresh=0;
	}
	image(bgLayer,0,0,SW*TS,SH*TS);
	deadEntityKiller();
	spriteCollisions();
	entityLogic();
	deadEntityKiller();
	flip();
	if(animationG<30) animationG+=2;
	else animationG=0;
}
