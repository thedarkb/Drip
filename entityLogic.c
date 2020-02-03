void playerBehaviour(int i) {
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
	char pmotion=0;
	static int zTimeout=0;

	for (int j=0; j<TS/8; j++) { //number of pixels to move per frame.
		if (entSet[i].status[3]==0 && !swordOut && !dialogueOut) {	
			if (keyboard[SDL_SCANCODE_RIGHT]) {
				moveX(&entSet[i], 1);
				pmotion=1;
			}
			if (keyboard[SDL_SCANCODE_LEFT]) {
				moveX(&entSet[i], -1);
				pmotion=1;
			}						
			if (keyboard[SDL_SCANCODE_UP]) {
				moveY(&entSet[i], -1);
				pmotion=1;
			}
			if (keyboard[SDL_SCANCODE_DOWN]) {
				moveY(&entSet[i], 1);
				pmotion=1;
			}
			if (pmotion == 0) entSet[i].animation=0;
			
			if(keyboard[SDL_SCANCODE_Z]) {
				entitySpawn(ent_dialogue(entSet[i].direction, entSet[i].x, entSet[i].y),0,0);
				dialogueOut=1;
				zTimeout=30;
			}				
			if (keyboard[SDL_SCANCODE_X] && !swordOut) {
				itemEffects(pInv.items[pInv.weapon].type);		
			}
			if (keyboard[SDL_SCANCODE_C]) itemEffects(pInv.items[pInv.selection].type);
		}

		if(!keyboard[SDL_SCANCODE_X]) swordOut=0;
		if(!keyboard[SDL_SCANCODE_Z] && !zTimeout) dialogueOut=0;

		if(zTimeout>0) zTimeout--;

		if(keyboard[SDL_SCANCODE_LSHIFT]) {
			if (keyboard[SDL_SCANCODE_1]) {
				pInv.weapon=0;
			} else if (keyboard[SDL_SCANCODE_2]) {
				pInv.weapon=1;
			} else if (keyboard[SDL_SCANCODE_3]) {
				pInv.weapon=2;
			} else if (keyboard[SDL_SCANCODE_4]) {
				pInv.weapon=3;
			} else if (keyboard[SDL_SCANCODE_5]) {
				pInv.weapon=4;
			} else if (keyboard[SDL_SCANCODE_6]) {
				pInv.weapon=5;
			} else if (keyboard[SDL_SCANCODE_7]) {
				pInv.weapon=6;
			} else if (keyboard[SDL_SCANCODE_8]) {
				pInv.weapon=7;
			}
		}
		if(!keyboard[SDL_SCANCODE_LSHIFT]) {
			if (keyboard[SDL_SCANCODE_1]) {
				pInv.selection=0;
			} else if (keyboard[SDL_SCANCODE_2]) {
				pInv.selection=1;
			} else if (keyboard[SDL_SCANCODE_3]) {
				pInv.selection=2;
			} else if (keyboard[SDL_SCANCODE_4]) {
				pInv.selection=3;
			} else if (keyboard[SDL_SCANCODE_5]) {
				pInv.selection=4;
			} else if (keyboard[SDL_SCANCODE_6]) {
				pInv.selection=5;
			} else if (keyboard[SDL_SCANCODE_7]) {
				pInv.selection=6;
			} else if (keyboard[SDL_SCANCODE_8]) {
				pInv.selection=7;
			}
		}
	
		if (keyboard[SDL_SCANCODE_D] && pInv.items[pInv.selection].type) {
			entitySpawn(ent_item(0, 0, pInv.items[pInv.selection].type, 255),entSet[i].x,entSet[i].y);
			pInv.items[pInv.selection].type=0;
		}
		if (keyboard[SDL_SCANCODE_K]) snapToGrid(&entSet[i]);
		if (keyboard[SDL_SCANCODE_L]) {
			pushMsg("Test\0");
			pushMsg("Message\0");
			pushMsg("Stacking\0");
		}
		if (keyboard[SDL_SCANCODE_N]) entitySpawn(ent_aitest(), 0,0);
		if (keyboard[SDL_SCANCODE_J]) {
			for (int k=0; k<ELIMIT; k++) {
				printf("%u\n", entSet[k].behaviour);
			}
		}
		if (keyboard[SDL_SCANCODE_F11]) SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN);                            
		
		if (entSet[i].y < 0) scroll = 1;
		if (entSet[i].y > TS*SH) scroll=2;
		if (entSet[i].x < -TS) scroll = 3;
		if (entSet[i].x > TS*SW-TS-1) scroll = 4;
		if (entSet[i].health<pMaxHealth && animationG > 29) entSet[i].health++;
		
		//if(entSet[i].status[3]>0) entSet[i].status[3]--;
		//if(entSet[i].status[0]>0) entSet[i].status[0]--;
	}
}

