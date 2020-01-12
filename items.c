unsigned char getItemSprite(unsigned char type) {
	unsigned char sprite[255];

	if (type==0) {
		printf("Invalid drop.");
		return 0;
	}
	sprite[0]=83;
	sprite[1]=69;
	sprite[2]=69;

	return sprite[type];
}

void itemEffects(unsigned char type) {
	switch (type) {
		case 0:
			printf("I am a potato.\n");
		break;
		case 1:
			printf("My name is Steve, I'm a bunch of coloured rocks and I like the exit function.\n");
			sleep(5);
			exit(0);
		break;
	}
}