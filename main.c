#include "main.h"

#include "draw.c"
#include "entityLogic.h"
#include "tcl.c"
#include "entityLogic.c"
#include "worldgen.c"


unsigned int weightedRand(int i, uint32_t in) {
	if(i<=0) return 0;
	in=lfsr(in);
	if(!(in%4)) return weightedRand(--i,in);
	return i;
}

void entityInitialise() { //Clears entity array, spawns player.
	for (int i=0; i<ELIMIT; i++) {
		memset(&entSet[i], 0, sizeof entSet[i]);
	}
}

void entityScroll(int x, int y) { //Corrects entity positions when player moves to a new area.
	/*for (int i=0; i<ELIMIT; i++) {
		entSet[i].x=entSet[i].x+((SW*TS)*x);
		entSet[i].y=entSet[i].y+((SH*TS)*y);
		if (entSet[i].x < -SW*TS || entSet[i].x > (SW*TS)*2) memset(&entSet[i], 0, sizeof entSet[i]);
		if (entSet[i].y < -SH*TS || entSet[i].y > (SH*TS)*2) memset(&entSet[i], 0, sizeof entSet[i]);
	}*/
}

void mapEntitySpawn(char* in, uint16_t xIn, uint16_t yIn, int x, int y) {
	int eX=(xIn-sX)*(SW*TS);
	int eY=(yIn-sY)*(SH*TS);
	entitySpawn(in,eX+x,eY+y);
}

void entitySpawn(char* in, int x, int y) {
	for(int i=0;i<ELIMIT;i++) {
		if(!entSet[0].state[0]) {
			Tcl_Eval(gameState,in);
			const char* newEnt=Tcl_GetStringResult(gameState);
			memset(&entSet[i],0,sizeof(entSet[i]));
			strcpy(entSet[i].state,newEnt);

			stateSet(entSet[i].state,E_X,x);
			stateSet(entSet[i].state,E_Y,y);

			break;
		}
	}
}


int euclideanDistance(unsigned int i, unsigned int j, unsigned int distance) {
	//if(DIST(entSet[i].x,entSet[i].y, entSet[j].x, entSet[j].y)<distance*distance) return 1;
	return 0;
}

void text(char* inStr, int x, int y) {
	int i=0;
	int xb=x;
	while (inStr[i]) { //Breaks if it hits a null terminator.
		if (inStr[i]>37) image(font[inStr[i]], x, y, 7, 7); //If character is printing, print character.
		x+=8; //Moves the cursor
		if(inStr[i]==10) { //If line feed is detected, reset cursor to left side of the screen.
			y+=10; //And move the cursor down one line.
			x=xb;
		}
		i++;
	}
}

void menu() {
	drawRect(2,74,236,58,0);
	int optCount=0;
	static int optSel=0;

	for(int i=0;menuText[i];) if(menuText[i++]==10) optCount++; //Counts the number of lines in the menu text.

	if(optSel>optCount)optSel=0;
	text(menuText,11,56); //Displays menu text.
	static int keyPress=1;
	static int zPress=1;
	if(!keyboard[SDL_SCANCODE_UP] && !keyboard[SDL_SCANCODE_DOWN]) keyPress=0;

	if(keyboard[SDL_SCANCODE_UP] && !keyPress && optSel>0) {
		keyPress=1;
		optSel--;
	}
	if(keyboard[SDL_SCANCODE_DOWN] && !keyPress && optSel<optCount) {
		keyPress=1;
		optSel++;
	}
	if(!keyboard[SDL_SCANCODE_Z]) {
		zPress=0;
		menuFirstCall=0;
	}
	if(keyboard[SDL_SCANCODE_Z] && !zPress && !menuFirstCall) {
		menuFlag=0;
		zPress=1;
		if(options[optSel]) options[optSel](); //Options are just function pointers.
	}

	text(">", 2, 56+(optSel*10));  //Draws the cursor.
}

void pushMsg(char* inStr) { //Adds a message to the stack.
	mode=1;
	msgBuffer[msgSlot]=inStr;
	msgSlot++;
}

void popMsg(){
	static char keyPressed=0;
	drawRect(2,120,236,58,0); //Background.
	text(msgBuffer[msgOut],2,102); //Renders text
	if (!keyPressed) { //Pops message off of the stack if you hit Z
		if (keyboard[SDL_SCANCODE_Z] && msgTimeout>10) {
			printf("Next slot:\n");
			msgOut++;
			if (msgSlot==msgOut) { //Breaks out of message mode when the bottom of the stack is hit.
				mode=2;
				msgSlot=0;
				msgOut=0;
			}
			msgTimeout=0;
			return;
		}
		if (keyboard[SDL_SCANCODE_Z]) keyPressed=1;
	}
	if(!keyboard[SDL_SCANCODE_Z]) keyPressed=0;
	msgTimeout++;
}

