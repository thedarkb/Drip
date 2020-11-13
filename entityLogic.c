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

	for (int j=0; j<TS/8; j++) { //number of pixels to move per frame.
		if (/*entSet[i].status[3]==0 &&*/ !swordOut && !dialogueOut) {	
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
				//entitySpawn(ent_dialogue(entSet[i].direction, entSet[i].x, entSet[i].y,0),0,0);
				dialogueOut=1;
				zTimeout=30;
			}				
			if (keyboard[SDL_SCANCODE_X] && !swordOut) {
				//itemEffects(pInv.items[pInv.weapon].type,pInv.weapon);		
			}
			//if (keyboard[SDL_SCANCODE_C] && !swordOut) itemEffects(pInv.items[pInv.selection].type,pInv.selection);
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
	
		//if (keyboard[SDL_SCANCODE_D] && pInv.items[pInv.selection].type) {
		//	entitySpawn(ent_item(pInv.items[pInv.selection].type, 255,0,0),entSet[i].x,entSet[i].y);
		//	pInv.items[pInv.selection].type=0;
		//}
		if (keyboard[SDL_SCANCODE_K]) snapToGrid(&entSet[i]);

		#ifdef DEV
		static int toggleTab=0;
		if(!toggleTab && keyboard[SDL_SCANCODE_TAB]) {
			toggleTab=1;
			if(mapEditorEnable) {
				mapEditorEnable=0;
				tilewrapper[1][1].flag=1;
				refresh=1;
			} else {
				mapEditorEnable=1;
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

void entityLogic() {
	for(int l=0; l<SPRITELAYERS; l++) {
		for (int i=0; i<ELIMIT; i++) {
			if(!entSet[0].behaviour) printf("Entity 0 is null!\n");
			if(entSet[i].behaviour && entSet[i].layer == l) {
				entSet[i].behaviour(i);
			}
			#ifdef DEV
			if(mapEditorEnable) break;
			#endif
		}
	}
}