view* editorArray[705][705];
unsigned int mapEditorTile=0;

void initMapEditor() {memset(&editorArray,0,sizeof editorArray);}

void mapEditorShim(view* in, int x, int y){
	if(!editorArray[x][y]) {
		editorArray[x][y]=malloc(sizeof tilewrapper[1][1]);
	}
	*editorArray[x][y]=*in;
	editorArray[x][y]->sX=x;
	editorArray[x][y]->sY=y;
}

void editorMapBinExport() {
	if(!editorArray[sX][sY]) return;
	view output=*editorArray[sX][sY];
	output.flag=1;
	FILE* fileOut=NULL;
	view testRead;

	char filename[64];
	char structName[64];
	memset(&filename,0,sizeof filename);
	fileOut=fopen(WORLDFILENAME, "rb+");
	while(fread(&testRead, sizeof(view), 1, fileOut)) {//Hunts down a record for this area of the world.
		if(testRead.sX==sX && testRead.sY==sY) {
			printf("Overwriting pre-existing entry at %u,%u.\n",output.sX,output.sY);
			fseek(fileOut, -sizeof (view), SEEK_CUR);//If found, move the cursor back by one record.
			fwrite(&output, sizeof(view), 1, fileOut);//Overwrites old record with new record.
			fclose(fileOut);
			return;
		}
	}
	printf("Creating new entry at %u,%u.\n",output.sX,output.sY);
	fwrite(&output, sizeof (view), 1, fileOut);//If no record is found by the above loop, append a new one to the EOF
	fclose(fileOut);
}

