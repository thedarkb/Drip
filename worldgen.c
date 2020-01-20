void worldgen(view* in, uint16_t xPos, uint16_t yPos) {
	union hashCom {
		uint16_t x;
		uint16_t y;
		uint32_t hashme;
	} hashCom;
	uint32_t hashme=0;

	hashme |= xPos;
	hashme |= yPos << 16;

	uint32_t screenHash = lfsr(hashme);

	//printf("%u\n",hashme);

	memcpy(&nentSet, &entSet, sizeof nentSet);
	nentityReset();

	for (int x=0; x<SW; x++) { //first pass clears the tileset, adds flowers.
		for (int y=0; y<SH; y++) {
			in->screen[x][y] = 11; //shifts left to account for the collision bit.
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
	if(xPos == 0 && yPos == 0) {
		memcpy(&in->screen, testhouse().tileLayer, sizeof in->screen);
		nentitySpawn(ent_item(120,120,1,255));
		nentSet[nspawnSlot-1].health=255;
		//pushMsg("Blobby's Dungeon\0");
		//pushMsg("Level 1:\0");
	}
}

void scrollMap() {
	int checkX = entSet[0].x;
	int checkY = entSet[0].y;
	const int speed=(TS*SW)/24;
	SDL_BlitSurface(s, &hudStripper, scrollLayer, NULL);
	switch (scroll) {
		case 1:
		checkY=TS*SH-5;
		if (tilewrapper[1][0].layers[checkX][checkY] == 0 && tilewrapper[1][0].layers[checkX+TS/2][checkY] == 0 && tilewrapper[1][0].layers[checkX+TS][checkY] == 0 ) {
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
			
			entityReset();
			entSet[0].x=checkX;
			entSet[0].y=TS*SH-TS-1;
			cameraX=entSet[0].x;
			cameraY=entSet[0].y;
		}
		scroll=0;
		break;
		case 2:
		printf("Attempting to downscroll.\n");
		checkY=5;
		if (tilewrapper[1][2].layers[checkX][checkY] == 0 && tilewrapper[1][2].layers[checkX+TS/2][checkY] == 0 && tilewrapper[1][2].layers[checkX+TS][checkY] == 0 ) {
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
			
			entityReset();
			entSet[0].x=checkX;
			entSet[0].y=1;
			cameraX=entSet[0].x;
			cameraY=entSet[0].y;
		}
		scroll=0;
		break;/*
		case 3:
		sX--;
		worldgen(sX,sY);
		checkX=TS*SW-5;
		if (nlayers[checkX][checkY] == 0 && nlayers[checkX][checkY+TS/2] == 0 && nlayers[checkX][checkY+TS] == 0 ) {
			memcpy(&cScreen, &nScreen, sizeof nScreen);
			memcpy(&layers, &nlayers, sizeof nlayers);
			memcpy(&entSet, &nentSet, sizeof nentSet);
			spawnSlot=nspawnSlot;
			bgDraw();
			entSet[0].x=TS*SW-TS;
		}
		else sX++;
		scroll=0;
		break;
		case 4:
		sX++;
		worldgen(sX,sY);
		checkX=5;
		if (nlayers[checkX][checkY] == 0 && nlayers[checkX][checkY+TS/2] == 0 && nlayers[checkX][checkY+TS] == 0 ) {
			//entSet[0].x=checkX;
			memcpy(&cScreen, &nScreen, sizeof nScreen);
			memcpy(&layers, &nlayers, sizeof nlayers);
			memcpy(&entSet, &nentSet, sizeof nentSet);
			spawnSlot=nspawnSlot;
			bgDraw();
			entSet[0].x=1;
		}
		else sX--;
		scroll=0;
		break;*/
	}
}
