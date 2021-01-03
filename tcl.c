enum {
	E_BEHAVIOUR,
	E_DIALOGUE,
	E_X,
	E_Y,
	E_DX,
	E_DY,
	E_XSUB,
	E_YSUB,
	E_HEALTH,
	E_STATELIST
};

char* lindex(char* in, int i) {
	static char output[512];
	output[0]='\0';
	char* token=NULL;
	

	if(!strlen(in)) return output;
	token=strtok(in," ");
	if(!i) {
		strcpy(output,token);
		return output;
	}
	for(int j=0;j<i;j++) {
		token=strtok(NULL, " ");
	}
	if(token) strcpy(output,token);
	return output;
}

int stateGet(char* in, int kv) {
	return atoi(lindex(in,kv));
}

char* stateSet(char* in, int kv, int value) {
	char toEval[512];
	toEval[0]='\0';

	sprintf(toEval,"lreplace {%s} %d 1 %d", in, kv, value);
	Tcl_Eval(gameState,toEval);
	strcpy(in,Tcl_GetStringResult(gameState));
	return in;
}

int T_key(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	if(argc!=2) {
		printf("Wrong number of arguments (%d) to keyboard!", argc);
		return 1;
	}
	if(keyboard[atoi(argv[1])]) Tcl_SetResult(state, "1", NULL);
	else Tcl_SetResult(state, "0", NULL);
	return 0;
}

int T_image(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	if(argc==6) {
		image(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));		
		return 0;
	} else if (argc==4) {
		image(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),TS,TS);
		return 0;	
	}
	printf("Wrong number of arguments (%d) to image!\n", argc);
	return 1;
}

/*int T_image(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	if(argc==6) {
		image(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));		
		return 0;
	} else if (argc==4) {
		image(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),TS,TS);
		return 0;	
	}
	printf("Wrong number of arguments (%d) to image!\n", argc);
	return 1;
}*/

int T_spawn(ClientData clientData, Tcl_Interp* state, int argc, char** argv) {
	
}

int registerCommands() {
	Tcl_CreateCommand(gameState, "image", (void*)T_image, NULL, NULL);
	Tcl_CreateCommand(gameState, "key", (void*)T_key, NULL, NULL);
}