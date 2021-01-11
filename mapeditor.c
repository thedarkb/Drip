#include <stdio.h>
#include <tcl/tcl.h>
#include <tcl/tk.h>
#include <sqlite3.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

sqlite3* map=NULL;
Tcl_Interp* interp=NULL;
SDL_Surface* sheet=NULL;
SDL_Surface* ws=NULL;
SDL_Window* w=NULL;
SDL_Event keyIn;

int loop(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	SDL_PollEvent(&keyIn);
	if (keyIn.type == SDL_QUIT) exit(0);
	return 0;
}

int main(int argc, char** argv) {
	int timer=0;

	SDL_Init(SDL_INIT_VIDEO);
	Tcl_FindExecutable(argv[0]);
	interp = Tcl_CreateInterp(); 
    if (Tcl_Init(interp) != TCL_OK) { 
        printf("Unable to start TCL interpreter, exiting.\n");
        return 1;
    }
    w = SDL_CreateWindow("Drip Map Editor", 0, 0, 640, 480, SDL_WINDOW_OPENGL);
    sheet=IMG_Load("sheet.png");
    ws=SDL_GetWindowSurface(w);

    Tcl_CreateCommand(interp, "sdlLoop", (void*)loop, NULL, NULL);
    if(Tcl_EvalFile(interp, "mapeditor.tcl") != TCL_OK) {
    	printf("Starting GUI event loop failed!\n");
    	printf(Tcl_GetStringResult(interp));
    }
    
    Tk_MainLoop();
	return 0;
}