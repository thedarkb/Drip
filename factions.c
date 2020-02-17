faction fac_empty() {
	faction me;
	memset(&me,0,sizeof me);
	me.aggroThreshold=999999;
	return me;
}

faction fac_fragment(faction in, int alignment) {
	in.baseAlignment=alignment;
	in.minAg=in.maxAg=alignment;
	return in;
}

faction fac_agrarian() {
	faction me;
	memset(&me,0,sizeof me);
	me.centreX=0;
	me.centreY=0;
	me.radius=8;
	me.baseAlignment=100;
	me.alignmentFuzz=10;
	me.aggroThreshold=106;
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
	faction dummy=fac_empty();
	rootFaction=malloc(sizeof dummy);
	*rootFaction=dummy;
	attachFac(fac_agrarian());
	attachFac(fac_techno());
}