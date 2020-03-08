unsigned char getItemSprite(unsigned char type) {
	unsigned char sprite[255];

	if (type==0) {
		printf("Invalid drop.");
		return 0;
	}
	
	sprite[0]=83;
	sprite[1]=69;
	sprite[2]=84;
	sprite[3]=96;

	return sprite[type];
}

void itemEffects(unsigned char type) {
	printf("Item type: %u\n", type);
	switch (type) {
		case 0:
			printf("I am a potato.\n");
		break;
		case 1:
			printf("My name is Steve, I'm a bunch of coloured rocks and I like the exit function.\n");
			SDL_Delay(5000);
			exit(0);
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
	}
}