void drawEditorOverlay(){
	static int lastTap=0;
	static int object=0;
	static int objectCounter=0;
	static int entitySlot=0;
	char over[64]="";
	int wEntSlot=0;

	
	if(mapEditorEnable && ((entSet[0].x+TS/2)/TS<SW&&(entSet[0].y+TS/2)/TS<SH)) {
		if(keyboard[SDL_SCANCODE_SPACE] && !object) {	 
			tilewrapper[1][1].screen[(entSet[0].y+TS/2)/TS][(entSet[0].x+TS/2)/TS]=mapEditorTile;
			mapEditorShim(&tilewrapper[1][1],sX,sY);
			refresh=1;
		} else if(keyboard[SDL_SCANCODE_SPACE] && object) {
			for(int i=0;i<MAPELIMIT;i++){
				if(!tilewrapper[1][1].preSpawns[i].id) {
					tilewrapper[1][1].preSpawns[i].id=objectCounter;
					tilewrapper[1][1].preSpawns[i].x=entSet[0].x;
					tilewrapper[1][1].preSpawns[i].y=entSet[0].y;
					printf("Enter the first spawn argument: ");
					scanf("%d", &tilewrapper[1][1].preSpawns[i].a1);
					printf("Enter the second spawn argument: ");
					scanf("%d", &tilewrapper[1][1].preSpawns[i].a2);
					printf("Enter the third spawn argument: ");
					scanf("%d", &tilewrapper[1][1].preSpawns[i].a3);
					printf("Enter the fourth spawn argument: ");
					scanf("%d", &tilewrapper[1][1].preSpawns[i].a4);
					goto fullBreak;
				}
			}
			pushMsg("Entity array full!");
			fullBreak:
			mapEditorShim(&tilewrapper[1][1],sX,sY);
			refresh=1;
		}
		if(keyboard[SDL_SCANCODE_RETURN] && entSet[0].x+TS/2<SW*TS && entSet[0].y+TS/2<SH*TS) {
			tilewrapper[1][1].tScreen[(entSet[0].y+TS/2)/TS][(entSet[0].x+TS/2)/TS]=mapEditorTile;
			mapEditorShim(&tilewrapper[1][1],sX,sY);
			refresh=1;
		}
		if(keyboard[SDL_SCANCODE_COMMA] && entSet[0].x+TS/2<SW*TS && entSet[0].y+TS/2<SH*TS) {
			tilewrapper[1][1].layers[(entSet[0].y+TS/2)/TS][(entSet[0].x+TS/2)/TS]=1;
			mapEditorShim(&tilewrapper[1][1],sX,sY);
			refresh=1;
		}
		if(keyboard[SDL_SCANCODE_PERIOD] && entSet[0].x+TS/2<SW*TS && entSet[0].y+TS/2<SH*TS) {
			tilewrapper[1][1].layers[(entSet[0].y+TS/2)/TS][(entSet[0].x+TS/2)/TS]=0;
			mapEditorShim(&tilewrapper[1][1],sX,sY);
			refresh=1;
		}
		if(keyboard[SDL_SCANCODE_F10]) {
			memset(&tilewrapper[1][1],0,sizeof tilewrapper[1][1]);
			mapEditorShim(&tilewrapper[1][1],sX,sY);
			refresh=1;
		}
	}

	if(!object) {
		if(!lastTap && keyboard[SDL_SCANCODE_PAGEDOWN] && mapEditorTile<TILECOUNT) mapEditorTile++;
		else if (!lastTap && keyboard[SDL_SCANCODE_PAGEUP] && mapEditorTile>0) mapEditorTile--;
	} else {
		if(!lastTap && keyboard[SDL_SCANCODE_PAGEDOWN] && objectCounter<ENTCOUNT-1) objectCounter++;
		else if (!lastTap && keyboard[SDL_SCANCODE_PAGEUP] && objectCounter>0) objectCounter--;
	}

	if(!lastTap&&keyboard[SDL_SCANCODE_T]) {
		printf("Enter new sX: ");
		scanf("%u", &sX);
		printf("Enter new sY: ");
		scanf("%u", &sY);
		refresh=1;
	}

	if(!lastTap && keyboard[SDL_SCANCODE_M]) editorMapBinExport();
	if(!lastTap && keyboard[SDL_SCANCODE_F9]) for(int i=1;i<ELIMIT;i++) memset(&entSet[i],0,sizeof entSet[i]);
	if(!lastTap && keyboard[SDL_SCANCODE_F8]) {refresh=1;tilewrapper[1][1].flag=1;}
	if(!lastTap && keyboard[SDL_SCANCODE_SEMICOLON] && object) object=0;
	else if(!lastTap && keyboard[SDL_SCANCODE_SEMICOLON] && !object) object=1;	


	if(keyboard[SDL_SCANCODE_PAGEUP] || keyboard[SDL_SCANCODE_PAGEDOWN] || keyboard[SDL_SCANCODE_M]
	 || keyboard[SDL_SCANCODE_SEMICOLON] || keyboard[SDL_SCANCODE_F9] || keyboard[SDL_SCANCODE_F8]) lastTap=1;
	else lastTap=0;

	if(keyboard[SDL_SCANCODE_RIGHTBRACKET]&&mapEditorTile<TILECOUNT-1)mapEditorTile+=2;
	if(keyboard[SDL_SCANCODE_LEFTBRACKET]&&mapEditorTile>1)mapEditorTile-=2;

	for(int x=0;x<SW;x++) {
		for(int y=0;y<SH;y++) {
			if(!keyboard[SDL_SCANCODE_B]){
				if(tilewrapper[1][1].layers[y][x]) drawRectAlphaTrack(x*TS, y*TS, TS, TS, 0, 32);
				else drawRectAlphaTrack(x*TS, y*TS, TS, TS, 0xFFFFFF, 32);
			} else {
				if(tilewrapper[1][1].layers[y][x]) drawRectAlphaTrack(x*TS, y*TS, TS, TS, 0, 255);
				else drawRectAlphaTrack(x*TS, y*TS, TS, TS, 0xFFFFFF, 255);
			}
		}
	}
	for(int i=0;i<MAPELIMIT;i++){
		if(!object) break;
		if(tilewrapper[1][1].preSpawns[i].id) {
			drawRectAlphaTrack(tilewrapper[1][1].preSpawns[i].x,tilewrapper[1][1].preSpawns[i].y,TS,TS,0xFF0000,32);
		}
	}

	drawRect(0,0,SW*TS,TS*2,0);
	cameraX=entSet[0].x;
	cameraY=entSet[0].y;
	emptyRect(entSet[0].x+CX,entSet[0].y+CY,TS,TS,0xFF0000);

	if(object){
		for(int i=0;i<MAPELIMIT;i++){
			if(get_diff(entSet[0].x,tilewrapper[1][1].preSpawns[i].x) < TS && get_diff(entSet[0].y,tilewrapper[1][1].preSpawns[i].y) < TS
			 && tilewrapper[1][1].preSpawns[i].id) {
				strcpy(over,editorEntityFlavour[tilewrapper[1][1].preSpawns[i].id]);
				if(keyboard[SDL_SCANCODE_DELETE]) tilewrapper[1][1].preSpawns[i].id=0;
				mapEditorShim(&tilewrapper[1][1],sX,sY);
			}
		}
	}
	

	char editorText[255];
	if(object) {
		if(objectCounter < ENTCOUNT) sprintf(editorText,"%s\nOVER: %s",editorEntityFlavour[objectCounter],over);
		else {
			strcpy(editorText,"Scroll back up!");
		}
	} else {
		sprintf(editorText,"Tile ID: %u\nGrid Pos: %u,%u\n",mapEditorTile,sX,sY);
	}
	text(editorText,TS+2,-TS);

	if(object && topLevelEntities[objectCounter]) hudDraw(hwtileset[topLevelEntities[objectCounter](0,0,0,0).frame[1]],0,0,TS,TS);
	else hudDraw(hwtileset[mapEditorTile],0,0,TS,TS);

	//if(editorArray[sX][sY]) *editorArray[sX][sY]=tilewrapper[1][1];
}