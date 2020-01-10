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
	me.behaviourId=1;
	me.x=280;
	me.y=20;
	me.xSub=14;
	me.ySub=1;
	me.direction=1;
	me.collisionClass=BIT(0);
	me.frame[0]=148;
	me.frame[1]=124;
	me.frame[2]=132;
	me.frame[3]=140;
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
	me.behaviourId=4;
	me.xSub=2;
	me.ySub=10;
	me.direction=1;
	me.attack=2;
	me.collisionClass=BIT(7);
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
	snapToGrid(&me);
	return me;
}

entity ent_wall(unsigned char x, unsigned char y, unsigned char tile) {
	entity me;
	me.behaviourId=2;
	me.x=x*TS;
	me.y=y*TS;
	me.frame[0]=tile;
	me.setframe=0;
	setCollision(x,y,1);
	return me;
}

entity ent_nonsolid(unsigned char x, unsigned char y, unsigned char tile) {
	entity me;
	me.behaviourId=2;
	me.x=x*TS;
	me.y=y*TS;
	me.animation=0;
	me.direction=0;
	me.frame[0]=tile;
	me.setframe=0;
	return me;
}