unsigned int get_diff (int val1, int val2) {
	if (val1>val2) return val1-val2;
	else return val2-val1;
	return 0;
}

uint32_t lfsr (uint32_t shift) { //Pseudo-random number generator.
	for (int i=0; i<33; i++) {
		shift ^= (shift >> 31);
		shift ^= (shift >> 31) << 4;
		shift ^= (shift >> 31) << 5;
		shift ^= (shift >> 31) << 30;
		shift = shift << 1;
	}
	return shift >> 1;
}




void loop() {
	#ifdef DEV
	if(keyboard[SDL_SCANCODE_F5]) {
		if(Tcl_EvalFile(gameState,"game.tcl")) {
			printf("game.tcl failed to execute.");
			printf(Tcl_GetStringResult(gameState));
			printf("\n");
		}
	}
	#endif
	
	SDL_SetRenderDrawColor(r,0,0,0,255);
	SDL_RenderClear(r);
	if (scroll) scrollMap();

	if (refresh) {		
		for(int i=1;i<ELIMIT;i++) memset(&entSet[i],0,sizeof entSet[i]);
		memset(&tilewrapper,0,sizeof tilewrapper);
		for (int x=0; x<3; x++) {
			for (int y=0; y<3; y++) {
				worldgen(&tilewrapper[x][y],(sX-1)+x,(sY-1)+y);
				bgDraw(&tilewrapper[x][y]);
			}
		}
		refresh=0;
	}


	// if(!frameTotal) {
	// 	menuReset;
	// 	menuText="Boy\nGirl\0";
	// 	options[0]=diag_boy;
	// 	options[1]=diag_girl;
	// 	menuCall;
	// }

	char xStart=0;
	char yStart=0;
	char xEnd=3;
	char yEnd=3;

	if(cameraX<(TS*SW)/2) xEnd=2;
	else xStart=1;

	if(cameraY<(TS*SH)/2) yEnd=2;
	else yStart=1;

	if(cameraX==(TS*SW)/2) {
		xStart=1;
		xEnd=2;
	}
	if(cameraY==(TS*SH)/2) {
		yStart=1;
		yEnd=2;
	}

	int offsetX=(RESX)/2-cameraX-TS/2;
	int offsetY=(RESY)/2-cameraY-TS/2;
	for(int wx=xStart;wx<xEnd;wx++){
		int worldX=(wx*SW*TS-SW*TS);
		for(int wy=yStart;wy<yEnd;wy++) {
			int worldY=(wy*SH*TS-SH*TS);
			for(int x=0;x<SW;x++) {
				for(int y=0;y<SH;y++) {
					int writeX=(x*TS)+worldX+offsetX;
					int writeY=(y*TS)+worldY+offsetY;
					if(writeX>0-TS && writeX<RESX && writeY>0-TS && writeY<RESY) {
						imageNoTrack(tilewrapper[wx][wy].screen[y][x],writeX,writeY,TS,TS);
					}
				}
			}
		}
	}
	entityLogic();
	if(Tcl_Eval(gameState,"loop")) {
		printf("TCL Game Loop returned non-zero value!\n");
		printf(Tcl_GetStringResult(gameState));
	}

	for(int wx=xStart;wx<xEnd;wx++){
		int worldX=(wx*SW*TS-SW*TS);
		for(int wy=yStart;wy<yEnd;wy++) {
			int worldY=(wy*SH*TS-SH*TS);
			for(int x=0;x<SW;x++) {
				for(int y=0;y<SH;y++) {
					#ifdef DEV
					if(mapEditorEnable && keyboard[SDL_SCANCODE_V]) continue;
					#endif
					int writeX=(x*TS)+worldX+offsetX;
					int writeY=(y*TS)+worldY+offsetY;
					if(writeX>0-TS && writeX<SW*TS && writeY>0-TS && writeY<SH*TS && tilewrapper[wx][wy].tScreen[y][x]){
						imageNoTrack(tilewrapper[wx][wy].tScreen[y][x],writeX,writeY,TS,TS);
					}
				}
			}
		}
	}

	if (mode==1) popMsg();
	else if (menuFlag) menu();
	else mode=0;

	//light();

	#ifndef DEV
	hudRefresh();
	#endif

	// #ifdef DEV
	// if(mapEditorEnable)drawEditorOverlay();
	// else hudRefresh();
	// #endif

	SDL_RenderPresent(r);
	//cameraX=entSet[0].x;
	//cameraY=entSet[0].y;

	if(animationG<30) animationG+=2;
	else animationG=0;
	
	frameTotal++;
	//assert(frameTotal<300);
}

