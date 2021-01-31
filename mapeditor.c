#include <stdio.h>
#include "main.h"
#include <tcl/tcl.h>
#include <tcl/tk.h>
#include <sqlite3.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <time.h>

view canvas={0};
sqlite3* map=NULL;
Tcl_Interp* interp=NULL;
SDL_Surface* sheetM=NULL;
SDL_Surface* sws=NULL;
SDL_Surface* ws=NULL;
SDL_Surface* efont=NULL;
SDL_Event keyIn;

void image(int index, int x, int y, int w, int h) { //Copies an image from the sprite sheet to an intermediate surface.
	SDL_Rect scaler = {x,y,w,h};
	SDL_Rect sprite={(index % (sheetX/tileSize))*tileSize, (index / (sheetX/tileSize))*tileSize, tileSize, tileSize};
	SDL_BlitScaled(sheetM,&sprite,ws,&scaler);
}

void eChar(int index, int x, int y, int w, int h) {//Draws character from font.png.
	SDL_Rect scaler = {x,y,w,h};
	SDL_Rect sprite={index*7,0,7,7};
	SDL_BlitScaled(efont,&sprite,ws,&scaler);
}

void ePrint(char* string,int x, int y, int w, int h) {
	int i=0;
	while(string[i]) {
		eChar(string[i],x+i*w,y,w,h);
		i++;
	}
	return;
}

int chunkExists(int sX,int sY) {
	char qString[54];
	int rVal=0;
	sqlite3_stmt *statement;

	sprintf(qString,"select count(*) from chunks where x=%d and y=%d",sX,sY);
	sqlite3_prepare(map,qString,-1,&statement,NULL);
	sqlite3_step(statement);
	rVal=sqlite3_column_int(statement,0);
	sqlite3_finalize(statement);
	return rVal;
}

int writeChunk(view in, int sX, int sY) {
	int tileIndex=0;
	char qString[70];
	sqlite3_stmt *statement;
	clock_t start=clock();
	double total;

	Tcl_Eval(interp, ".textScript get 1.0 end");
	strncpy(canvas.script,Tcl_GetStringResult(interp),sizeof canvas.script);
	sprintf(qString,"replace into chunks(x,y,data) values(%d,%d,?)",sX,sY);
	sqlite3_prepare_v2(map,qString,-1,&statement,NULL);
	sqlite3_bind_blob(statement,1,&in,sizeof(view),SQLITE_STATIC);
	sqlite3_step(statement);
	printf(sqlite3_errmsg(map));
	sqlite3_finalize(statement);
	total=(double)(clock()-start)/CLOCKS_PER_SEC;
	printf("Chunk serialised in %lf seconds.\n",total);
}

int loadChunk(view* in, int x, int y) {
	char qString[68];
	sqlite3_stmt *statement;
	int tileIndex=0;
	clock_t start=clock();
	double total;

	if(!in) return 1;
	if(!chunkExists(x,y)) return 0;

	sprintf(qString,"select data from chunks where x=%d and y=%d",x,y);
	sqlite3_prepare(map,qString,-1,&statement,NULL);
	if(sqlite3_step(statement)!=SQLITE_DONE) {
		view* load=(view*)sqlite3_column_blob(statement,0);
		if(!load) return 0;
		*in=*load;
	}
	sqlite3_finalize(statement);
	total=(double)(clock()-start)/CLOCKS_PER_SEC;
	printf("Chunk deserialised in %lf seconds.\n",total);
	return 0;
}

int reloadDb(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	int sX,sY;
	Tcl_Eval(interp, "list $::sX");
	sX=atoi(Tcl_GetStringResult(interp));
	Tcl_Eval(interp, "list $::sY");
	sY=atoi(Tcl_GetStringResult(interp));
	if(loadChunk(&canvas,sX,sY)==0) return 0;
	return 1;
}

int writeDb(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	int sX,sY;
	Tcl_Eval(interp, "list $::sX");
	sX=atoi(Tcl_GetStringResult(interp));
	Tcl_Eval(interp, "list $::sY");
	sY=atoi(Tcl_GetStringResult(interp));
	writeChunk(canvas,sX,sY);
	return 0;
}

