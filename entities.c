entity ent_playerM() { //Male
	entity me={
		112,
		64,
		TS/5,
		1,
		0,
		playerBehaviour,
		1,
		0,
		2,
		5,
		1
	};
	me.x=50;
	me.y=50;
	me.xSub=0;
	me.ySub=0;
	me.behaviour=playerBehaviour;
	me.health=pMaxHealth;
	me.frame[0]=148; //up
	me.frame[1]=124; //down
	me.frame[2]=132; //left
	me.frame[3]=140; //right
	me.frame[4]=147; //up step1
	me.frame[5]=123; //down step1
	me.frame[6]=131; //left step1
	me.frame[7]=139; //right step1
	me.frame[8]=149; //up step2
	me.frame[9]=125; //down step2
	me.frame[10]=133; //left step2
	me.frame[11]=141; //right step2
	return me;
}