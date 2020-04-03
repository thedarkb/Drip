void entFetch(unsigned int xIn, unsigned int yIn) {
	uint32_t screenNum=0;
	screenNum |= sX+(xIn-1);
	screenNum |= sY+(yIn-1) << 16;
	
	int xMult=(xIn-1)*(SW*TS);
	int yMult=(yIn-1)*(SH*TS);
	int xOffset=(xIn-1);
	int yOffset=(yIn-1);	
	tilewrapper[xIn][yIn].flag=0;
}

void loadSpawn() {
	for(int x=0; x<3; x++) {
		for(int y=0; y<3; y++) {
			if (tilewrapper[x][y].flag) {
				entFetch(x,y);
				//if(tilewrapper[x][y].spawnFunc) tilewrapper[x][y].spawnFunc(sX+(x-1),sY+(y-1));
				//factionFetch(x,y);
				tilewrapper[x][y].flag=0;
			}
		}
	}
}

void worldgen(view* in, uint16_t xPos, uint16_t yPos) {
	loadSpawn();
	uint32_t screenHash=PAIR(xPos,yPos);
	screenHash = lfsr(screenHash);
	memset(in,0,sizeof tilewrapper[1][1]);

	#ifdef DEV
	if(editorArray[xPos][yPos]) {
		*in=*editorArray[xPos][yPos];
		return;
	}
	#endif

	if(mapLoader[xPos][yPos]) {
		mapLoader[xPos][yPos](in,xPos,yPos);
		return;
	}

	//printf("Difference: %d\n", diff);
	if(DIST(xPos,yPos,400,400)<40000 && yPos>250 && xPos<267) {
		*in=map_grasslandBase(xPos,yPos);
	} else if( DIST(xPos,yPos,400,400)<40000 && yPos<250){
		*in=map_snowlandBase(xPos,yPos);
	} else if(DIST(xPos,yPos,400,400)<40000 && yPos==250) {
		*in=map_snowgrass(xPos,yPos);
	} else if((DIST(xPos,yPos,400,400)<40000 && yPos>250 && xPos>267)) {
		*in=map_burren(xPos,yPos);
	} else if (DIST(xPos,yPos,400,400)<40804){
		*in=map_beach(xPos,yPos);
	} else if(xPos<605&&yPos<605){
		memset(&in->screen,21,sizeof in->screen);
		memset(&in->layers,1,sizeof in->layers);
	} else {
		memset(in,0,sizeof *in);
		memset(in->layers,1,sizeof *in);
	}
	in->flag=1;
}

/*void worldgen(view* in, uint16_t xPos, uint16_t yPos) {
	if(xPos>WW-1 || yPos>WH-1) return;
	if(!world[xPos][yPos]) return;
	*in=*world[xPos][yPos];
	in->flag=1;
	loadSpawn();
}*/