void behav_potato(int i) {
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS);
}

void behav_chase(int i) {
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS); //ANIMPARSE live in main.h
	for (int j=0; j<TS/16; j++) {
		if (entSet[0].x > entSet[i].x) moveX(&entSet[i], 1);
		if (entSet[0].x < entSet[i].x) moveX(&entSet[i], -1);
		if (entSet[0].y > entSet[i].y) moveY(&entSet[i], 1);
		if (entSet[0].y < entSet[i].y) moveY(&entSet[i], -1);
	}
}

void behav_wait(int i) {
	entSet[i].animation=0;
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS);
	if(get_diff(entSet[i].x,entSet[0].x) < TS*2 && get_diff(entSet[i].y,entSet[0].y) < TS*2) entSet[i].behaviour=behav_chase;
}

void behav_upLeft(int i) {
	fastMoveX(&entSet[i], -1, TS/8);
	fastMoveY(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);				
}

void behav_upRight(int i) {
	fastMoveX(&entSet[i], 1, TS/8);
	fastMoveY(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);	
}

void behav_downLeft(int i) {
	fastMoveX(&entSet[i], -1, TS/8);
	fastMoveY(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_downRight(int i) {
	fastMoveX(&entSet[i], 1, TS/8);
	fastMoveY(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_sword(int i) {
	if (!swordOut) entSet[i].health=0;
	switch(entSet[entSet[i].status[1]].direction) {
		case 0:
			entSet[i].x=entSet[entSet[i].status[1]].x;
			entSet[i].y=entSet[entSet[i].status[1]].y-TS;
		break;
		case 1:
			entSet[i].x=entSet[entSet[i].status[1]].x;
			entSet[i].y=entSet[entSet[i].status[1]].y+TS;
		break;
		case 2:
			entSet[i].x=entSet[entSet[i].status[1]].x-TS;
			entSet[i].y=entSet[entSet[i].status[1]].y;
		break;
		case 3:
			entSet[i].x=entSet[entSet[i].status[1]].x+TS;
			entSet[i].y=entSet[entSet[i].status[1]].y;
		break;
	}
	/*if(entSet[entSet[i].status[1]].status[3])*/ image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);	
}

void behav_up(int i) {
	fastMoveY(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_down(int i) {
	fastMoveY(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_left(int i) {
	fastMoveX(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_right(int i) {
	fastMoveX(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_dialogue(int i) {
	if (!dialogueOut) entSet[i].health=0;
	else {
		printf("Looking for dialogue...\n");
		
		switch(entSet[entSet[i].status[1]].direction) {
			case 0:
				entSet[i].x=entSet[entSet[i].status[1]].x;
				entSet[i].y=entSet[entSet[i].status[1]].y-TS;
			break;
			case 1:
				entSet[i].x=entSet[entSet[i].status[1]].x;
				entSet[i].y=entSet[entSet[i].status[1]].y+TS;
			break;
			case 2:
				entSet[i].x=entSet[entSet[i].status[1]].x-TS;
				entSet[i].y=entSet[entSet[i].status[1]].y;
			break;
			case 3:
				entSet[i].x=entSet[entSet[i].status[1]].x+TS;
				entSet[i].y=entSet[entSet[i].status[1]].y;
			break;
		}
	}
	//image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_dead(int i) {
	if (entSet[i].deathframe) image(hwtileset[entSet[i].deathframe], entSet[i].x, entSet[i].y, TS, TS);
}


void behav_itemStatic(int i) {
	image(hwtileset[entSet[i].frame[0]], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_item(int i) {
	fastMoveY(&entSet[i], 1, TS);
	image(hwtileset[entSet[i].frame[0]], entSet[i].x, entSet[i].y, TS, TS);
	entSet[i].behaviour=behav_itemStatic;
	entSet[i].collisionClass=130;
}

void behav_techNpcSpawn(int i) {
	entSet[i].status[0]=0;
	for(int j=0; j<ELIMIT; j++) {
		if(entSet[j].behaviour==&behav_agNpcSpawn && !entSet[j].status[1]) {
			entSet[i].status[0]=j;
			entSet[j].status[1]=1;
			goto quit;
		}
	}
	for(int j=0; j<ELIMIT; j++) {
		if(entSet[j].behaviour==&behav_agNpc) {
			entSet[i].status[0]=j;
			entSet[j].status[1]=1;
			goto quit;
		}
	}
	quit:
	;
	entSet[i].behaviour=behav_techNpc;
}

void behav_techNpc(int i) {
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS); //ANIMPARSE live in main.h
	reroll();

	for (int j=0; j<TS/8; j++) {
		if (entSet[entSet[i].status[0]].x > entSet[i].x) moveX(&entSet[i], 1);
		if (entSet[entSet[i].status[0]].x < entSet[i].x) moveX(&entSet[i], -1);
		if (entSet[entSet[i].status[0]].y > entSet[i].y) moveY(&entSet[i], 1);
		if (entSet[entSet[i].status[0]].y < entSet[i].y) moveY(&entSet[i], -1);
	}
	if(!entSet[entSet[i].status[0]].health) entSet[i].behaviour=behav_techNpcSpawn;
}

void behav_agNpcSpawn(int i) {
	entSet[i].status[0]=0;
	for(int j=0; j<ELIMIT; j++) {
		printf("Agrarian NPC has located tech NPC\n");
		if(entSet[j].behaviour==&behav_techNpcSpawn && !entSet[j].status[1]) {
			entSet[i].status[0]=j;
			entSet[j].status[1]=1;
			goto quit;
		}
	}
	for(int j=0; j<ELIMIT; j++) {
		if(entSet[j].behaviour==&behav_techNpc) {
			printf("Agrarian NPC has located ganged up tech NPC\n");
			entSet[i].status[0]=j;
			entSet[j].status[1]=1;
			goto quit;
		}
	}
	quit:
	;
	entSet[i].behaviour=behav_agNpc;
}

void behav_agNpc(int i) {
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS); //ANIMPARSE lives in main.h
	reroll();
	for (int j=0; j<TS/8; j++) {
		if (entSet[entSet[i].status[0]].x > entSet[i].x) moveX(&entSet[i], 1);
		if (entSet[entSet[i].status[0]].x < entSet[i].x) moveX(&entSet[i], -1);
		if (entSet[entSet[i].status[0]].y > entSet[i].y) moveY(&entSet[i], 1);
		if (entSet[entSet[i].status[0]].y < entSet[i].y) moveY(&entSet[i], -1);
	}
	if(!entSet[entSet[i].status[0]].health) entSet[i].behaviour=behav_agNpcSpawn;
}


void entityLogic() {
	for(int l=0; l<SPRITELAYERS; l++) {
		for (int i=0; i<ELIMIT; i++) {
			if(entSet[i].behaviour && entSet[i].layer == l) {
				entSet[i].behaviour(i);
			}
		}
	}
}