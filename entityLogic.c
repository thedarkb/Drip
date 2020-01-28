void entityLogic() {
	for (int i=0; i<spawnSlot; i++) { //Bottom entity layer, affectionately known as the corpse pit.
		switch (entSet[i].behaviourId) {
			case 255:
				if (entSet[i].deathframe) image(hwtileset[entSet[i].deathframe], entSet[i].x, entSet[i].y, TS, TS);
			break;
			case 10:
				image(hwtileset[entSet[i].frame[0]], entSet[i].x, entSet[i].y, TS, TS);
			break;
		}
	}
	for (int i=0; i<spawnSlot; i++) {
		switch(entSet[i].behaviourId) {

			/*The AI state machine starts here.*/	
			case 1: //The player.
				playerBehaviour(i);
			break;//End of player.


			case 2://Potato
				behav_potato(i);
			break;//End of Potato


			case 3://Chase state.
				behav_chase(i);
			break;//End of chase state.


			case 4://Entity stays still, changes to chase state when player is in range.
				behav_wait(i);
			break; //End wait state.

			case 5:
				behav_upLeft(i);
			break;
			case 6:
				behav_upRight(i);
			break;
			case 7:
				behav_downLeft(i);
			break;
			case 8:
				behav_downRight(i);
			break;

			case 9:
				behav_sword(i);
			break;

			case 11:
				behav_up(i);
			break;
			case 12:
				behav_down(i);
			break;
			case 13:
				behav_left(i);
			break;
			case 14:
				behav_right(i);
			break;
			
			case 15:
				behav_dialogue(i);
			break;
		}
	}
}

void playerBehaviour(int i) {
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
	char pmotion=0;
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
			}				
			if (keyboard[SDL_SCANCODE_X] && !swordOut) {
				entitySpawn(ent_sword(entSet[i].direction, entSet[i].x, entSet[i].y, i),0,0);
				swordOut=1;							
			}
		}
		if(!keyboard[SDL_SCANCODE_X]) swordOut=0;
		if(!keyboard[SDL_SCANCODE_Z]) dialogueOut=0;
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
	
		if (keyboard[SDL_SCANCODE_D] && pInv.items[pInv.selection].type) {
			entitySpawn(ent_item(entSet[i].x, entSet[i].y, pInv.items[pInv.selection].type, 255),0,0);
			fastMoveY(&entSet[spawnSlot-1], 1, TS);
			pInv.items[pInv.selection].type=0;
		}
		if (keyboard[SDL_SCANCODE_K]) snapToGrid(&entSet[i]);
		if (keyboard[SDL_SCANCODE_L]) {
			pushMsg("Test\0");
			pushMsg("Message\0");
			pushMsg("Stacking\0");
		}
		if (keyboard[SDL_SCANCODE_N]) entitySpawn(ent_aitest(), 0,0);
		if (keyboard[SDL_SCANCODE_A]) itemEffects(pInv.items[pInv.selection].type);
		if (keyboard[SDL_SCANCODE_J]) {
			for (int k=0; k<ELIMIT; k++) {
				printf("%u\n", entSet[k].behaviourId);
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
	if(get_diff(entSet[i].x,entSet[0].x) < TS*2 && get_diff(entSet[i].y,entSet[0].y) < TS*2) entSet[i].behaviourId=3;
}

void behav_upLeft(int i) {
	fastMoveX(&entSet[i], -1, TS/8);
	fastMoveY(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviourId=entSet[i].status[0];
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);				
}

void behav_upRight(int i) {
	fastMoveX(&entSet[i], 1, TS/8);
	fastMoveY(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviourId=entSet[i].status[0];
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);	
}

void behav_downLeft(int i) {
	fastMoveX(&entSet[i], -1, TS/8);
	fastMoveY(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviourId=entSet[i].status[0];
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_downRight(int i) {
	fastMoveX(&entSet[i], 1, TS/8);
	fastMoveY(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviourId=entSet[i].status[0];
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
	printf("Up\n");
	fastMoveY(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviourId=entSet[i].status[0];
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_down(int i) {
	printf("Down\n");
	fastMoveY(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviourId=entSet[i].status[0];
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_left(int i) {
	printf("Left\n");
	fastMoveX(&entSet[i], -1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviourId=entSet[i].status[0];
		entSet[i].collisionClass=entSet[i].status[2];
	}
	image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}

void behav_right(int i) {
	printf("Right\n");
	fastMoveX(&entSet[i], 1, TS/8);
	if(entSet[i].status[1]>0) entSet[i].status[1]--;
	else {
		entSet[i].behaviourId=entSet[i].status[0];
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
	/*if(entSet[entSet[i].status[1]].status[3])*/ image(hwtileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
}