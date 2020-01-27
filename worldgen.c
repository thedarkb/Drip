void entFetch(unsigned int xIn, unsigned int yIn) {
	uint32_t screenNum=0;
	screenNum |= sX+(xIn-1);
	screenNum |= sY+(yIn-1) << 16;
	
	int xMult=(xIn-1)*(SW*TS);
	int yMult=(yIn-1)*(SH*TS);
	
	
	if(screenNum==0) {
		entitySpawn(ent_aitest(),xMult+80,yMult+120);
	}
	tilewrapper[xIn][yIn].flag=0;
	//return ent_empty();
}

void loadSpawn() {
	for(int x=0; x<3; x++) {
		for(int y=0; y<3; y++) {
			if (tilewrapper[x][y].flag) {
				entFetch(x,y);
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

	memcpy(&nentSet, &entSet, sizeof nentSet);
	nentityReset();
	
	int diff=intersect(xPos, yPos);
	printf("Difference: %d\n", diff);
	if(xPos<50 && yPos<50) {
		if(diff>=0 && diff < 100) {
			printf("Generated full room.\n");
			memset(&in->screen, 15, sizeof in->screen);
			memset(&in->layers, 0, sizeof in->layers);
			for (int edgePos=1; edgePos<SH-1; edgePos++) {
				screenHash=lfsr(screenHash);
				for(int edgeCount=0; edgeCount<(screenHash>>29); edgeCount++) {
					in->screen[edgeCount][edgePos]=22;
					setCollision(in,edgeCount,edgePos,1);
				}
			}
			for (int edgePos=1; edgePos<SH-1; edgePos++) {
				screenHash=lfsr(screenHash);
				for(int edgeCount=0; edgeCount<(screenHash>>29); edgeCount++) {
					in->screen[SW-edgeCount][edgePos]=22;
					setCollision(in,SW-edgeCount,edgePos,1);
				}
			}
			for (int edgePos=5; edgePos<SW-5; edgePos++) {
				screenHash=lfsr(screenHash);
				for(int edgeCount=0; edgeCount<(screenHash>>29); edgeCount++) {
					in->screen[edgePos][edgeCount]=22;
					setCollision(in,edgePos,edgeCount,1);
				}
			}
			for (int edgePos=5; edgePos<SW-5; edgePos++) {
				screenHash=lfsr(screenHash);
				for(int edgeCount=0; edgeCount<(screenHash>>29); edgeCount++) {
					in->screen[edgePos][SH-edgeCount]=22;
					setCollision(in,edgePos,SH-edgeCount,1);
				}
			}	
		}
		else if(diff>0 && diff<200) {
			printf("Corner room generated.\n");
			//memset(&in->layers, 0, sizeof in->layers);
			*in=cornerRoom;
			//memset(&in->screen, 16, sizeof in->screen);
			//setCollision(in,0,0,0);
		}
		else memset(&in->screen, 22, sizeof in->screen);
	} else memset(&in->screen, 22, sizeof in->screen);
	
	in->flag=1;
	/*if(xPos == 0 && yPos == 0) {
		memcpy(&in->screen, testhouse().tileLayer, sizeof in->screen);
		memcpy(&in->layers, testhouse().collisionLayer, sizeof in->screen);
		entitySpawn(ent_item(120,120,1,255));
		entSet[nspawnSlot-1].health=255;
		//pushMsg("Blobby's Dungeon\0");
		//pushMsg("Level 1:\0");
	}*/
}

void scrollMap() {
	int checkX = entSet[0].x;
	int checkY = entSet[0].y;
	const int speed=(TS*SW)/24;
	SDL_BlitSurface(s, &hudStripper, scrollLayer, NULL);
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
			//SDL_DestroyTexture(bgTex[0][0]);
			bgTex[0][0]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[1][0],sX,sY-1);
			bgDraw(&tilewrapper[1][0]);
			//SDL_DestroyTexture(bgTex[1][0]);
			bgTex[1][0]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[2][0],sX+1,sY-1);
			bgDraw(&tilewrapper[2][0]);
			//SDL_DestroyTexture(bgTex[2][0]);
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
			//SDL_DestroyTexture(bgTex[0][2]);
			bgTex[0][2]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[1][2],sX,sY+1);
			bgDraw(&tilewrapper[1][2]);
			//SDL_DestroyTexture(bgTex[1][2]);
			bgTex[1][2]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[2][2],sX+1,sY+1);
			bgDraw(&tilewrapper[2][2]);
			//SDL_DestroyTexture(bgTex[2][2]);
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
			//SDL_DestroyTexture(bgTex[0][0]);
			bgTex[0][0]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[0][1],sX-1,sY);
			bgDraw(&tilewrapper[0][1]);
			//SDL_DestroyTexture(bgTex[0][1]);
			bgTex[0][1]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[0][2],sX-1,sY+1);
			bgDraw(&tilewrapper[0][2]);
			//SDL_DestroyTexture(bgTex[0][2]);
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
			//SDL_DestroyTexture(bgTex[2][0]);
			bgTex[2][0]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[2][1],sX+1,sY);
			bgDraw(&tilewrapper[2][1]);
			//SDL_DestroyTexture(bgTex[2][1]);
			bgTex[2][1]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			worldgen(&tilewrapper[2][2],sX+1,sY+1);
			bgDraw(&tilewrapper[2][2]);
			//SDL_DestroyTexture(bgTex[2][2]);
			bgTex[2][2]=SDL_CreateTextureFromSurface(r, bgLayer);
			
			//entSet[0].x=-TS+1;
			cameraX=entSet[0].x;
			cameraY=entSet[0].y;
			scroll=0;
		break;			
	}
}
