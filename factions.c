faction fac_empty() {
	faction me;
	memset(&me,0,sizeof me);
	me.aggroThreshold=999999;
	return me;
}

faction fac_agrarian() {
	faction me;
	memset(&me,0,sizeof me);
	me.centreX=0;
	me.centreY=0;
	me.radius=8;
	me.baseAlignment=200;
	me.alignmentFuzz=10;
	me.aggroThreshold=200;
	me.entPlates[0]=ent_agNpc();
	return me;
}

faction fac_techno() {
	faction me;
	memset(&me,0,sizeof me);
	me.centreX=50;
	me.centreY=50;
	me.radius=45;
	me.baseAlignment=-200;
	me.alignmentFuzz=10;
	me.aggroThreshold=200;
	me.entPlates[0]=ent_techNpc();
	return me;
}

void initialiseFactions() {
	faction dummy;
	rootFaction=malloc(sizeof dummy);
	*rootFaction=fac_empty();
	guineaPig=attachFac(fac_agrarian());
	attachFac(fac_techno());
}