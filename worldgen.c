void entFetch(unsigned int xIn, unsigned int yIn) {
	uint32_t screenNum=0;
	screenNum |= sX+(xIn-1);
	screenNum |= sY+(yIn-1) << 16;
	
	int xMult=(xIn-1)*(SW*TS);
	int yMult=(yIn-1)*(SH*TS);
	int xOffset=(xIn-1);
	int yOffset=(yIn-1);	
	
	if(screenNum==0) {
	//	entitySpawn(ent_aitest(),xMult+80,yMult+120);
	//	entitySpawn(ent_techNpc(),xMult,yMult);
	//	entitySpawn(ent_agNpc(),xMult+70,yMult+70);
	}
	tilewrapper[xIn][yIn].flag=0;
	//return ent_empty();
}

void factionFetch(unsigned int xIn, unsigned int yIn) {
	int x = sX+(xIn-1);
	int y = sY+(yIn-1);

	int xMult=(xIn-1)*(SW*TS);
	int yMult=(yIn-1)*(SH*TS);

	faction* position=rootFaction;
	int counter=0;
	if(intersect(x,y)) return;

	while(position) {
		if(get_diff(position->centreX, x) < position->radius && get_diff(position->centreY,y) < position->radius) {
			printf("Spawning entities belonging to faction in position %d\n",counter++);
			factionSpawn(position, xMult+120, yMult+80);
		}
		position=position->next;
	}
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
	uint32_t hashme=0;

	hashme |= xPos;
	hashme |= yPos << 16;

	uint32_t screenHash = lfsr(hashme);

	//printf("%u\n",hashme);
	/*if(xPos==2 && yPos==2) {
		view sroom=map_spawnroom();
		memcpy(in,&sroom,sizeof sroom);
		return;
	}*/

	int diff=intersect(xPos, yPos);
	if(!diff) in->room=1;
	printf("Difference: %d\n", diff);
	if(xPos<400 && yPos<400) {
			*in=map_grasslandBase(xPos,yPos);
	} else memset(in,0,sizeof *in);
	in->spawnFunc=NULL;
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

			cwrapper[0][2]=cwrapper[0][1];
			cwrapper[1][2]=cwrapper[1][1];
			cwrapper[2][2]=cwrapper[2][1];
			
			cwrapper[0][1]=cwrapper[0][0];
			cwrapper[1][1]=cwrapper[1][0];
			cwrapper[2][1]=cwrapper[2][0];
			
			
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

			cwrapper[0][0]=cwrapper[0][1];
			cwrapper[1][0]=cwrapper[1][1];
			cwrapper[2][0]=cwrapper[2][1];
			
			cwrapper[0][1]=cwrapper[0][2];
			cwrapper[1][1]=cwrapper[1][2];
			cwrapper[2][1]=cwrapper[2][2];
				
			
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

			cwrapper[2][0]=cwrapper[1][0];
			cwrapper[2][1]=cwrapper[1][1];
			cwrapper[2][2]=cwrapper[1][2];
			
			cwrapper[1][0]=cwrapper[0][0];
			cwrapper[1][1]=cwrapper[0][1];
			cwrapper[1][2]=cwrapper[0][2];
			
			
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

			cwrapper[0][0]=cwrapper[1][0];
			cwrapper[0][1]=cwrapper[1][1];
			cwrapper[0][2]=cwrapper[1][2];
			
			cwrapper[1][0]=cwrapper[2][0];
			cwrapper[1][1]=cwrapper[2][1];
			cwrapper[1][2]=cwrapper[2][2];
			
			
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
