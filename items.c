enum {
	ITEM_NULL,
	ITEM_Q,
	ITEM_SWORD,
	ITEM_AXE,
	ITEM_KEY
} itemSyms;

unsigned char getItemSprite(unsigned char type) {
	unsigned char sprite[255]={
		0,
		69,
		84,
		96,
		99
	};

	return sprite[type];
}

void itemEffects(unsigned char type, unsigned char slot) {
	printf("Item type: %u\n", type);
	switch (type) {
		case 0:
			printf("I am a potato.\n");
		break;
		case 2:
			;
			entity spawnMe=ent_sword(entSet[0].direction, 0, 0, 0);
			entitySpawn(spawnMe,0,0);
			swordOut=1;
		break;
		case 3:
			entitySpawn(ent_axe(entSet[0].direction, 0, 0, 0),0,0);
			swordOut=1;
		case 4:
			for(int i=0;i<ELIMIT;i++) {
				if(entSet[i].behaviour==behav_lockedDoor && euclideanDistance(i,0,32)) {
					flagArray[entSet[i].status[0]]=1;
					pInv.items[slot].type=0;
					break;
				}
			}
		break;
	}
}