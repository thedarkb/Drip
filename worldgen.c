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
		memset(in,0,sizeof tilewrapper[1][1]);
	}
	in->flag=1;
}

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

			for(int i=0;i<3;i++) tilewrapper[i][2]=tilewrapper[i][1];
			for(int i=0;i<3;i++) tilewrapper[i][1]=tilewrapper[i][0];
			
			worldgen(&tilewrapper[0][0],sX-1,sY-1);			
			worldgen(&tilewrapper[1][0],sX,sY-1);			
			worldgen(&tilewrapper[2][0],sX+1,sY-1);
			
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
			
			worldgen(&tilewrapper[0][2],sX-1,sY+1);
			worldgen(&tilewrapper[1][2],sX,sY+1);
			worldgen(&tilewrapper[2][2],sX+1,sY+1);

			
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

			worldgen(&tilewrapper[0][0],sX-1,sY-1);
			worldgen(&tilewrapper[0][1],sX-1,sY);	
			worldgen(&tilewrapper[0][2],sX-1,sY+1);
			
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

			worldgen(&tilewrapper[2][0],sX+1,sY-1);
			worldgen(&tilewrapper[2][1],sX+1,sY);
			worldgen(&tilewrapper[2][2],sX+1,sY+1);
			
			//entSet[0].x=-TS+1;
			cameraX=entSet[0].x;
			cameraY=entSet[0].y;
			scroll=0;
		break;			
	}
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
	view interior={
		{
			{5,6,6,6,6,6,7,0,0,0,0,0,0,0,0},
			{5,8,8,8,8,8,7,0,0,0,0,0,0,0,0},
			{7,8,8,8,8,8,7,0,0,0,0,0,0,0,0},
			{7,8,8,8,8,8,7,0,0,0,0,0,0,0,0},
			{7,8,8,8,8,8,7,0,0,0,0,0,0,0,0},
			{6,6,6,8,6,6,6,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		},
		{
			{1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
			{1,1,1,0,1,1,1,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		},
		{
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,6,6,48,6,6,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		}
	};
	*in=interior;
	mapEntitySpawn(ent_door(48,300,300,64,144),xPos,yPos,48,80);
	mapEntitySpawn(ent_npc(),xPos,yPos,48,32);
}

void axiomLoad(){
	memset(&mapLoader,0,sizeof mapLoader);
	mapLoader[300][300]=ax_startPad;
	mapLoader[607][305]=ax_testhouse;
}