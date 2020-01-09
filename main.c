#ifdef WEB
#include <emscripten.h>
#endif
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "main.h"
#include "entities.c"
#include "maps.c"
#include "worldgen.c"
#include "entityLogic.c"

void entityInitialise() {
	for (register i=0; i<ELIMIT; i++) {
		memset(&entSet[i], 0, sizeof entSet[i]);
	}
	entSet[0]=ent_player();
}

void entityReset() {
	for (register i=1; i<ELIMIT; i++) {
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
	for (register i=1; i<ELIMIT; i++) {
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

void mapLoader(char entities[SW][SH], char collisions[SW][SH]) {
	for (register x=0; x<SW; x++) {
		for (register y=0; y<SH; y++) {
			if(collisions[x][y]>0) setCollision(x,y,collisions[x][y]);
		}
	}
	for (register x=0; x<SW; x++) {
		for (register y=0; y<SH; y++) {
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
	w = SDL_CreateWindow("Test", 0, 0, SW*TS, SH*TS, SDL_WINDOW_OPENGL);
	r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
	s = SDL_GetWindowSurface(w);
	bgLayer=SDL_CreateRGBSurface(0,SW*TS,SH*TS,32,0,0,0,0);
	scrollLayer=SDL_CreateRGBSurface(0,SW*TS,SH*TS,32,0,0,0,0);

	keyboard = SDL_GetKeyboardState(NULL);

	loader = IMG_Load("sheet.png"); //tilesheet

	for (register x=0; x<SW; x++) {
		for (register y=0; y<SH; y++) {
			setCollision(x,y,0);
		}
	}

	/*surfLoader: First arg is the width of the tilesheet, second is height, third is tile size on sheet, fourth is for the
	tile size as stored, fifth is for the tile number. It reads from the */
	for (register i = 0; i<TILECOUNT; i++) {
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
	for (register i=0; i<32; i++) {
		shift ^= (shift >> 31);
		shift ^= (shift >> 31) << 4;
		shift ^= (shift >> 31) << 5;
		shift ^= (shift >> 31) << 30;
		shift = shift << 1;
	}
	return shift;
}

uint32_t getrandom() {
	return lfsr(SDL_GetTicks());
}

void setCollision(int iX, int iY, char stat) { //Leaves a 1 pixel border to allow for slight sprite overlap.
	for (register x=1; x<TS-1; x++) {
		for (register y=1; y<TS-1; y++) {
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
	for (register x=0; x<SW; x++) {
		for (register y=0; y<SH; y++) {
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

void snapToGrid(entity* movEnt) {
	if (((*movEnt).x % TS) > (TS/2)) (*movEnt).x+=(TS-((*movEnt).x % TS));
	else (*movEnt).x = ((*movEnt).x/TS)*TS;
	if (((*movEnt).y % TS) > (TS/2)) (*movEnt).y+=(TS-((*movEnt).y % TS));
	else (*movEnt).y = ((*movEnt).y/TS)*TS;
}

void loop() {
	if (scroll != 0) {
		int checkX = entSet[0].x;
		int checkY = entSet[0].y;
		switch (scroll) {
			case 1:
			sY--;
			SDL_BlitSurface(s, NULL, scrollLayer, NULL);
			worldgen(sX,sY);
			checkY=TS*SH-5;
			if (nlayers[checkX][checkY] == 0 && nlayers[checkX+TS/2][checkY] == 0 && nlayers[checkX+TS][checkY] == 0 ) {
				entSet[0].x=checkX;
				memcpy(&cScreen, &nScreen, sizeof nScreen);
				memcpy(&layers, &nlayers, sizeof nlayers);
				memcpy(&entSet, &nentSet, sizeof nentSet);
				spawnSlot=nspawnSlot;
				bgDraw();
				#ifndef NOSCROLL
					for (register i=0; i<SH*TS; i+=5) {
						if(entSet[0].y<TS*SH-TS) entSet[0].y=entSet[0].y+5;
						image(bgLayer,0,i-SH*TS,SW*TS,SH*TS);
						image(scrollLayer,0,i,SW*TS,SH*TS);
						flip();
					}
				#endif
				entSet[0].y=TS*SH-TS-1;
			}
			else sY++;
			scroll=0;
			break;
			case 2:
			sY++;
			SDL_BlitSurface(s, NULL, scrollLayer, NULL);
			worldgen(sX,sY);
			checkY=5;
			if (nlayers[checkX][checkY] == 0 && nlayers[checkX+TS/2][checkY] == 0 && nlayers[checkX+TS][checkY] == 0 ) {
				entSet[0].x=checkX;
				memcpy(&cScreen, &nScreen, sizeof nScreen);
				memcpy(&layers, &nlayers, sizeof nlayers);
				memcpy(&entSet, &nentSet, sizeof nentSet);
				spawnSlot=nspawnSlot;
				bgDraw();
				#ifndef NOSCROLL
					for (register i=SH*TS; i>0; i=i-5) {
						if(entSet[0].y>0) entSet[0].y=entSet[0].y-5;
						image(bgLayer,0,i,SW*TS,SH*TS);
						image(scrollLayer,0,i-SH*TS,SW*TS,SH*TS);
						flip();
					}
				#endif
				entSet[0].y=1;
			}
			else sY--;
			scroll=0;
			break;
			case 3:
			sX--;
			SDL_BlitSurface(s, NULL, scrollLayer, NULL);
			worldgen(sX,sY);
			checkX=TS*SW-5;
			if (nlayers[checkX][checkY] == 0 && nlayers[checkX][checkY+TS/2] == 0 && nlayers[checkX][checkY+TS] == 0 ) {
				memcpy(&cScreen, &nScreen, sizeof nScreen);
				memcpy(&layers, &nlayers, sizeof nlayers);
				memcpy(&entSet, &nentSet, sizeof nentSet);
				spawnSlot=nspawnSlot;
				bgDraw();
				#ifndef NOSCROLL
					for (register i=0; i<SW*TS; i+=5) {
						if(entSet[0].x<SW*TS-TS) entSet[0].x+=5;
						image(bgLayer,i-SW*TS,0,SW*TS,SH*TS);
						image(scrollLayer,i,0,SW*TS,SH*TS);
						flip();
					}
				#endif
				entSet[0].x=TS*SW-TS;
			}
			else sX++;
			scroll=0;
			break;
			case 4:
			sX++;
			SDL_BlitSurface(s, NULL, scrollLayer, NULL);
			worldgen(sX,sY);
			checkX=5;
			if (nlayers[checkX][checkY] == 0 && nlayers[checkX][checkY+TS/2] == 0 && nlayers[checkX][checkY+TS] == 0 ) {
				//entSet[0].x=checkX;
				memcpy(&cScreen, &nScreen, sizeof nScreen);
				memcpy(&layers, &nlayers, sizeof nlayers);
				memcpy(&entSet, &nentSet, sizeof nentSet);
				spawnSlot=nspawnSlot;
				bgDraw();
				#ifndef NOSCROLL
					for (register i=SW*TS; i>0; i-=5) {
						if(entSet[0].x>0) entSet[0].x-=5;
						image(bgLayer,i,0,SW*TS,SH*TS);
						image(scrollLayer,i-SW*TS,0,SW*TS,SH*TS);
						flip();
					}
				#endif
				entSet[0].x=1;
			}
			else sX--;
			scroll=0;
			break;
		}

	}
	if (refresh != 0) {
		worldgen(sX,sY);
		memcpy(&cScreen, &nScreen, sizeof nScreen);
		memcpy(&layers, &nlayers, sizeof nlayers);
		bgDraw();
		refresh=0;
	}
	image(bgLayer,0,0,SW*TS,SH*TS);
	entityLogic();
	flip();
	if(animationG<30) animationG+=2;
	else animationG=0;
}