int main (int argc, char** argv) {
	memset(&emptyView,0,sizeof emptyView);
	SDL_Init(SDL_INIT_VIDEO);
	Tcl_FindExecutable(argv[0]); 
    gameState = Tcl_CreateInterp(); 
    if (Tcl_Init(gameState) != TCL_OK) { 
        printf("Unable to start TCL interpreter, exiting.\n");
        return 1;
    }
	#ifdef WEB
	w = SDL_CreateWindow(TITLE, 0, 0, SW*TS*2, (SH*TS+HUDHEIGHT)*2, SDL_WINDOW_OPENGL);
	#else
	w = SDL_CreateWindow(TITLE, 0, 0, SW*TS*3, (SH*TS+HUDHEIGHT)*3, SDL_WINDOW_OPENGL);
	#endif
	r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(r, TS*SW,TS*SH+HUDHEIGHT);
	#ifdef WEB
	SDL_RenderSetScale(r,2,2);
	#else
	SDL_RenderSetScale(r,4,4);
	#endif
	SDL_RenderSetClipRect(r,&clipRect);
	SDL_SetRenderDrawBlendMode(r,SDL_BLENDMODE_BLEND);
	s = SDL_GetWindowSurface(w);
	rng.ui32=4; //SEEDS THE MAIN RNG
	//initialiseFactions();
	tunnels[0].m=1;
	tunnels[0].c=0;
	memset(&tilewrapper, 0, sizeof tilewrapper);
	lOverlay=SDL_CreateRGBSurface(0,SW*TS,SH*TS,32,0,0,0,0);
	SDL_SetSurfaceBlendMode(lOverlay,SDL_BLENDMODE_NONE);

	keyboard = SDL_GetKeyboardState(NULL);

	loader = IMG_Load("sheet.png"); //tilesheet
	sheet=SDL_CreateTextureFromSurface(r,loader);
	SDL_FreeSurface(loader);

	#ifdef DEV
	//initMapEditor();
	#endif



	unsigned int timer=0;
	memset(&entSet, 0, sizeof entSet); //Zeroes out the entity table.
	entityInitialise(); //Loads the player
	memset(&tilewrapper[1][1],0,sizeof tilewrapper[1][1]); //Resets the player's spawn area
	memset(&tilewrapper,0,sizeof tilewrapper);
	entityInitialise();
	registerCommands();

	Tcl_LinkVar(gameState, "cameraX", (void*)&cameraX, TCL_LINK_INT);
	Tcl_LinkVar(gameState, "cameraY", (void*)&cameraY, TCL_LINK_INT);
	Tcl_LinkVar(gameState, "sheetX", (void*)&sheetX, TCL_LINK_INT);
	Tcl_LinkVar(gameState, "sheetY", (void*)&sheetY, TCL_LINK_INT);
	Tcl_LinkVar(gameState, "tileSize", (void*)&tileSize, TCL_LINK_INT);

	if(Tcl_EvalFile(gameState,"game.tcl")) {
		printf("game.tcl failed to execute.");
		printf(Tcl_GetStringResult(gameState));
		printf("\n");
		#ifndef DEV
		return 1;
		#endif
	}

	if(Tcl_Eval(gameState,"setup")) {
		printf("TCL Setup returned non-zero value!\n");
		printf(Tcl_GetStringResult(gameState));
	}

	entitySpawn("testEntSpawn",120,80);

	#ifndef WEB
	while(1) { //The timing loop leaves a little to be desired.
		if (SDL_GetTicks()-timer < 1000/FRAMERATE && !keyboard[SDL_SCANCODE_P]) SDL_Delay(1000/FRAMERATE);
		else printf("Frames dropped: %u\n", (SDL_GetTicks()-timer)/(1000/FRAMERATE)+1);
		timer = SDL_GetTicks();
		SDL_PollEvent(&keyIn);
		if (keyIn.type == SDL_QUIT) break;
		loop();
	}
	#endif
	#ifdef WEB
	emscripten_set_main_loop(loop, 30, 1); //For those of you on WebOS
	#endif
	return 0;
}