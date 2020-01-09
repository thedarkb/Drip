void worldgen(uint16_t xPos, uint16_t yPos) {
	uint32_t hashme;
	uint16_t* hashCom = &hashme;

	*hashCom = xPos;
	hashCom++;
	*hashCom = yPos;

	uint32_t screenHash = lfsr(hashme);

	printf("%u\n",hashme);

	memcpy(&nentSet, &entSet, sizeof nentSet);
	nentityReset();

	for (register x=0; x<SW; x++) { //first pass clears the tileset, adds flowers.
		for (register y=0; y<SH; y++) {
			nScreen[x][y] = 11; //shifts left to account for the collision bit.
			if ((screenHash & BIT(2)) && (screenHash & BIT(5)) && (screenHash & BIT(9))) nScreen[x][y] = 12;
			screenHash = lfsr(screenHash);
			setCollision(x,y,0);
		}
	}

	for (register x=0; x<SW; x++) { //adds trees
		for (register y=0; y<SH; y++) {
			if ((screenHash & BIT(5)) && (screenHash & BIT(7))) {
				nScreen[x][y] = (30);
				setCollision(x,y,1);
			}
			screenHash = lfsr(screenHash);
		}
	}
	if(xPos == 0 && yPos == 0) {
		memcpy(&nScreen, testhouse().tileLayer, sizeof nScreen);
		//nentitySpawn(ent_techo());
	}
}