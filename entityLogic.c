#define ME entSet[i]

void entityLogic() {
	for(int l=0; l<SPRITELAYERS; l++) {
		for (int i=0; i<ELIMIT; i++) {
			if(entSet[i].behaviour[0]) {
				char callString[128];
				/*sprintf(&callString, "%s {%d %d %d %d %d %d %d %s}", entSet[i].behaviour,
					entSet[i].x,
					entSet[i].y,
					entSet[i].dX,
					entSet[i].dY,
					entSet[i].xSub,
					entSet[i].ySub,
					entSet[i].state
				);*/
				Tcl_Eval(gameState, callString);
				printf(Tcl_GetStringResult(gameState));
			}
		//	if(!entSet[0].behaviour) printf("Entity 0 is null!\n");
		//	if(entSet[i].behaviour && entSet[i].layer == l) {
		//		entSet[i].behaviour(i);
		//	}
			#ifdef DEV
			//if(mapEditorEnable) break;
			#endif
		}
	}
}