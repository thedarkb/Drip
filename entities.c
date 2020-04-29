entity ent_item(int type, int status, int lol1, int lol2) {
	entity me;
	memset(&me, 0, sizeof me);
	if(flagArray[status]) return me;
	me.radius=TS;
	me.health=10;
	me.behaviour=behav_item;
	me.xSub=10;
	me.ySub=10;
	me.layer=0;
	me.frame[0]=getItemSprite(type);
	me.setframe=0;
	me.status[0]=type;
	me.status[1]=status;
	me.visible=0;
	me.collisionClass=0;
	me.collider=col_item;
	return me;
}

entity ent_blobby(int lol1, int lol2, int lol3, int lol4) {
	entity me;
	memset(&me,0,sizeof me);
	me.collisionClass=2;
	me.radius=TS/2;
	me.xSub=2;
	me.ySub=1;
	me.visible=1;
	me.frame[UP]=212;
	me.frame[DOWN]=188;
	me.frame[LEFT]=196;
	me.frame[RIGHT]=204;
	me.frame[4+UP]=211;
	me.frame[4+DOWN]=187;
	me.frame[4+LEFT]=195;
	me.frame[4+RIGHT]=203;
	me.frame[8+UP]=213;
	me.frame[8+DOWN]=189;
	me.frame[8+LEFT]=197;
	me.frame[8+RIGHT]=205;
	me.health=20;
	me.layer=1;
	me.collider=col_bouncy;
	me.behaviour=behav_npcSpawn;
	return me;
}

entity ent_doorLocked(int tile, int flag,int filler, int filler2){
	entity me;
	memset(&me,0,sizeof me);
	me.frame[0]=tile;
	me.behaviour=behav_lockedDoor;
	me.status[0]=flag;
	me.health=255;
	return me;
}

