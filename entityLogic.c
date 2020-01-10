void entityLogic() {
	for (register i=0; i<spawnSlot; i++) {
		switch(entSet[i].behaviourId) {

			/*The AI state machine starts here.*/

			case 1: //The player.
				image(tileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
				char pmotion=0;
				for (register j=0; j<7; j++) { //number of pixels to move per frame.
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

					if (keyboard[SDL_SCANCODE_K]) snapToGrid(&entSet[i]);
					if (keyboard[SDL_SCANCODE_F11]) SDL_SetWindowFullscreen(w, SDL_WINDOW_FULLSCREEN);                            

					if (entSet[i].y < 1) scroll = 1;
					if (entSet[i].y+TS > TS*SH-1) scroll=2;
					if (entSet[i].x < 1) scroll = 3;
					if (entSet[i].x > TS*SW-TS-1) scroll = 4;

				}
			break;//End of player.


			case 2://Potato
				image(tileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS);
			break;//End of Potato


			case 3://Chase state.
				image(tileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS); //ANIMPARSE live in main.h
				for (register j=0; j<4; j++) {
					if (entSet[0].x > entSet[i].x) moveX(&entSet[i], 1);
					if (entSet[0].x < entSet[i].x) moveX(&entSet[i], -1);
					if (entSet[0].y > entSet[i].y) moveY(&entSet[i], 1);
					if (entSet[0].y < entSet[i].y) moveY(&entSet[i], -1);
				}
			break;//End of chase state.


			case 4://Entity stays still, changes to chase state when player is in range.
				entSet[i].animation=0;
				image(tileset[ANIMPARSE], entSet[i].x, entSet[i].y,TS,TS);
				if(get_diff(entSet[i].x,entSet[0].x) < TS*2 && get_diff(entSet[i].y,entSet[0].y) < TS*2) entSet[i].behaviourId=3;
			break; //End wait state.

			case 5:
				fastMoveX(&entSet[i], -1, 8);
				fastMoveY(&entSet[i], -1, 8);
				if(entSet[i].status[1]>0) entSet[i].status[1]--;
				else {
					entSet[i].behaviourId=entSet[i].status[0];
					entSet[i].collisionClass=entSet[i].status[2];
				}
				image(tileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
			break;
			case 6:
				fastMoveX(&entSet[i], 1, 8);
				fastMoveY(&entSet[i], -1, 8);
				if(entSet[i].status[1]>0) entSet[i].status[1]--;
				else {
					entSet[i].behaviourId=entSet[i].status[0];
					entSet[i].collisionClass=entSet[i].status[2];
				}
				image(tileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
			break;
			case 7:
				fastMoveX(&entSet[i], -1, 8);
				fastMoveY(&entSet[i], 1, 8);
				if(entSet[i].status[1]>0) entSet[i].status[1]--;
				else {
					entSet[i].behaviourId=entSet[i].status[0];
					entSet[i].collisionClass=entSet[i].status[2];
				}
				image(tileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
			break;
			case 8:
				fastMoveX(&entSet[i], 1, 8);
				fastMoveY(&entSet[i], 1, 8);
				if(entSet[i].status[1]>0) entSet[i].status[1]--;
				else {
					entSet[i].behaviourId=entSet[i].status[0];
					entSet[i].collisionClass=entSet[i].status[2];
				}
				image(tileset[ANIMPARSE], entSet[i].x, entSet[i].y, TS, TS);
			break;
		}
	}
}