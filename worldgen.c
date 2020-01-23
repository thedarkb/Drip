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
	
	if(diff && xPos<100 && yPos<100) {
		if (diff==1) {
			for (int x=0; x<SW; x++) { //first pass clears the tileset, adds flowers.
				for (int y=0; y<SH; y++) {
					in->screen[x][y] = 15;
					if ((screenHash & BIT(2)) && (screenHash & BIT(5)) && (screenHash & BIT(9))) in->screen[x][y] = 12;
					screenHash = lfsr(screenHash);
					setCollision(in,x,y,0);
				}
			}
		
			for (int x=0; x<SW; x++) { //adds trees
				for (int y=0; y<SH; y++) {
					if ((screenHash & BIT(5)) && (screenHash & BIT(7)) && (screenHash & BIT(10))) {
						in->screen[x][y] = (30);
						setCollision(in,x,y,1);
					}
					screenHash = lfsr(screenHash);
				}
			}
		} else {
			printf("Get else'd");
		}
	} else {
		memset(&in->screen, 22, sizeof in->screen);
	}
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
