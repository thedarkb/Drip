void initialiseFactions() {
	memset(&factions, 0, sizeof factions);

	factions[0].centreX=0;
	factions[0].centreY=0;
	factions[0].radius=8;
	factions[0].baseAlignment=200;
	factions[0].alignmentFuzz=10;
	factions[0].aggroThreshold=200;
	factions[0].entPlates[0]=ent_agNpc();

	factions[1].centreX=50;
	factions[1].centreY=50;
	factions[1].radius=45;
	factions[1].baseAlignment=-200;
	factions[1].alignmentFuzz=10;
	factions[1].aggroThreshold=200;
	factions[1].entPlates[0]=ent_techNpc();
}
