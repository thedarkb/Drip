view* editorArray[705][705];
unsigned int mapEditorTile=0;

void initMapEditor() {memset(&editorArray,0,sizeof editorArray);}

void mapEditorShim(view* in, int x, int y){
	if(!editorArray[x][y]) {
		editorArray[x][y]=malloc(sizeof tilewrapper[1][1]);
	}
	*editorArray[x][y]=*in;
}

void editorMapExport() {
	if(!editorArray[sX][sY]) return;
	FILE* fileOut=NULL;

	printf("Output Filename: ");
	char filename[64];
	char structName[64];
	memset(&filename,0,sizeof filename);
	scanf("%s", structName);
	sprintf(filename,"%s.c",structName);
	fileOut=fopen(filename, "w");

	fprintf(fileOut, "const view %s={\n", structName);
	fprintf(fileOut,"	{\n");
	for(int y=0;y<SH;y++){
		fprintf(fileOut,"		{");
		for(int x=0;x<SW;x++){
			if(x!=SW-1)fprintf(fileOut,"%u,",(*editorArray[sX][sY]).screen[y][x]);
			else fprintf(fileOut,"%u",(*editorArray[sX][sY]).screen[y][x]);
		}
		if(y!=SH-1) fprintf(fileOut,"},\n");
		else fprintf(fileOut,"}\n");
	}
	fprintf(fileOut,"	},\n	{\n");
	for(int y=0;y<SH;y++){
		fprintf(fileOut,"		{");
		for(int x=0;x<SW;x++){
			if(x!=SW-1)fprintf(fileOut,"%u,",(*editorArray[sX][sY]).layers[y][x]);
			else fprintf(fileOut,"%u",(*editorArray[sX][sY]).layers[y][x]);
		}
		if(y!=SH-1) fprintf(fileOut,"},\n");
		else fprintf(fileOut,"}\n");
	}
	fprintf(fileOut,"	},\n	{\n");
	for(int y=0;y<SH;y++){
		fprintf(fileOut,"		{");
		for(int x=0;x<SW;x++){
			if(x!=SW-1)fprintf(fileOut,"%u,",(*editorArray[sX][sY]).tScreen[y][x]);
			else fprintf(fileOut,"%u",(*editorArray[sX][sY]).tScreen[y][x]);
		}
		if(y!=SH-1) fprintf(fileOut,"},\n");
		else fprintf(fileOut,"}\n");
	}
	fprintf(fileOut,"	}\n");	
	fprintf(fileOut,"};");
	fclose(fileOut);
}

void drawEditorOverlay(){
	static int lastTap=0;
	static int object=0;

	if(!lastTap && keyboard[SDL_SCANCODE_PAGEDOWN] && mapEditorTile<TILECOUNT) mapEditorTile++;
	else if (!lastTap && keyboard[SDL_SCANCODE_PAGEUP] && mapEditorTile>0) mapEditorTile--;
	else if(!lastTap&&keyboard[SDL_SCANCODE_T]) {
		printf("Enter new sX: ");
		scanf("%u", &sX);
		printf("Enter new sY: ");
		scanf("%u", &sY);
		refresh=1;
	}

	if(!lastTap && keyboard[SDL_SCANCODE_M]) editorMapExport();
	if(!lastTap && keyboard[SDL_SCANCODE_SEMICOLON] && object) object=0;
	else if(!lastTap && keyboard[SDL_SCANCODE_SEMICOLON] && !object) object=1;	


	if(keyboard[SDL_SCANCODE_PAGEUP] || keyboard[SDL_SCANCODE_PAGEDOWN] || keyboard[SDL_SCANCODE_M] || keyboard[SDL_SCANCODE_SEMICOLON]) lastTap=1;
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

	drawRect(0,0,SW*TS,TS*2,0);
	cameraX=entSet[0].x;
	cameraY=entSet[0].y;
	emptyRect(entSet[0].x+CX,entSet[0].y+CY,TS,TS,0xFF0000);

	char editorText[255];
	if(object) {
		if(mapEditorTile < FLAVOURCOUNT-1) strcpy(editorText,editorEntityFlavour[mapEditorTile]);
		else {
			strcpy(editorText,"Scroll back up!");
			text(editorText,TS+2,-TS);
			return;
		}
	} else {
		sprintf(editorText,"Tile ID: %u\nGrid Pos: %u,%u\n",mapEditorTile,sX,sY);
	}
	text(editorText,TS+2,-TS);

	if(object && topLevelEntities[mapEditorTile]) hudDraw(hwtileset[topLevelEntities[mapEditorTile](0,0,0,0).frame[1]],0,0,TS,TS);
	else hudDraw(hwtileset[mapEditorTile],0,0,TS,TS);
}