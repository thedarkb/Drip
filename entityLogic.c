#define ME entSet[i]

void entityLogic() {
	for (int i=0; i<ELIMIT; i++) {
		if(entSet[i].state[0]) {
			printf("%s\n",entSet[i].state);
			if(!Tcl_Eval(gameState, entSet[i].state)) {
				strcpy(entSet[i].state,Tcl_GetStringResult(gameState));
			} else {
				printf("Error: %s\n",Tcl_GetStringResult(gameState));
				exit(1);
			}
		}
	}
}