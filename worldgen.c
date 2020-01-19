void worldgen(uint16_t xPos, uint16_t yPos) {
	union hashCom {
		uint16_t x;
		uint16_t y;
		uint32_t hashme;
	} hashCom;

	hashCom.x = xPos;
	hashCom.y = yPos;

	uint32_t screenHash = lfsr(hashCom.hashme);

	printf("%u\n",hashCom.hashme);

	memcpy(&nentSet, &entSet, sizeof nentSet);
	nentityReset();

	for (int x=0; x<SW; x++) { //first pass clears the tileset, adds flowers.
		for (int y=0; y<SH; y++) {
			nScreen[x][y] = 11; //shifts left to account for the collision bit.
			if ((screenHash & BIT(2)) && (screenHash & BIT(5)) && (screenHash & BIT(9))) nScreen[x][y] = 12;
			screenHash = lfsr(screenHash);
			setCollision(x,y,0);
		}
	}

	for (int x=0; x<SW; x++) { //adds trees
		for (int y=0; y<SH; y++) {
			if ((screenHash & BIT(5)) && (screenHash & BIT(7)) && (screenHash & BIT(10))) {
				nScreen[x][y] = (30);
				setCollision(x,y,1);
			}
			screenHash = lfsr(screenHash);
		}
	}
	if(xPos == 0 && yPos == 0) {
		memcpy(&nScreen, testhouse().tileLayer, sizeof nScreen);
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
		sY--;
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
				for (int i=0; i<SH*TS; i+=speed) {
					if(entSet[0].y<TS*SH-TS) entSet[0].y=entSet[0].y+5;
					simage(bgLayer,0,i-SH*TS,SW*TS,SH*TS);
					simage(scrollLayer,0,i,SW*TS,SH*TS);
					hudRefresh();
					wait();
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
		worldgen(sX,sY);
		checkY=5;
		if (nlayers[checkX][checkY] == 0 && nlayers[checkX+TS/2][checkY] == 0 && nlayers[checkX+TS][checkY] == 0 ) {
			entSet[0].x=checkX;
			memcpy(&cScreen, &nScreen, sizeof nScreen);
			memcpy(&layers, &nlayers, sizeof nlayers);
			memcpy(&entSet, &nentSet, sizeof nentSet);
			spawnSlot=nspawnSlot;
			bgDraw();
			entSet[0].y=1;
		}
		else sY--;
		scroll=0;
		break;
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
		break;
	}
	SDL_DestroyTexture(bgTex[1][1]);
	bgTex[1][1]=SDL_CreateTextureFromSurface(r, bgLayer);
}