int edloop(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	static float scaleFactor=0.0f;
	static char last;
	int heightScale=0;
	int windowW=0;
	int windowH=0;
	int layerSelect=4;
	int tileSelect=0;
	int mouseX,mouseY,tileIndex;
	SDL_Rect outline={SW*TS+TS-2,SH*TS/2,TS+2,TS+1};

	SDL_PollEvent(&keyIn);
	Tcl_Eval(interp,".listLayer curselection");
	layerSelect=atoi(Tcl_GetStringResult(interp));

	if(layerSelect==0 || layerSelect == 3) {
		for(int y=0;y<SH;y++) {
			for(int x=0;x<SW;x++) {
				image(canvas.screen[y][x],x*TS,y*TS,TS,TS);
			}
		}
	}
	if(layerSelect==1 || layerSelect == 3) {
		for(int y=0;y<SH;y++) {
			for(int x=0;x<SW;x++) {
				char text[4];
				sprintf(text,"%u",canvas.layers[y][x]);
				ePrint(text,x*TS,y*TS,7,7);
			}
		}
	}
	if(layerSelect==2 || layerSelect == 3) {
		for(int y=0;y<SH;y++) {
			for(int x=0;x<SW;x++) {
				if(canvas.tScreen[y][x]) image(canvas.tScreen[y][x],x*TS,y*TS,TS,TS);
			}
		}
	}
	if (keyIn.type == SDL_QUIT) exit(0);

	SDL_FillRect(ws,&outline,0xFFFFFF);
	Tcl_Eval(interp,"list $::tileNo");
	tileIndex=atoi(Tcl_GetStringResult(interp));
	for(int i=-SH/2;i<SH/2;i++) {
		if(tileIndex+i>0) image(i+tileIndex,SW*TS+TS-1,SH*TS/2+i*TS,TS,TS);
	}

	if(SDL_GetMouseState(&mouseX,&mouseY)) {
		
		mouseX=(mouseX/scaleFactor)/TS;
		mouseY=(mouseY/scaleFactor)/TS;
		if(mouseX==16 && (last<1 || last > 5)) {
			if(tileIndex-5+mouseY>0) {
				char cmd[32];
				sprintf(cmd,"global tileNo\nset tileNo %d",tileIndex-5+mouseY);
				Tcl_Eval(interp,cmd);
			}
		} else if (mouseX<SW && mouseX >= 0 && mouseY < SH && mouseY >=0) {
			switch(layerSelect) {
				case 0:
				canvas.screen[mouseY][mouseX]=tileIndex;
				break;
				case 1:
				canvas.layers[mouseY][mouseX]=tileIndex;
				break;
				case 2:
				canvas.tScreen[mouseY][mouseX]=tileIndex;
				break;
			}
		}
		last++;
	} else last=0;
	//ePrint("Jordan is a soyboy",0,0,14,14);
	SDL_BlitScaled(ws,NULL,sws,NULL);
	SDL_UpdateWindowSurface(w);
	SDL_FreeSurface(sws);
	SDL_GetWindowSize(w,&windowW,&windowH);
	scaleFactor=(float)windowW/(SW*TS+TS*2);
	heightScale=((SH*TS+TS*2)*scaleFactor);
	SDL_SetWindowSize(w,windowW,heightScale);
	sws=SDL_GetWindowSurface(w);
	SDL_FillRect(ws,NULL,0x000000);
	return 0;
}

int main(int argc, char** argv) {
	int timer=0;
	int tileI=0;
	sqlite3_stmt *statement;
	static int sXo,sYo;
	int sX=0,sY=0;

	sheetM=IMG_Load("sheet.png");
	efont=IMG_Load("font.png");
	

	sqlite3_open("world.map",&map);
	loadChunk(&canvas,0,0);

	SDL_Init(SDL_INIT_VIDEO);
	Tcl_FindExecutable(argv[0]);
	interp = Tcl_CreateInterp(); 
    if (Tcl_Init(interp) != TCL_OK) { 
        printf("Unable to start TCL interpreter, exiting.\n");
        return 1;
    }
    w = SDL_CreateWindow("Drip Map Editor", 0, 0, SW*TS+TS*2, SH*TS+TS*2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    sheetM=IMG_Load("sheet.png");
    ws=SDL_CreateRGBSurface(0, SW*TS+TS*2, SH*TS+TS*2, 32, 0, 0, 0, 0);
    sws=SDL_GetWindowSurface(w);
    efont=SDL_ConvertSurface(efont,sws->format,0);
    SDL_SetColorKey(efont,SDL_TRUE,0x00FF00);
    SDL_SetWindowSize(w,640,480);

    Tcl_CreateCommand(interp, "sdlLoop", (void*)edloop, NULL, NULL);//Registers a callback for the TK event loop to poll.
    Tcl_CreateCommand(interp, "writeDb", (void*)writeDb, NULL, NULL);
    Tcl_CreateCommand(interp, "reloadDb", (void*)reloadDb, NULL, NULL);

    if(Tcl_EvalFile(interp, "mapeditor.tcl") != TCL_OK) {
    	printf("Starting GUI event loop failed!\n");
    	printf(Tcl_GetStringResult(interp));
    }

    Tcl_Eval(interp, "list $::sX");
	sX=atoi(Tcl_GetStringResult(interp));
	Tcl_Eval(interp, "list $::sY");
	sY=atoi(Tcl_GetStringResult(interp));
	if(sX!=sXo || sY!=sYo) {
		memset(&canvas,0,sizeof(canvas));
		loadChunk(&canvas,sX,sY);
		sXo=sX;
		sYo=sY;
	}
    
    Tk_MainLoop();
	return 0;
}