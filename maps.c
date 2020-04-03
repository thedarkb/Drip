view map_box(unsigned int tile, unsigned char layer, unsigned int xIn, unsigned int yIn, unsigned int w, unsigned int h) {
	view me;
	memset(&me,0,sizeof me);
	for(int x=xIn;x<xIn+w;x++) {
		for(int y=yIn;y<yIn+h;y++) {
			if(y<SH && x<SW) {
				me.screen[y][x]=tile;
				me.layers[y][x]=layer;
			}
		}
	}
	return me;
}

view map_interior() {
	printf("Creating interior...");
	view me={
		{
			{5,5,5,5,5,6,0,0,0,0},
			{6,16,16,16,16,6,0,0,0,0},
			{6,16,16,16,16,6,0,0,0,0},
			{6,16,16,16,16,16,0,0,0,0},
			{6,16,16,16,16,6,0,0,0,0},
			{6,16,16,16,16,6,0,0,0,0},
			{5,5,5,5,5,6,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0}
		},
		{
			{1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,1,1,1,1,1},
			{1,0,0,0,0,1,1,1,1,1},
			{1,0,0,0,0,0,1,1,1,1},
			{1,0,0,0,0,1,1,1,1,1},
			{1,0,0,0,0,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1}
		}
	};
	return me;
}

view map_beachbottom() {
	view me={
		{
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29},
			{0,0,0,0,0,0,0,0,0,29}

		},
		{
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0}

		}
	};
	return me;
}

view map_beachtop() {
	view me={
		{
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0},
			{37,0,0,0,0,0,0,0,0,0}

		},
		{
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0}

		}
	};
	return me;
}

view map_beachleft() {
	view me={
		{
			{71,71,71,71,71,71,71,71,71,71},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0}

		},
		{
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0}

		}
	};
	return me;
}

view map_beachright() {
	view me={
		{
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{110,110,110,110,110,110,110,110,110,110}

		},
		{
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0}

		}
	};
	return me;
}

view map_beach(unsigned int xPos, unsigned int yPos) {
	view me;
	memset(&me,0,sizeof me);
	memset(&me.screen,10,sizeof me.screen);
	memset(&me.layers,0,sizeof me.layers);
	uint32_t screenHash=0;
	screenHash |= xPos<<16;
	screenHash |= yPos;
	char up=0;
	char down=0;
	char left=0;
	char right=0;
	if(DIST(xPos,yPos-1,400,400)>40804){
		me=blendMap(me,map_beachbottom());
		down=1;
	}
	if(DIST(xPos,yPos+1,400,400)>40804) {
		me=blendMap(me,map_beachtop());
		up=1;
	}
	if(DIST(xPos-1,yPos,400,400)>40804) {
		me=blendMap(me,map_beachright());
		right=1;
	}
	if(DIST(xPos+1,yPos,400,400)>40804) {
		me=blendMap(me,map_beachleft());
		left=1;
	}
	if(up||left||down||right) {
		if(up&&left) me.screen[0][0]=94;
		if(down&&left) me.screen[0][9]=102;
		if(up&&right) me.screen[14][0]=95;
		if(down&&right) me.screen[14][9]=103;
	} else {
		if(DIST(xPos-1,yPos-1,400,400)>40804) me.screen[14][9]=127;
		if(DIST(xPos+1,yPos+1,400,400)>40804) me.screen[0][0]=118;
		if(DIST(xPos+1,yPos-1,400,400)>40804) me.screen[0][9]=126;
		if(DIST(xPos-1,yPos+1,400,400)>40804) me.screen[14][0]=119;
		for(int x=1;x<SW-1;x++) {
			for(int y=1;y<SH-1;y++) {
				screenHash=lfsr(screenHash);
				if(screenHash&BIT(1)&&screenHash&BIT(2)) me.screen[y][x]=11;
			}
		}
	}
	return me;
}

view map_ridge(uint32_t hash, char in) {
	view me;
	view blend;
	memset(&me,0,sizeof me);
	in*=8;
	me.screen[0][0]=155+in;
	me.layers[0][0]=1;
	int x=1;
	for(;;x++) {
		hash=lfsr(hash);
		if(x>SW-3||hash&BIT(7) && x>3){
			me.screen[0][x]=157+in;
			me.layers[0][x]=1;
			break;
		} else {
			me.screen[0][x]=156+in;
			me.layers[0][x]=1;
		}
	}
	memset(&blend,0,sizeof blend);
	blend=offsetBlendMap(blend,me,SW-2-x,0);
	return blend;
}

