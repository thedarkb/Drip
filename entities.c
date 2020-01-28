/*ENTITY FIELDS:

unsigned int x; //X position
unsigned int y; //Y position
unsigned char xSub; //Hitbox width (it's subtractive from tile width)
unsigned char ySub; //Hitbox height (ditto)
unsigned int behaviourId; //Pretty self explanatory.
uint16_t health;
uint32_t status; //I'd suggest bit packing this.
unsigned char inventory[INVLIMIT];
unsigned char frame; //Current frame.

IF YOU DON'T FILL ALL OF THESE, YOU'LL GET UNDEFINED BEHAVIOUR.*/

entity ent_player() {
	entity me;
	memset(&me, 0, sizeof me);
	me.behaviourId=1;
	me.x=0;
	me.y=0;
	me.health=pMaxHealth;
	me.attack=5;
	me.xSub=TS/5;
	me.ySub=1;
	me.direction=1;
	me.collisionClass=1;
	me.frame[0]=148; //up
	me.frame[1]=124; //down
	me.frame[2]=132; //left
	me.frame[3]=140; //right
	me.frame[4]=147;
	me.frame[5]=123;
	me.frame[6]=131;
	me.frame[7]=139;
	me.frame[8]=149;
	me.frame[9]=125;
	me.frame[10]=133;
	me.frame[11]=141;
	return me;
}

entity ent_aitest() {
	entity me;
	memset(&me, 0, sizeof me);
	me.behaviourId=4;
	me.xSub=2;
	me.ySub=10;
	me.direction=1;
	me.attack=2;
	me.deathframe=206;
	me.health=30;
	me.collisionClass=2;
	me.frame[0]=177;
	me.frame[1]=153;
	me.frame[2]=161;
	me.frame[3]=169;
	me.frame[4]=176;
	me.frame[5]=152;
	me.frame[6]=160;
	me.frame[7]=168;
	me.frame[8]=178;
	me.frame[9]=154;
	me.frame[10]=162;
	me.frame[11]=170;
	me.drop[0]=1;
	snapToGrid(&me);
	return me;
}

entity ent_sword(unsigned char direction, uint16_t x, uint16_t y, unsigned char creator) {
	entity me;
	memset(&me, 0, sizeof me);
	switch(direction){
		case 0:
			me.y-=TS;
			me.xSub=TS/4;
			me.ySub=1;
		break;
		case 1:
			me.y+=TS;
			me.xSub=TS/4;
			me.ySub=1;			
		break;
		case 2:
			me.x-=TS;
			me.xSub=1;
			me.ySub=TS/4;
		break;
		case 3:
			me.x+=TS;
			me.xSub=1;
			me.ySub=TS/4;
		break;
	}
	me.frame[0]=67;
	me.frame[1]=67;
	me.frame[2]=67;
	me.frame[3]=67;
	me.health=255;
	me.status[0]=10;
	me.status[1]=creator;
	me.behaviourId=9;
	me.setframe=direction;
	me.collisionClass=129;
	return me;
}

entity ent_dialogue(unsigned char direction, uint16_t x, uint16_t y) {
	entity me;
	memset(&me, 0, sizeof me);
	me.frame[0]=67;
	me.frame[1]=67;
	me.frame[2]=67;
	me.frame[3]=67;
	me.health=255;
	me.status[0]=10;
	me.status[1]=0;
	me.behaviourId=15;
	me.setframe=direction;
	me.collisionClass=129;
	return me;
}

entity ent_wall(unsigned char x, unsigned char y, unsigned char tile) {
	entity me;
	me.behaviourId=2;
	me.health=1;
	me.x=x*TS;
	me.y=y*TS;
	me.frame[0]=tile;
	me.setframe=0;
	setCollision(&tilewrapper[1][1],x,y,1);
	return me;
}

entity ent_nonsolid(unsigned char x, unsigned char y, unsigned char tile) {
	entity me;
	me.behaviourId=2;
	me.collisionClass=0;
	me.health=1;
	me.x=x*TS;
	me.y=y*TS;
	me.animation=0;
	me.direction=0;
	me.frame[0]=tile;
	me.setframe=0;
	return me;
}

entity ent_item(unsigned int x, unsigned int y, unsigned char type, unsigned char status) {
	entity me;
	memset(&me, 0, sizeof me);
	me.health=10;
	me.behaviourId=10;
	me.x=x;
	me.y=y;
	me.frame[0]=getItemSprite(type);
	me.setframe=0;
	me.status[0]=type;
	me.collisionClass=130;
	return me;
}

entity ent_empty() {
	entity me;
	memset(&me,0,sizeof me);
	return me;
}
