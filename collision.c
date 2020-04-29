void col_bouncy(int i, int j) {
	printf("Bouncy call\n");
	if (entSet[j].health>20) entSet[j].health-=20;
	else entSet[j].health=0;
	if (entSet[j].y+TS/2 < entSet[i].y+TS/2) {
		if(entSet[j].x+TS/2<entSet[i].x+TS/2) {
			entSet[j].status[2]=entSet[j].collisionClass;
			entSet[j].status[1]=10;
			if(entSet[j].behaviour!=behav_upLeft) entSet[j].prevState=entSet[j].behaviour;
			entSet[j].behaviour=behav_upLeft; //up left
			entSet[j].collisionClass=0;
		}
		else {
			entSet[j].status[2]=entSet[j].collisionClass;
			entSet[j].status[1]=10;
			if(entSet[j].behaviour!=behav_upRight) entSet[j].prevState=entSet[j].behaviour;
			entSet[j].behaviour=behav_upRight; //up right
			entSet[j].collisionClass=0;
		}
	}
	else {
		if(entSet[j].x+TS/2<entSet[i].x+TS/2) {
			entSet[j].status[2]=entSet[j].collisionClass;
			entSet[j].status[1]=10;
			if(entSet[j].behaviour!=behav_downLeft) entSet[j].prevState=entSet[j].behaviour;
			entSet[j].behaviour=behav_downLeft; //down left
			entSet[j].collisionClass=0;
		}
		else {
			entSet[j].status[2]=entSet[j].collisionClass;
			entSet[j].status[1]=10;
			if(entSet[j].behaviour!=behav_downRight) entSet[j].prevState=entSet[j].behaviour;
			entSet[j].behaviour=behav_downRight; //down right
			entSet[j].collisionClass=0;
		}
	}
}

void col_weapon(int i, int j) {
	//printf("Collision\n");
	//printf("Direction: %u\n", entSet[i].direction);
	switch(entSet[entSet[i].status[1]].direction) {
		case 0:
			entSet[j].status[2]=entSet[j].collisionClass;
			entSet[j].status[1]=10;
			entSet[j].prevState=entSet[j].behaviour;
			entSet[j].behaviour=behav_up; //up
			entSet[j].collisionClass=entSet[i].collisionClass;
		break;
		case 1:
			entSet[j].status[2]=entSet[j].collisionClass;
			entSet[j].status[1]=10;
			entSet[j].prevState=entSet[j].behaviour;
			entSet[j].behaviour=behav_down; //down
			entSet[j].collisionClass=entSet[i].collisionClass;
		break;
		case 2:
			entSet[j].status[2]=entSet[j].collisionClass;
			entSet[j].status[1]=10;
			entSet[j].prevState=entSet[j].behaviour;
			entSet[j].behaviour=behav_left; //left
			entSet[j].collisionClass=entSet[i].collisionClass;
		break;
		case 3:
			entSet[j].status[2]=entSet[j].collisionClass;
			entSet[j].status[1]=10;
			entSet[j].prevState=entSet[j].behaviour;
			entSet[j].behaviour=behav_right; //right
			entSet[j].collisionClass=entSet[i].collisionClass;
		break;
		entSet[j].lastHit=i;
	}
	if (entSet[entSet[i].status[1]].attack < entSet[j].health) entSet[j].health-=entSet[entSet[i].status[1]].attack;
	else entSet[j].health=0;
}

void col_item(int i, int j) {
	if (j) return;
	for (int k=0; k<INVLIMIT; k++) {
		if (!pInv.items[k].type) {
			pInv.items[k].type=entSet[i].status[0];
			entSet[i].health=0;
			if(entSet[i].status[1]) flagArray[entSet[i].status[1]]=1;
			break;
		}
	}
}

void col_dialogue(int i, int j) {
	speaker=j;
	printf("dialogue collision detected.\n");
	if(entSet[j].dialogue) entSet[j].dialogue();
	else pushMsg("No dialogue found for entity.\n");
	entSet[i].health=0;
	entSet[i].behaviour=NULL;
	entSet[i].collisionClass=0;
}