view map_ruinedhouse() {
	view me={
		{
			{5,6,6,62,62,6,7,0,0,0,0,0,0,0,0},
			{5,15,0,0,15,0,62,0,0,0,0,0,0,0,0},
			{5,0,0,15,0,0,62,0,0,0,0,0,0,0,0},
			{6,6,6,8,6,62,62,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		},
		{
			{1,1,1,0,0,1,1,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,1,1,0,1,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		}
	};
	return me;
}

view map_dungeonVertical(unsigned int floor, unsigned int wall) {
	
}

view map_snowgrass(uint16_t xIn, uint16_t yIn) {
	view me;
	memset(&me.screen,85,sizeof me.screen);
	memset(&me.layers,0,sizeof me.layers);
	uint32_t screenHash=0;
	screenHash |= xIn<<16;
	screenHash |= yIn;
	screenHash=lfsr(screenHash);
	int eX=(xIn-sX)*(SW*TS);
	int eY=(yIn-sY)*(SH*TS);

	for(int x=0;x<SW;x++) {
		for(int y=0;y<SH;y++) {
			screenHash=lfsr(screenHash);
			if(screenHash&BIT(0)) me.screen[x][y]=11;
			else if(screenHash&BIT(1)) me.screen[x][y]=82;
			else me.screen[x][y]=85;
			if(screenHash & BIT(2) && screenHash & BIT(3) && screenHash & BIT(4) && screenHash & BIT(5)){
				mapEntitySpawn(ent_wall(134),xIn,yIn,TS*x,TS*y);
				me.layers[y][x]=1;
			}
		}
	}
	return me;
}

view map_snowlandBase(uint16_t xIn, uint16_t yIn) {
	view me;
	memset(&me.screen,85,sizeof me.screen);
	memset(&me.layers,0,sizeof me.layers);
	uint32_t screenHash=0;
	screenHash |= xIn<<16;
	screenHash |= yIn;
	screenHash=lfsr(screenHash);
	int eX=(xIn-sX)*(SW*TS);
	int eY=(yIn-sY)*(SH*TS);

	if(screenHash & BIT(2) && screenHash & BIT(1) && screenHash & BIT(3)) {
		screenHash=lfsr(screenHash);
		me=offsetBlendMap(me,map_ruinedhouse(),screenHash%3,screenHash%3);
	} else if(screenHash&BIT(0)){
		screenHash=lfsr(screenHash);
		me=offsetBlendMap(me,map_ridge(screenHash,1),0,screenHash%9);
	}
	for(int x=0;x<SW;x++) {
		for(int y=0;y<SH;y++) {
			screenHash=lfsr(screenHash);
			if(screenHash & BIT(2) && screenHash & BIT(3) && screenHash & BIT(4) && screenHash & BIT(5) && me.screen[y][x]==85) {
				entitySpawn(ent_wall(134),eX+(TS*x),eY+(TS*y));
				me.layers[y][x]=1;
			}
		}
	}
	return me;
}

view map_grasslandBase(uint16_t xIn, uint16_t yIn) {
	view me;
	memset(&me.screen,11,sizeof me.screen);
	memset(&me.layers,0,sizeof me.layers);
	uint32_t screenHash=0;
	screenHash |= xIn<<16;
	screenHash |= yIn;
	screenHash=lfsr(screenHash);
	int eX=(xIn-sX)*(SW*TS);
	int eY=(yIn-sY)*(SH*TS);
	//printf("Wrapper: %d,%d\n",eX,eY);

	for(int x=0;x<SW;x++){
		for(int y=0;y<SH;y++) {
			if(screenHash & BIT(0) && screenHash & BIT(1)) me.screen[y][x]=12;
			screenHash=lfsr(screenHash);
		}
	}
	if(screenHash & BIT(0)) {
		screenHash=lfsr(screenHash);
		me=offsetBlendMap(me,map_ridge(screenHash,0),0,screenHash%SH);
	} else if(screenHash & BIT(2) && screenHash & BIT(1) && screenHash & BIT(3)) {
		screenHash=lfsr(screenHash);
		me=offsetBlendMap(me,map_ruinedhouse(),screenHash%3,screenHash%3);
	}
	for(int x=0;x<SW;x++) {
		for(int y=0;y<SH;y++) {
			if(screenHash & BIT(2) && screenHash & BIT(3) && screenHash & BIT(4) && screenHash & BIT(5) && me.screen[x][y]==11) {
				entitySpawn(ent_wall(30),eX+(TS*x),eY+(TS*y));
				me.layers[y][x]=1;
			}
			screenHash=lfsr(screenHash);
		}
	}
	return me;
}

view map_burren(uint16_t xIn, uint16_t yIn) {
	/*view me;
	memset(&me.screen,15,sizeof me.screen);
	memset(&me.layers,0,sizeof me.layers);
	uint32_t screenHash=0;
	screenHash |= xIn<<16;
	screenHash |= yIn;
	screenHash=lfsr(screenHash);
	for(int x=0;x<SW;x++){
		for(int y=0;y<SH;y++){
			screenHash=lfsr(screenHash);
			if(screenHash&BIT(0)){
				me.screen[x][y]=
			}
		}
	}
	return me;*/
	return map_grasslandBase(xIn,yIn);
}
