

void generateDungeons() {
	memset(&dungeonRooms,0,sizeof dungeonRooms);
	int state=lfsr(SEED);
	for(int x=0;x<WORLDWIDTH;x++) {//Creates the initial cells from which the dungeon will grow.
		for(int y=0;y<WORLDHEIGHT;y++) {
			state=lfsr(state);
			if(state % 3 == 0) {
				dungeonRooms[x][y]=1;
				state=lfsr(state);
				if(state%10 == 0) dungeonRooms[x][y]|=(1<<1);
			}
		}
	}
	for(int x=1;x<WORLDWIDTH-1;x++) {
		for(int y=1;y<WORLDHEIGHT-1;y++) {
			if(dungeonRooms[x][y]) {
				int neighbours=0;
				if(dungeonRooms[x-1][y]) neighbours++;
				if(dungeonRooms[x+1][y]) neighbours++;
				if(dungeonRooms[x][y-1]) neighbours++;
				if(dungeonRooms[x][y+1]) neighbours++;

				while(neighbours<2) {
					state=lfsr(state);
					if(state%4==0 && !dungeonRooms[x-1][y]) {
						dungeonRooms[x-1][y]=1;
						neighbours++;
					}
					if(state%4==1 && !dungeonRooms[x+1][y]) {
						dungeonRooms[x+1][y]=1;
						neighbours++;
					}
					if(state%4==2 && !dungeonRooms[x][y-1]) {
						dungeonRooms[x][y-1]=1;
						neighbours++;
					}
					if(state%4==3 && !dungeonRooms[x][y+1]) {
						dungeonRooms[x][y+1]=1;
						neighbours++;
					}
				}
			}
		}
	}
}

void axiomLoad(){
	memset(mapLoader,0,sizeof mapLoader);
	view inMap;
	FILE* inFile=NULL;
	if(!(inFile=fopen(WORLDFILENAME, "rb"))) {
		printf(WORLDFILENAME);
		printf(" not found, exiting.\n");
		exit(1);
	}
	while(fread(&inMap, sizeof inMap, 1, inFile)) {
		printf("Map segment found with sX:%u and sY:%u.\n", inMap.sX, inMap.sY);
		mapLoader[inMap.sX][inMap.sY]=malloc(sizeof inMap);
		*mapLoader[inMap.sX][inMap.sY]=inMap;
	}
}

void entFetch(unsigned int xIn, unsigned int yIn) {
	uint32_t screenNum=0;
	screenNum |= sX+(xIn-1);
	screenNum |= sY+(yIn-1) << 16;
	
	int xMult=(xIn-1)*(SW*TS);
	int yMult=(yIn-1)*(SH*TS);
	int xOffset=(xIn-1);
	int yOffset=(yIn-1);

	printf("EntFetch entered...\n");

	/*for(int i=0;i<MAPELIMIT;i++) {
		if(!tilewrapper[xIn][yIn].preSpawns[i].id) continue;
		printf("Entity found, spawning at %u,%u\n",tilewrapper[xIn][yIn].sX,tilewrapper[xIn][yIn].sY);
		mapEntitySpawn(topLevelEntities[tilewrapper[xIn][yIn].preSpawns[i].id](
			tilewrapper[xIn][yIn].preSpawns[i].a1,
			tilewrapper[xIn][yIn].preSpawns[i].a2,
			tilewrapper[xIn][yIn].preSpawns[i].a3,
			tilewrapper[xIn][yIn].preSpawns[i].a4),
			tilewrapper[xIn][yIn].sX,
			tilewrapper[xIn][yIn].sY,
			tilewrapper[xIn][yIn].preSpawns[i].x,
			tilewrapper[xIn][yIn].preSpawns[i].y);
	}

	tilewrapper[xIn][yIn].flag=0;*/
}

void loadSpawn() {
	for(int x=0; x<3; x++) {
		for(int y=0; y<3; y++) {
			if (tilewrapper[x][y].flag) {
				entFetch(x,y);
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
	if(xPos<WORLDWIDTH && yPos<WORLDHEIGHT) {
		#ifdef DEV
		//if(editorArray[xPos][yPos]) {
		//	*in=*editorArray[xPos][yPos];
		//	in->flag=1;
		//	return;
		//}
		#endif

		if(mapLoader[xPos][yPos]) {
			*in=*mapLoader[xPos][yPos];
			in->flag=1;
			return;
		}
	}
	//memset(in,0,sizeof tilewrapper[1][1]);
	//return;

	//printf("Difference: %d\n", diff);

	memset(in,0,sizeof(*in));
	memset(in->screen,11,sizeof tilewrapper[1][1].screen);

	in->flag=0;
}

void scrollMap() {
	int checkX = stateGet(entSet[0].state, E_X);
	int checkY = stateGet(entSet[0].state, E_Y);
	//collisionReset=1;
	const int speed=(TS*SW)/24;
	switch (scroll) {
		case 1:
			entityScroll(0,1);
			
			checkY=TS*SH-5;
			sY--;

			for(int i=0;i<3;i++) tilewrapper[i][2]=tilewrapper[i][1];
			for(int i=0;i<3;i++) tilewrapper[i][1]=tilewrapper[i][0];
			
			worldgen(&tilewrapper[0][0],sX-1,sY-1);			
			worldgen(&tilewrapper[1][0],sX,sY-1);			
			worldgen(&tilewrapper[2][0],sX+1,sY-1);
			
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
			
			worldgen(&tilewrapper[0][2],sX-1,sY+1);
			worldgen(&tilewrapper[1][2],sX,sY+1);
			worldgen(&tilewrapper[2][2],sX+1,sY+1);

			
			//entSet[0].x=checkX;
			//entSet[0].y=1;
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

			worldgen(&tilewrapper[0][0],sX-1,sY-1);
			worldgen(&tilewrapper[0][1],sX-1,sY);	
			worldgen(&tilewrapper[0][2],sX-1,sY+1);
			
			//entSet[0].x=TS*SW-TS;
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

			worldgen(&tilewrapper[2][0],sX+1,sY-1);
			worldgen(&tilewrapper[2][1],sX+1,sY);
			worldgen(&tilewrapper[2][2],sX+1,sY+1);
			
			//entSet[0].x=-TS+1;
		break;
		scroll=0;
		cameraX=stateGet(entSet[0].state, E_X);
		cameraY=stateGet(entSet[0].state, E_Y);
	}
}