void scrollMap() {
	int checkX = entSet[0].x;
	int checkY = entSet[0].y;
	//collisionReset=1;
	const int speed=(TS*SW)/24;
	switch (scroll) {
		case 1:
			entityScroll(0,1);
			
			checkY=TS*SH-5;
			sY--;
			
			tilewrapper[0][2]=tilewrapper[0][1];
			tilewrapper[1][2]=tilewrapper[1][1];
			tilewrapper[2][2]=tilewrapper[2][1];
			
			tilewrapper[0][1]=tilewrapper[0][0];
			tilewrapper[1][1]=tilewrapper[1][0];
			tilewrapper[2][1]=tilewrapper[2][0];
			
			
			SDL_DestroyTexture(bgTex[0][2]);
			SDL_DestroyTexture(bgTex[1][2]);
			SDL_DestroyTexture(bgTex[2][2]);	
				
			
			bgTex[0][2]=bgTex[0][1];
			bgTex[1][2]=bgTex[1][1];
			bgTex[2][2]=bgTex[2][1];
			
			bgTex[0][1]=bgTex[0][0];
			bgTex[1][1]=bgTex[1][0];
			bgTex[2][1]=bgTex[2][0];
			
			
			worldgen(&tilewrapper[0][0],sX-1,sY-1);
			bgDraw(&tilewrapper[0][0]);
			bgTex[0][0]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[1][0],sX,sY-1);
			bgDraw(&tilewrapper[1][0]);
			bgTex[1][0]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[2][0],sX+1,sY-1);
			bgDraw(&tilewrapper[2][0]);
			bgTex[2][0]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			//entSet[0].x=checkX;
			//entSet[0].y=TS*SH;
			cameraX=entSet[0].x;
			cameraY=entSet[0].y;
		scroll=0;
		break;
		case 2:
		checkY=5;
			entityScroll(0,-1);
			sY++;
			
			
			tilewrapper[0][0]=tilewrapper[0][1];
			tilewrapper[1][0]=tilewrapper[1][1];
			tilewrapper[2][0]=tilewrapper[2][1];
			
			tilewrapper[0][1]=tilewrapper[0][2];
			tilewrapper[1][1]=tilewrapper[1][2];
			tilewrapper[2][1]=tilewrapper[2][2];
			
			SDL_DestroyTexture(bgTex[0][0]);
			SDL_DestroyTexture(bgTex[1][0]);
			SDL_DestroyTexture(bgTex[2][0]);
			
			
			bgTex[0][0]=bgTex[0][1];
			bgTex[1][0]=bgTex[1][1];
			bgTex[2][0]=bgTex[2][1];
			
			bgTex[0][1]=bgTex[0][2];
			bgTex[1][1]=bgTex[1][2];
			bgTex[2][1]=bgTex[2][2];
			
			
			worldgen(&tilewrapper[0][2],sX-1,sY+1);
			bgDraw(&tilewrapper[0][2]);
			bgTex[0][2]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[1][2],sX,sY+1);
			bgDraw(&tilewrapper[1][2]);
			bgTex[1][2]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[2][2],sX+1,sY+1);
			bgDraw(&tilewrapper[2][2]);
			bgTex[2][2]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			//entSet[0].x=checkX;
			//entSet[0].y=1;
			cameraX=entSet[0].x;
			cameraY=entSet[0].y;
			scroll=0;
		break;
		case 3:
			entityScroll(1,0);
			sX--;
			
			tilewrapper[2][0]=tilewrapper[1][0];
			tilewrapper[2][1]=tilewrapper[1][1];
			tilewrapper[2][2]=tilewrapper[1][2];
			
			tilewrapper[1][0]=tilewrapper[0][0];
			tilewrapper[1][1]=tilewrapper[0][1];
			tilewrapper[1][2]=tilewrapper[0][2];

			
			
			SDL_DestroyTexture(bgTex[2][0]);
			SDL_DestroyTexture(bgTex[2][1]);
			SDL_DestroyTexture(bgTex[2][2]);
				
			
			bgTex[2][0]=bgTex[1][0];
			bgTex[2][1]=bgTex[1][1];
			bgTex[2][2]=bgTex[1][2];
			
			bgTex[1][0]=bgTex[0][0];
			bgTex[1][1]=bgTex[0][1];
			bgTex[1][2]=bgTex[0][2];
			
			
			worldgen(&tilewrapper[0][0],sX-1,sY-1);
			bgDraw(&tilewrapper[0][0]);
			bgTex[0][0]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[0][1],sX-1,sY);
			bgDraw(&tilewrapper[0][1]);
			bgTex[0][1]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[0][2],sX-1,sY+1);
			bgDraw(&tilewrapper[0][2]);
			bgTex[0][2]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			//entSet[0].x=TS*SW-TS;
			cameraX=entSet[0].x;
			cameraY=entSet[0].y;
			scroll=0;
		break;
		case 4:
			entityScroll(-1,0);
			sX++;
			
			tilewrapper[0][0]=tilewrapper[1][0];
			tilewrapper[0][1]=tilewrapper[1][1];
			tilewrapper[0][2]=tilewrapper[1][2];
			
			tilewrapper[1][0]=tilewrapper[2][0];
			tilewrapper[1][1]=tilewrapper[2][1];
			tilewrapper[1][2]=tilewrapper[2][2];

			
			
			SDL_DestroyTexture(bgTex[0][0]);
			SDL_DestroyTexture(bgTex[0][1]);
			SDL_DestroyTexture(bgTex[0][2]);
				
			
			bgTex[0][0]=bgTex[1][0];
			bgTex[0][1]=bgTex[1][1];
			bgTex[0][2]=bgTex[1][2];
			
			bgTex[1][0]=bgTex[2][0];
			bgTex[1][1]=bgTex[2][1];
			bgTex[1][2]=bgTex[2][2];
			
			
			worldgen(&tilewrapper[2][0],sX+1,sY-1);
			bgDraw(&tilewrapper[2][0]);
			bgTex[2][0]=SDL_CreateTextureFromSurface(r, bgLayer);

			
			worldgen(&tilewrapper[2][1],sX+1,sY);
			bgDraw(&tilewrapper[2][1]);
			bgTex[2][1]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[2][2],sX+1,sY+1);
			bgDraw(&tilewrapper[2][2]);
			bgTex[2][2]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			//entSet[0].x=-TS+1;
			cameraX=entSet[0].x;
			cameraY=entSet[0].y;
			scroll=0;
		break;			
	}
}

