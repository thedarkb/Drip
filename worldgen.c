void worldgen(uint16_t xPos, uint16_t yPos) {
	uint32_t hashme;
	uint16_t* hashCom = &hashme;

	*hashCom = xPos;
	hashCom++;
	*hashCom = yPos;

	uint32_t screenHash = lfsr(hashme);

	printf("%u\n",hashme);

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
			if ((screenHash & BIT(5)) && (screenHash & BIT(7))) {
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
	}
}

void scrollMap() {
	int checkX = entSet[0].x;
	int checkY = entSet[0].y;
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
				for (int i=0; i<SH*TS; i+=5) {
					if(entSet[0].y<TS*SH-TS) entSet[0].y=entSet[0].y+5;
					simage(bgLayer,0,i-SH*TS,SW*TS,SH*TS);
					simage(scrollLayer,0,i,SW*TS,SH*TS);
					hudRefresh();
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
			#ifndef NOSCROLL
				for (int i=SH*TS; i>0; i=i-5) {
					if(entSet[0].y>0) entSet[0].y=entSet[0].y-5;
					simage(bgLayer,0,i,SW*TS,SH*TS);
					simage(scrollLayer,0,i-SH*TS,SW*TS,SH*TS);
					hudRefresh();
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
		worldgen(sX,sY);
		checkX=TS*SW-5;
		if (nlayers[checkX][checkY] == 0 && nlayers[checkX][checkY+TS/2] == 0 && nlayers[checkX][checkY+TS] == 0 ) {
			memcpy(&cScreen, &nScreen, sizeof nScreen);
			memcpy(&layers, &nlayers, sizeof nlayers);
			memcpy(&entSet, &nentSet, sizeof nentSet);
			spawnSlot=nspawnSlot;
			bgDraw();
			#ifndef NOSCROLL
				for (int i=0; i<SW*TS; i+=5) {
					if(entSet[0].x<SW*TS-TS) entSet[0].x+=5;
					simage(bgLayer,i-SW*TS,0,SW*TS,SH*TS);
					simage(scrollLayer,i,0,SW*TS,SH*TS);
					hudRefresh();
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
				for (int i=SW*TS; i>0; i-=5) {
					if(entSet[0].x>0) entSet[0].x-=5;
					simage(bgLayer,i,0,SW*TS,SH*TS);
					simage(scrollLayer,i-SW*TS,0,SW*TS,SH*TS);
					hudRefresh();
					flip();
				}
			#endif
			entSet[0].x=1;
		}
		else sX--;
		scroll=0;
		break;
	}
	SDL_DestroyTexture(bgTex);
	bgTex=SDL_CreateTextureFromSurface(r, bgLayer);
}