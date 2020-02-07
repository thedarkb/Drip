#define menuReset memset(&options, 0, sizeof options);menuText=NULL //I said I'd save ya some typing. -Ben
#define menuCall menuFlag=1;menuFirstCall=1


void diag_owl();
//29 chars in 1 line
//Long story short, this is all a dream. The assets used in this can be reused for random houses in settlements and towns.
//This is also a good intro quest because it gets the player familiarised with the movement and the interactions and the whole "The WORLD IS FUCKED" dynamic of the main story.
void diag_blob() {
    pushMsg("ALRIGHT, WHAT'S ALL THIS \nRUCKUS, THEN?\0");
    pushMsg("MUST BE COMING FROM INSIDE\nTHE SHED.\nDA PROBABLY FORGOT TO CLOSE\nTHE WINDOWS AGAIN.\0");
    pushMsg("GOTTA PUT SOMETHING ON AND\nGET DOWN THERE BEFORE\nSOMETHING HAPPENS AND DA\nBLAMES ME.\nAGAIN.\0");

    pushMsg("MUCH BETTER!\nRIGHT, ONTO THE TASK AT HAND...\0");
    pushMsg("BETTER NOT WAKE UP MA OR DA\nOR I'LL GET KILLED, OR WORSE!\0");
    pushMsg("WAIT, WHAT?\n");
    diag_owl();
}

void diag_player() {
//room, night time
	pushMsg("ALRIGHT, WHAT'S ALL THIS\nRUCKUS, THEN?\0"); 
	pushMsg("MUST BE COMING FROM INSIDE\nTHE SHED.\nDA PROBABLY FORGOT TO CLOSE\nTHE WINDOWS AGAIN.\0");
	pushMsg("GOTTA PUT SOMETHING ON AND\nGET DOWN THERE BEFORE\nSOMETHING HAPPENS AND DA\nBLAMES ME.\nAGAIN.\0");
//try and get a trigger that switches the player sprite from undies to normal fully clothed when you step in front of a wardrobe.
//also try to get a rustling clothes sound effect that isn't that big. if the GBA could have one, so can i.
	pushMsg("MUCH BETTER!\nRIGHT, ONTO THE TASK AT HAND.\0");
	pushMsg("BETTER NOT WAKE UP MA OR DA\nOR I'LL GET KILLED,\n OR WORSE!\0");
	pushMsg("WAIT, WHAT?\nI CAN'T GET DOUBLE-KILLED,\n CAN I?\0");
	pushMsg("PROBABLY ISN'T IN MY BEST INTERESTS TO FIND OUT AT THE MOMENT.");
//house layout is basically pokemon ripoff
	pushMsg("RIGHT, GOTTA SNEAK OUT TO THE\n SHED AND CLOSE THAT WINDOW.");
//                                       |
//cue quest sequence of getting out the house.
//outside, night time still.
	pushMsg("ALMOST THERE. WHAT IS MAKING\n SO MUCH NOISE THOUGH?");
//just outside of shed
	pushMsg("WAIT. THE CLANGING'S STOPPED\n STILL HAVE TO CLOSE THAT WINDOW.");
	diag_owl();
//creaking noise of shed door
//protag gets in shed
//shed is tiny, has 1 thing you can interact with on the far side, maybe get rieno to make that bit of the shed look messed up.
//protag finds cat in shed
//ben weeps silently /s
//cat is now called Jim
//because this is a dream and the protag is basically Wurzel on steroids, Jim speaks.
}
void diag_JimAndPlayer() {
	pushMsg(/*PlayerName,*/ ":\nHI THERE LITTLE GUY. HOW'D\nYOU GET IN HERE?");
	pushMsg("CAT:\nI'VE ALWAYS BEEN HERE, KID.\nWHERE ARE YOUR PARENTS?\nI MUST SPEAK TO THEM.");
	pushMsg("(FAINT VOICES IN THE DISTANCE)");
	pushMsg("VOICES:\nWE'RE DEAD, JIM...\nWE'RE DEAD, JIM.\nWE'RE DEAD, JIM!");
//maybe add flash of white for theatrical effect
	pushMsg("JIM:\n...I APPEAR TO BE TOO LATE.\nI AM SORRY, KID.");
	pushMsg("JIM:\nGOOD LUCK.\nSEARCH FOR THE LIGHT AT THE\nEND OF THE TUNNEL.\nWAKE UP NOW.\nIT WAS ALL A DREAM.");
	diag_owl();
					//He's dead, Jim.
//fade to white
//fade back to cave.
}
//fade into protag's pal in cave, Jim, waking protag up.
//jim is humanoid but not exactly human maybe ex-robot serf depending on where you want to go with this.
//Jim can also reappear in towns or something if you like to give you pointers. 
void diag_PlayerAndJim() {
	pushMsg("JIM:\nWAKE UP MAN, WE HAVE A LONG,\nHARD, THROBBING PATH AHEAD\nOF US.");
	pushMsg(/*PlayerName,*/ ":\n...LIGHT AT...\n ...END OF TUNNEL...");
	diag_owl();
}

void diag_menuTest() {
		pushMsg("Test dialogues.\0");
	    menuReset; //CALL THIS FOR EVERY MENU
	    options[0]=diag_player; //The function it will run if you select the first option. Each menu can have up to six options.
	    options[1]=diag_JimAndPlayer;
	    options[2]=diag_PlayerAndJim;
	    menuText="Player\nJim/Player\nPlayer/Jim\nQuit\0"; //Each new line is considered an option. Blank options just exit the menu.
	    menuCall; //Putting stuff in the function after this call may cause weird shit.
}

void diag_owl() {
	pushMsg("Would you like to return to\nthe dialogue test menu?\0");
	menuReset;
	options[0]=diag_menuTest;
	menuText="Yes\nNo\0";
	menuCall;
}