void generateTunnels() {
	for(char i=0; i<TLIMIT; i++) {
		duplicate://Dirty, I know.
		reroll();
		tunnels[i].m=rng.c>>7;
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

void ax_d1entrance(view* in, unsigned int xPos, unsigned int yPos) {
	*in=map_d1entrance();
	mapEntitySpawn(ent_door(18,608,320,120,80),xPos,yPos,6*TS,4*TS);
	mapEntitySpawn(ent_door(18,608,320,120,80),xPos,yPos,7*TS,4*TS);
	mapEntitySpawn(ent_door(18,608,320,120,80),xPos,yPos,6*TS,5*TS);
	mapEntitySpawn(ent_door(18,608,320,120,80),xPos,yPos,7*TS,5*TS);
}

void ax_startPad(view* in, unsigned int xPos, unsigned int yPos){
	memset(&in->screen,1,sizeof in->screen);
	mapEntitySpawn(ent_door(48,301,300,16,32),xPos,yPos,16,16);
	mapEntitySpawn(ent_door(48,595,380,16,32),xPos,yPos,64,64);
	mapEntitySpawn(ent_door(48,607,305,48,64),xPos,yPos,64,128);
	mapEntitySpawn(ent_door(48,608,310,80,64),xPos,yPos,80,128);
	in->flag=1;
}

void ax_testhouse(view* in, unsigned int xPos, unsigned int yPos) {
	memset(&in->screen,1,sizeof in->screen);
	*in=map_interior();
	mapEntitySpawn(ent_door(48,300,300,64,144),xPos,yPos,48,80);
	mapEntitySpawn(ent_npc(),xPos,yPos,48,32);
}

void ax_d1rightCorridor1f(view *in, unsigned int xPos, unsigned int yPos){
	*in=map_d1rightCorridor1f();
}

void ax_d1topRightCorner1f(view* in, unsigned int xPos, unsigned int yPos) {
	*in=map_d1topRightCorner1f();
}

void ax_d1bottomRightCorner1f(view* in, unsigned int xPos, unsigned int yPos) {
	*in=map_d1bottomRightCorner1f();
}

void ax_d1topRightHall1f(view* in, unsigned int xPos, unsigned int yPos) {
	*in=map_d1topRightHall1f();	
}

void ax_d1bottomRightHall1f(view* in, unsigned int xPos, unsigned int yPos){
	*in=map_d1bottomRightHall1f();
}

void ax_d1atrium(view* in, unsigned int xPos, unsigned int yPos) {
	*in=map_d1atrium();
}

void axiomLoad(){
	memset(&mapLoader,0,sizeof mapLoader);
	mapLoader[300][300]=ax_startPad;
	mapLoader[607][305]=ax_testhouse;
	mapLoader[608][310]=ax_d1entrance;
	mapLoader[608][320]=ax_d1rightCorridor1f;
	mapLoader[608][319]=ax_d1topRightCorner1f;
	mapLoader[608][321]=ax_d1bottomRightCorner1f;
	mapLoader[607][319]=ax_d1topRightHall1f;
	mapLoader[607][321]=ax_d1bottomRightHall1f;
	mapLoader[606][321]=ax_d1atrium;
}