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
SDL_Surface* ws=NULL;
SDL_Event keyIn;

view map_beachright={
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,110}
	},
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	}
};

void image(int index, int x, int y, int w, int h) { //Copies an image from the hardware buffer to the screen.
	SDL_Rect scaler = {x*4,y*4,w*4,h*4}; //Accounts for HUD and camera.
	SDL_Rect sprite={(index % (sheetX/tileSize))*tileSize, (index / (sheetX/tileSize))*tileSize, tileSize, tileSize};
	SDL_BlitScaled(sheetM,&sprite,ws,&scaler);
}

int edloop(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	int layerSelect=4;

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
				//image(canvas.layers[y][x],x*TS,y*TS,TS,TS);
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

	SDL_UpdateWindowSurface(w);
	SDL_FillRect(ws,NULL,0x000000);
	return 0;
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
	char qString[52];
	sqlite3_stmt *statement;
	clock_t start=clock();
	double total;

	for(int y=0;y<SH;y++)
		for(int x=0;x<SW;x++) {
			sprintf(qString,"replace into chunks values(%d,%d,%d,%d)",tileIndex++,in.screen[y][x],sX,sY);
			sqlite3_prepare(map,qString,-1,&statement,NULL);
			sqlite3_step(statement);
			sqlite3_finalize(statement);
		}
	for(int y=0;y<SH;y++)
		for(int x=0;x<SW;x++) {
			sprintf(qString,"replace into chunks values(%d,%d,%d,%d)",tileIndex++,in.layers[y][x],sX,sY);
			sqlite3_prepare(map,qString,-1,&statement,NULL);
			sqlite3_step(statement);
			sqlite3_finalize(statement);
		}
	for(int y=0;y<SH;y++)
		for(int x=0;x<SW;x++) {
			sprintf(qString,"replace into chunks values(%d,%d,%d,%d)",tileIndex++,in.tScreen[y][x],sX,sY);
			sqlite3_prepare(map,qString,-1,&statement,NULL);
			sqlite3_step(statement);
			sqlite3_finalize(statement);
		}
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

	sprintf(qString,"select tiles from chunks where x=%d and y=%d order by tileN",x,y);
	sqlite3_prepare(map,qString,-1,&statement,NULL);
	if(!statement) return 2;
	while(sqlite3_step(statement)!=SQLITE_DONE && tileIndex<SW*SH) {
		in->screen[tileIndex/SH][tileIndex%SW]=sqlite3_column_int(statement,1);
		tileIndex++;
	}
	tileIndex=0;
	while(sqlite3_step(statement)!=SQLITE_DONE && tileIndex<SW*SH) {
		in->layers[tileIndex/SH][tileIndex%SW]=sqlite3_column_int(statement,1);
		tileIndex++;
	}
	tileIndex=0;
	while(sqlite3_step(statement)!=SQLITE_DONE && tileIndex<SW*SH) {
		in->tScreen[tileIndex/SH][tileIndex%SW]=sqlite3_column_int(statement,1);
		tileIndex++;
	}
	sqlite3_finalize(statement);
	total=(double)(clock()-start)/CLOCKS_PER_SEC;
	printf("Chunk deserialised in %lf seconds.\n",total);
	return 0;
}

int main(int argc, char** argv) {
	int timer=0;
	int tileI=0;
	sqlite3_stmt *statement;
	sheetM=IMG_Load("sheet.png");

	sqlite3_open("world.map",&map);
	loadChunk(&canvas,0,0);
	canvas=map_beachright;

	SDL_Init(SDL_INIT_VIDEO);
	Tcl_FindExecutable(argv[0]);
	interp = Tcl_CreateInterp(); 
    if (Tcl_Init(interp) != TCL_OK) { 
        printf("Unable to start TCL interpreter, exiting.\n");
        return 1;
    }
    w = SDL_CreateWindow("Drip Map Editor", 0, 0, SW*TS*4+TS*8, SH*TS*4+TS*8, SDL_WINDOW_OPENGL);
    sheetM=IMG_Load("sheet.png");
    ws=SDL_GetWindowSurface(w);

    Tcl_CreateCommand(interp, "sdlLoop", (void*)edloop, NULL, NULL);//Registers a callback for the TK event loop to poll.

    if(Tcl_EvalFile(interp, "mapeditor.tcl") != TCL_OK) {
    	printf("Starting GUI event loop failed!\n");
    	printf(Tcl_GetStringResult(interp));
    }
    
    Tk_MainLoop();
	return 0;
}