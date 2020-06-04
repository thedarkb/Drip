#define ME entSet[i]

void playerBehaviour(int i) {
	if(swordOut) entSet[i].animation=8;
	#ifndef DEV
	if(!refresh) image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
	#endif
	#ifdef DEV
	if(mapEditorEnable) image(hwtileset[83], entSet[i].x, entSet[i].y, TS, TS);
	else if(!refresh) image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
	#endif

	if(mode) return;

	char pmotion=0;
	static int zTimeout=0;
	if(entSet[i].freezeFrames) {
		entSet[i].freezeFrames--;
		return;
	}

	for (int j=0; j<TS*8; j++) { //number of pixels to move per frame.
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
			if (!pmotion) entSet[i].animation=0;
			
			if(keyboard[SDL_SCANCODE_Z]) {
				entitySpawn(ent_dialogue(entSet[i].direction, entSet[i].x, entSet[i].y,0),0,0);
				dialogueOut=1;
				zTimeout=30;
			}				
			if (keyboard[SDL_SCANCODE_X] && !swordOut) {
				itemEffects(pInv.items[pInv.weapon].type,pInv.weapon);		
			}
			if (keyboard[SDL_SCANCODE_C] && !swordOut) itemEffects(pInv.items[pInv.selection].type,pInv.selection);
		}

		if(!keyboard[SDL_SCANCODE_X] && !keyboard[SDL_SCANCODE_C]) swordOut=0;
		if(!keyboard[SDL_SCANCODE_Z] && !zTimeout) dialogueOut=0;
		if(!keyboard[SDL_SCANCODE_F6]) collisionReset=1;

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
			entitySpawn(ent_item(pInv.items[pInv.selection].type, 255,0,0),entSet[i].x,entSet[i].y);
			pInv.items[pInv.selection].type=0;
		}
		if (keyboard[SDL_SCANCODE_K]) snapToGrid(&entSet[i]);

		#ifdef DEV
		static int toggleTab=0;
		if(!toggleTab && keyboard[SDL_SCANCODE_TAB]) {
			toggleTab=1;
			if(mapEditorEnable) {
				mapEditorEnable=0;
				entSet[0].collisionClass=1;
				tilewrapper[1][1].flag=1;
				refresh=1;
			} else {
				mapEditorEnable=1;
				entSet[0].collisionClass=0;
				mapEditorShim(&tilewrapper[1][1],sX,sY);
			}
		}
		if(!keyboard[SDL_SCANCODE_TAB]) toggleTab=0;
		#endif

		if (keyboard[SDL_SCANCODE_F11]) SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN_DESKTOP);
		if (keyboard[SDL_SCANCODE_F1]) printf("Entity 0 at %d,%d\n",entSet[i].x,entSet[i].y);
		if(keyboard[SDL_SCANCODE_F3]) printf("Screen: %u,%u\n",sX,sY);
		if(keyboard[SDL_SCANCODE_F2]) printf("Camera at %d,%d\n",cameraX,cameraY);
		
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
	//image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS);
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
	drawClothes(&entSet[i]);			
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
	drawClothes(&entSet[i]);
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
	drawClothes(&entSet[i]);
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
	drawClothes(&entSet[i]);
}

void behav_sword(int i) {
	if (!swordOut) entSet[i].health=0;
	entSet[i].direction=entSet[entSet[i].status[1]].direction;
	switch(entSet[entSet[i].status[1]].direction) {
		case 0:
			entSet[i].x=entSet[entSet[i].status[1]].x;
			entSet[i].y=entSet[entSet[i].status[1]].y-TS;
			image(hwtileset[entSet[i].frame[0]], entSet[i].x, entSet[i].y, TS, TS);
			image(hwtileset[entSet[i].frame[4]], entSet[i].x, entSet[i].y+TS, TS, TS);
		break;
		case 1:
			entSet[i].x=entSet[entSet[i].status[1]].x;
			entSet[i].y=entSet[entSet[i].status[1]].y+TS;
			image(hwtileset[entSet[i].frame[1]], entSet[i].x, entSet[i].y, TS, TS);
			image(hwtileset[entSet[i].frame[5]], entSet[i].x, entSet[i].y-TS, TS, TS);
		break;
		case 2:
			entSet[i].x=entSet[entSet[i].status[1]].x-TS;
			entSet[i].y=entSet[entSet[i].status[1]].y;
			image(hwtileset[entSet[i].frame[2]], entSet[i].x, entSet[i].y, TS, TS);
			image(hwtileset[entSet[i].frame[6]], entSet[i].x+TS, entSet[i].y, TS, TS);
		break;
		case 3:
			entSet[i].x=entSet[entSet[i].status[1]].x+TS;
			entSet[i].y=entSet[entSet[i].status[1]].y;
			image(hwtileset[entSet[i].frame[3]], entSet[i].x, entSet[i].y, TS, TS);
			image(hwtileset[entSet[i].frame[7]], entSet[i].x-TS, entSet[i].y, TS, TS);
		break;
	}
}

void behav_up(int i) {
	fastMoveY(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
	drawClothes(&entSet[i]);
}

void behav_down(int i) {
	fastMoveY(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
	drawClothes(&entSet[i]);
}

void behav_left(int i) {
	fastMoveX(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
	drawClothes(&entSet[i]);
}

void behav_right(int i) {
	fastMoveX(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviour=entSet[i].prevState;
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
	drawClothes(&entSet[i]);
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

void behav_npcSpawn(int i){entSet[i].behaviour=behav_npc;}

void behav_npc(int i) {
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS); //ANIMPARSE lives in main.h
	drawClothes(&entSet[i]);

	reroll();
	if(entSet[entSet[i].status[0]].x > entSet[i].x) moveX(&entSet[i], 1); //Chase target at x,y
	if(entSet[entSet[i].status[0]].x < entSet[i].x) moveX(&entSet[i], -1);
	if(entSet[entSet[i].status[0]].y > entSet[i].y) moveY(&entSet[i], 1);
	if(entSet[entSet[i].status[0]].y < entSet[i].y) moveY(&entSet[i], -1); //Pointer to entity, target position, speed.

	//if(!entSet[entSet[i].status[0]].health || !entSet[entSet[i].status[0]].visible) entSet[i].behaviour=behav_npcSpawn;
}

void behav_wall(int i) {
	image(hwtileset[entSet[i].frame[0]], entSet[i].x, entSet[i].y,TS,TS);
}

void behav_door(int i) {
	if(euclideanDistance(i,0,10)) {
		entSet[0].x=ME.status[2];
		entSet[0].y=ME.status[3];
		sX=ME.status[0];
		sY=ME.status[1];
		refresh=1;
	}
	image(hwtileset[entSet[i].frame[0]], entSet[i].x, entSet[i].y,TS,TS);
}

void behav_lockedDoor(int i) {
	if(entSet[i].x<0||entSet[i].x>SW*TS || entSet[i].y<0||entSet[i].y>SH*TS) goto drawOnly;
	if(flagArray[entSet[i].status[0]]) {
		tilewrapper[1][1].layers[entSet[i].y/TS][entSet[i].x/TS]=0;
		return;
	} else {
		tilewrapper[1][1].layers[entSet[i].y/TS][entSet[i].x/TS]=1;
	}

	drawOnly:
	if(flagArray[entSet[i].status[0]]) return;
	image(hwtileset[entSet[i].frame[0]], entSet[i].x, entSet[i].y,TS,TS);
}

void entityLogic() {
	for(int l=0; l<SPRITELAYERS; l++) {
		for (int i=0; i<ELIMIT; i++) {
			if(entSet[i].behaviour && entSet[i].layer == l) {
				entSet[i].behaviour(i);
			}
			#ifdef DEV
			if(mapEditorEnable) break;
			#endif
		}
	}
}