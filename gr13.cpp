/**
 *
 * Grobin - Platform game starring Robin May
 *
 **/

#include <allegro.h>
#include <cmath>
#include <iostream>
using namespace std;

/* This is the timer function and integer.
 * Since they will be used throughout the entire program, basically 
 * being tested each loop, we will just declare them globally. 
 * There really isn't any waste of memory seeing as how they are used so often.
 */

//grobin, main player character
struct GROBIN
{
double x, y, momentum_h, momentum_v, x_old, y_old;
double width, height;
double jump_val;
int onsurface;
int direction;
int state;
};


//movable object
struct OBJECT
{
double x, y, momentum_h, momentum_v, x_old, y_old;
double width, height;
int onsurface;
};

//enemy characters
struct ENEMY
{
double x, y, momentum_h, momentum_v, x_old, y_old;
double width, height;
double jump_val;
int onsurface;
int direction;
};


//platform surfaces
struct SURFACE
{
int x1, x2, y1, y2;
int kind;
};




// penis
int levelw;
int levelh;



volatile long speed_counter = 0; //A long integer which will store the value of the
								 //speed counter.

				// this variable will be used for timing certain animations.
				// sorry if this isn't a sensible method of doing things
volatile long second_counter = 0;

void increment_speed_counter() //A function to increment the speed counter
{
	speed_counter++; // This will just increment the speed counter by one. :)
	second_counter++;
}
END_OF_FUNCTION(increment_speed_counter); //Make sure you tell it that it's the end of the
										  //function

// int collideborders(double &x, double &y){
int collideborders(double &x, double &y, double width, double height, double &momentum_h, double &momentum_v){

int surfacecheck;

	if(x>levelw-width){
		x=levelw-width;
		if(momentum_h>10) momentum_h = momentum_h-momentum_h*1.8;
		else momentum_h=-0;
	}	
	if(x<0){
		x=0;
		if(momentum_h<-10) momentum_h = momentum_h-momentum_h*1.8;
		else momentum_h=0;
	}
	if(y>levelh-height){
		y=levelh-height;
//	        if(momentum_v>2) momentum_v=-0.3*momentum_v;
		momentum_v=0; 
		surfacecheck = 1;
	}
//	if(y<=480-129)
	else {
		surfacecheck = 0;
	}

return (surfacecheck);
}

int collidesurfaces(double &x, double &y, double &x_old, double &y_old, double width, double height, double &momentum_h, double &momentum_v, SURFACE &surface, int j){
//  X1Y1   x2
//  
//  y2   X2Y2

int surfacecheck;

	// feet
	/*if(
		( (x>=surface.x1-width and x<surface.x2-width)
		or (x>surface.x1 and x<surface.x2) 
		or (x<surface.x1 and x>surface.x2-width) )*/
	if((x+width>surface.x1 and x<surface.x2)
		and
		( (y_old<=surface.y1-height)
		and (y>surface.y1-height) )
		){
			//cout << "feet " << j;
			y=surface.y1-height;
			if(momentum_v>0) momentum_v=0; 
			surfacecheck = 1;
	}
	else {
		surfacecheck=0;
	}
	// head
/*	if(

	( (x>surface.x1-width and x<surface.x2-width)
		or (x>surface.x1 and x<surface.x2)
		or (x<surface.x1 and x>surface.x2-width) )*/
	if((x+width>surface.x1+10 and x<surface.x2-10)
	and
		( (y_old>=surface.y2)
		and (y<surface.y2) )

		){
			cout << "head " << j;

			y=surface.y2;
			if(momentum_v<-2) momentum_v = momentum_v-momentum_v*1.8;
			else momentum_v=1;
	}
	// right side of character
/*	if(
		( (y>surface.y1-height and y<surface.y2-height)
		or (y>surface.y1 and y<surface.y2)
		or (y<surface.y1 and y>surface.y2-height) )*/
	if((y+height>surface.y1 and y<surface.y2)	
	and
		( (x_old<=surface.x1-width) 
		and (x>surface.x1-width) )	
 	) {
			//cout << "right " << j;
			x=surface.x1-width;
			if(momentum_h>10) momentum_h = momentum_h-momentum_h*1.8;
			else momentum_h=-0;
	}
	// left side
/*	if(
		( (y>surface.y1-height and y<surface.y2-height)
		or (y>surface.y1 and y<surface.y2)
		or (y<surface.y1 and y>surface.y2-height) )*/
	if((y+height>surface.y1 and y<surface.y2)			
	and
		( (x_old>=surface.x2) 
		and (x<surface.x2) )	
 	) {
			//cout << "left " << j;
			x=surface.x2;
			if(momentum_h<-10) momentum_h = momentum_h-momentum_h*1.8;
			else momentum_h=0;
	}
	/*
	// both sides
	if( (x>surface.x1-width and x<surface.x2-width) and (x>surface.x1 and x<surface.x2) and (y<surface.y1 and y>surface.y2-height) and (momentum_v>0) ) {
		if(momentum_v>0) {
			y=surface.y1-height;
			if(momentum_v>0) momentum_v=0; 
			surfacecheck = 1;
			}
		else {
			y=surface.y2;
			if(momentum_v<-2) momentum_v = momentum_v-momentum_v*1.8;
			else momentum_v=1;
		}
	}
	*/
	
return (surfacecheck);

}


int collideobjects(GROBIN &player, OBJECT &object){
//  X1Y1   x2
//  
//  y2   X2Y2
int surfacecheck;

	// feet
	/*if(        //player's right side greater than object's left side      P2>O1   PL OL PR OR
		  // and player's right side less than object's right side   P2>O2   |  |  |  |
		( (player.x+player.width>object.x and player.x+player.width<object.x+object.width)
		or
		  //player's left side greater than object's left side       P1>O1   OL PL OR PR
		  // and player's left side less than object's right side    P1<O2   |  |  |  |
		 (player.x>object.x and player.x<object.x+object.width) 
		or
		 //player's left side less than object's left side           P1<O1   PL OL OR PR
		 // and player's right side greater than object's right side P2>O2   |  |  |  |
		(player.x<object.x and player.x+player.width>object.x+object.width) )*/
	if((player.x+player.width>object.x and player.x<object.x+object.width)
		and
		( (player.y_old<=object.y-player.height)
		and (player.y>object.y-player.height) )
		){

		player.y=object.y-player.height;
		if(player.momentum_v>0) player.momentum_v=0; 
		surfacecheck = 1;
	}
	else {
		surfacecheck=0;
	}
	
	// left side
	if((player.y+player.height>object.y and player.y<object.y+player.height)			
	and
		( (player.x_old>=object.x+object.width) 
		and (player.x<object.x+object.width) )	
 	) {
		//cout << "left " << j;
		player.x=object.x+object.width;
		object.x=player.x-object.width;
		object.momentum_h=player.momentum_h;
	}

	// right side
	if((player.y+player.height>object.y and player.y<object.y+object.height)	
	and
		( (player.x_old<=object.x-player.width) 
		and (player.x>object.x-player.width) )	
 	) {
		//cout << "right " << j;
		player.x=object.x-player.width;
		object.x=player.x+player.width;
		object.momentum_h=player.momentum_h;
	}

/*	// both sides
	if( (player.x>object.x-player.width and player.x<object.x+object.width-player.width) and (player.x>object.x and player.x<object.x+object.width) and (player.y<object.y and player.y>object.y+object.width-player.height) and (player.momentum_v>0) ) {
		if(player.momentum_v>0) {
			player.y=object.y-player.height;
			if(player.momentum_v>0) player.momentum_v=0; 
			surfacecheck = 1;
			}
		else {
			object.y=object.width+player.y;
		}
	}
*/
	
	
return (surfacecheck);

}



/*
int collideobjects(GROBIN &player, OBJECT &object){

	
	//player's right side touches object's left side
	if(	(player.x+player.width > object.x)
		and 
		(object.y>player.y)
		and
		(object.y<player.y+player.height)
//		and
//		(player.momentum_h > object.momentum_h)
	){
		object.x = player.x + player.width;
//		object.momentum_h = player.momentum_h;
	}

	// player's left side touches object's right side
	if(	(player.x < object.x+object.width)
		and 
		(object.y>player.y)
		and
		(object.y<player.y+player.height)
//		and
//		(player.momentum_h > object.momentum_h)
	){
		object.x = player.x + player.width;
//		object.momentum_h = player.momentum_h;
	}
}
*/


/* Set up the general main function, and init allegro. */
int main(int argc, char *argv[])
{



	allegro_init(); // Initialize Allegro
	install_keyboard(); // Initialize keyboard routines
	
	/* You need to tell allegro to install the timer routines in order to use timers. */
	install_timer(); // Initialize the timer routines
	
	/* Ok, now heres some new code. First, we need to, as the function suggests, 
	 * lock our speed counter. Then, we need to lock the function which increments 
	 * the speed counter. Locking is used maily for historical reasons (eg: Pure DOS
	 * Allegro applications) It doesn't hurt to lock the variable and function anyway
	 * even if we're not running DOS only mode.
	 */
	LOCK_VARIABLE(speed_counter); //Used to set the timer - which regulates the game's
	LOCK_FUNCTION(increment_speed_counter);//speed.
	
	/* Next, we need to tell the computer to keep incrementing the speed counter 
	 * independent of what the program is currenlty doing with the following function. 
	 * We will also tell it how fast to go. The argument to the function BPS_TO_TIMER 
	 * will tell the computer how many 'beat's per second. Experiment with this number 
	 * to get the overall 'speed' you would like in your game. 60 works well, generally.
	 */		
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60)); //Set our BPS
	
	/* This next chunk of code is exactly the same as lesson 5. Create a buffer, 
	 * and a bitmap (and load an image into the bitmap).
	 */
	set_color_depth(16); // Set the color depth
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800,600,0,0); // Change our graphics mode to 640x480 
	
//	BITMAP *objects[0]; //Declare a BITMAP called objects[0]
//	objects[0] = load_bitmap("picture.bmp", NULL); // Load our picture

//	BITMAP *objects[0] = create_bitmap(129,136);

//	BITMAP *background = load_bitmap("bg.bmp", NULL);

	BITMAP *frames[17];
	frames[0] = load_bitmap("frame0.bmp", NULL);
	frames[1] = load_bitmap("frame1.bmp", NULL);
	frames[2] = load_bitmap("frame2.bmp", NULL);
	frames[3] = load_bitmap("frame3.bmp", NULL);  
	frames[4] = load_bitmap("frame4.bmp", NULL); 
	frames[5] = load_bitmap("frame5.bmp", NULL); 
	frames[6] = load_bitmap("frame6.bmp", NULL); 
	frames[7] = load_bitmap("frame7.bmp", NULL); 
	frames[8] = load_bitmap("frame8.bmp", NULL); 
	frames[9] = load_bitmap("frame9.bmp", NULL); 
	frames[10] = load_bitmap("frame10.bmp", NULL); 
	frames[11] = load_bitmap("frame11.bmp", NULL); 
	frames[12] = load_bitmap("frame12.bmp", NULL); 
	frames[13] = load_bitmap("frame13.bmp", NULL); 
	frames[14] = load_bitmap("frame14.bmp", NULL); 
	frames[15] = load_bitmap("frame15.bmp", NULL); 
	frames[16] = load_bitmap("stand.bmp", NULL);
	BITMAP *grobin_dash = load_bitmap("dash.bmp", NULL); 
	BITMAP *bgpic = load_bitmap("bg2.bmp", NULL); 
	BITMAP *testobject = load_bitmap("testobject.bmp", NULL); 

	// PENIS
	
	GROBIN grobin;

	int num_objects=1;
	int num_surfaces=0;

	OBJECT objects[num_objects];
	grobin.x=400;
	grobin.y = 20;
	grobin.onsurface=0;
	grobin.direction=0;
	grobin.state=0;
	grobin.momentum_h=0;
	grobin.momentum_v=0;
	grobin.jump_val=0;
	grobin.width=129;
	grobin.height=136;

	objects[0].width=100;
	objects[0].height=100;
	objects[0].x=500;
	objects[0].y=0;

	char levelarray[9][100];
	strcpy((levelarray[0]),"00000000000000000000000009");
	strcpy((levelarray[1]),"00000000000000000000000009");
	strcpy((levelarray[2]),"00000000000000000000000009");
	strcpy((levelarray[3]),"00000111100000000000111119");
	strcpy((levelarray[4]),"010000000000000000000000009");
	strcpy((levelarray[5]),"0000000000000000000000000009");
	strcpy((levelarray[6]),"0000100000000000000000000009");
	strcpy((levelarray[7]),"0100000000000000000000000019");
	strcpy((levelarray[8]),"0111111111111111111111111118");
	cout << levelarray[0] << endl;
	cout << levelarray[1] << endl;
	cout << levelarray[2] << endl;
	cout << levelarray[3] << endl;
	cout << levelarray[4] << endl;


	int penistos_x=0, penistos_y=0, peniscount=0;
	int countx=0, county=0;
	num_surfaces=0;

	//Work out how many surfaces there are
	char current=0;
/*	while(levelarray[county][countx] != atoi("8")){
		current=levelarray[county][countx];
		if(current != atoi("0") && current!=atoi("9")) num_surfaces++;
		penistos_x++;
		countx++;
		if(current==atoi("9")) { countx=0; county++;}
//		cout << countx << " " << county << endl;
	}*/
	while(levelarray[county][countx] != '8'){
		current=levelarray[county][countx];
		if(current != '0' && current!='9') num_surfaces++;
		countx++;
		if(current=='9') { countx=0; county++;}
	//	cout << countx << " " << county << endl;
	}


	//cout << (int)levelarray[0][13] << " " << atoi("9");
		cout << num_surfaces;

	//Make the surfaces
	SURFACE surfaces[num_surfaces];
	
	//Set up the surfaces
	peniscount=0;
	int peniscount_z=0;
	penistos_y=0;
	penistos_x=0;
	countx=0;
	county=0;
	while(levelarray[county][countx] != '8'){
		current = levelarray[county][countx];
		//cout << countx << " " << county << " " << current << endl;
		if(current != '0' and current != '9'){
			surfaces[peniscount_z].kind=current;
			
			surfaces[peniscount_z].x1=penistos_x;
			surfaces[peniscount_z].y1=penistos_y;
			surfaces[peniscount_z].x2=penistos_x+96;
			surfaces[peniscount_z].y2=penistos_y+96;

			penistos_x=penistos_x+96;
			countx++;
			peniscount_z++;
		}
		else{
			penistos_x=penistos_x+96;
			countx++;
			if(current == '9'){
				penistos_x=0;
				penistos_y=penistos_y+96;
				countx=0;
				county++;
			}
			
		}
		
		peniscount++;
	}
	//cout << peniscount_z << endl;

	BITMAP *buffer = create_bitmap(800,600); // Declare and create an empty bitmap in one statment.
			
	/* Declare some integers for later use */

	double gravity=1;
	double hmove=3;

//	These variables were originally called "viewpoint". They were shortened to save time typing.
	double vp_x;
	double vp_y;

	double frame_counter = 0;// A counter for which frame to draw
	double frame_counter_jump =0;

	// this can be pretty much anything
	levelw=2600;
	levelh=800;
	int bg_x, bg_y;
//	double angle;
	double maxjump_val=8;
	double jumpstart=-10;

	int dash_ok=2;
	int dash_time=0;
	double dash_two=20;
	double dash_value=10;
//	int dash_time_b=0;

					
	while(!key[KEY_ESC]) //If the user hits escape, quit the program
	{
//		if(grobin.onsurface==1) cout << frame_counter/4 << endl;
//		else cout << frame_counter_jump/4 << endl;

		/* Here is the last part of the timer routines. We want to separate the logic
		 * and the drawing portions of the program, so we crate a while loop to test 
		 * for the value of speed counter
							
		 * While the value is greater than 0 it will do the logical part, and once it 
		 * drops down to equal to (or lower than) 0, it will draw the screen. Generally 
		 * the values of speed_counter fluctuate only between 0 and 1
		 *
		 * What this does is allow for the computer to do the all the logical part of the 
		 * program before it blits to the screen, so you don't miss any keypresses, etc.
		 */
		while(speed_counter > 0)
		{
			/* This part of the code, the "series of tests" is considered the logical 
			 * part of the program. It figures out all calculations, etc. The non-logical 
			 * part of the program would be any image blitting, or sprite(ing). Therefore, 
			 * since it is a logical part of the program, it will be within the while loop 
			 * that tests to see if the counter is greater than 0. 
			 */

		cout << dash_ok << endl;
										
			/* Here is a series of tests - so we can move our bitmap around the screen */
			if(key[KEY_RIGHT])// If the user hits the right key, change the picture's X coordinate
			{
				grobin.direction=1;
				if(grobin.momentum_h<20)
				{
				grobin.momentum_h=grobin.momentum_h+hmove;
				}
//				grobin.x ++;
			}
			if(key[KEY_LEFT])// Ditto' - only for left key
			{
				grobin.direction=0;
				if(grobin.momentum_h>-20)
				{
				grobin.momentum_h=grobin.momentum_h-hmove;
				}
//				grobin.x --;// Moving left, so lower the X coordinate by 1
			}
//			if(key[KEY_UP])
			if(key[KEY_UP] && grobin.momentum_v<=1)// If the user hits the up key, change the picture's Y coordinate
			{
				//if(onsurface==1) {jump_val = 5.8; grobin.momentum_v=0;}
				if(grobin.onsurface==1){
					grobin.momentum_v=jumpstart*1.45f;
					grobin.onsurface=0;
					grobin.jump_val = maxjump_val;
					}
				if(grobin.jump_val>0.1) grobin.momentum_v=grobin.momentum_v-0.74f;
//				if(grobin.jump_val>0.1) grobin.momentum_v=grobin.momentum_v-grobin.jump_val;
				//if(grobin.momentum_v=grobin.momentum_v-jump_val/1.7f;
				
			}
			if(key[KEY_SPACE] && dash_ok>0)
			{
				if(grobin.onsurface==1){
					if(dash_ok==2){
						if(grobin.direction==1) grobin.momentum_h=+10;
						else grobin.momentum_h=-10;

						grobin.state=1;
						dash_ok--;
					}
				}
				if(grobin.direction==1) grobin.momentum_h=grobin.momentum_h+dash_two;
				else grobin.momentum_h=grobin.momentum_h-dash_two;

				if(dash_two>0.1) dash_two=dash_two-1;
				else { dash_two=0; dash_ok=0; }
			}
			/*
			if(key[KEY_DOWN])// Ditto' - only for down
			{

				if(grobin.momentum_v<20)
				{
				grobin.momentum_v++;
				}
			}
			*/
			
			/* At the end of your logic loop you will want to tell the program to decrease 
			 * your speed_counter variable. If the program passes through this loop enough times, 
			 * then it can do all your drawing sequences without missing any keypresses!
			 * Note, however, that if your logical portion of the code takes so long and the speed_counter
			 * variable is increased (via our timer incrementing funciton) more times than it is
			 * decreased, we will remain in the logical part of the code infinitely.
			 */

			// This code deals with grobin

			// slow down
			if(grobin.momentum_h>0) grobin.momentum_h=grobin.momentum_h*0.85;
			if(grobin.momentum_h<0) grobin.momentum_h=grobin.momentum_h*0.85;
			
			// if momentum_h is less than 1.6, set it to 0. Dunno what this is for
			if(fabs(grobin.momentum_h)<1.6) grobin.momentum_h=0;

			// store old values before changing current ones
			grobin.x_old=grobin.x;
			grobin.y_old=grobin.y;

			// add momentum values to location variables
			grobin.x=grobin.x+grobin.momentum_h;//*0.5;
			grobin.y=grobin.y+grobin.momentum_v;//*0.5;

			// jump stuff
			if(grobin.onsurface==1){
				grobin.jump_val = maxjump_val;
				grobin.momentum_v=0;
			}
			else if(grobin.jump_val>0) grobin.jump_val=grobin.jump_val*0.80f;
			if(grobin.jump_val<0.01) grobin.jump_val=0;

			// surface check
			grobin.onsurface=0;

			int j=0;
			while(j<num_surfaces){
			 if( (collidesurfaces(grobin.x, grobin.y, grobin.x_old, grobin.y_old, grobin.width, grobin.height, grobin.momentum_h, grobin.momentum_v, surfaces[j],j)) == 1) 
			 {
			 grobin.onsurface=1;
			 }
			 //cout << endl;
			 j++;
			}
			if( (collideborders(grobin.x, grobin.y, grobin.width, grobin.height, grobin.momentum_h, grobin.momentum_v)) == 1) grobin.onsurface=1;

			// object collisions
			int a;
			for(a=0;a<num_objects;a++){
				//double temp = collideobjects(grobin, objects[a]);
				if(collideobjects(grobin, objects[a]) == 1) grobin.onsurface=1;
			}
			
				
			// add gravity to vertical momentum
			grobin.momentum_v=grobin.momentum_v+gravity;//*0.5; 

			// This loop deals with objects
			int i=0;
			while(i<num_objects){

				// slow down
				if(objects[i].momentum_h>0) objects[i].momentum_h=objects[i].momentum_h*0.85;
				if(objects[i].momentum_h<0) objects[i].momentum_h=objects[i].momentum_h*0.85;
				
				// if momentum_h is less than 1.6, set it to 0. Dunno what this is for
				if(fabs(objects[i].momentum_h)<1.6) objects[i].momentum_h=0;

				// store old values before changing current ones
				objects[i].x_old=objects[i].x;
				objects[i].y_old=objects[i].y;
				// add momentum values to location variables
				objects[i].x=objects[i].x+objects[i].momentum_h*0.5;
				objects[i].y=objects[i].y+objects[i].momentum_v*0.5;

				// jump stuff, doesn't apply to objects because they don't jump
				/*
				if(objects[i].onsurface==1) {objects[i].jump_val = maxjump_val; objects[i].momentum_v=0;}
				else if(objects[i].jump_val>0) objects[i].jump_val=objects[i].jump_val*0.86f;
				if(objects[i].jump_val<0.01) objects[i].jump_val=0;
				*/

				// surface check
				objects[i].onsurface=0;

				int j=0;
				while(j<num_surfaces){
				 if( (collidesurfaces(objects[i].x, objects[i].y, objects[i].x_old, objects[i].y_old, objects[i].width, objects[i].height, objects[i].momentum_h, objects[i].momentum_v, surfaces[j],j)) == 1) objects[i].onsurface=1;
				 j++;
				}
				if( (collideborders(objects[i].x, objects[i].y, objects[i].width, objects[i].height, objects[i].momentum_h, objects[i].momentum_v)) == 1) objects[i].onsurface=1;

				// object collisions
				/*
				int a, b;
				for(a=0;a<num_objects;a++){
					for(b=0;b<num_objects;b++) {
						if(a!=b) collideobjects(objects[a], objects[b]);
						}
				}
				*/
				
				// add gravity to vertical momentum
				objects[i].momentum_v=objects[i].momentum_v+gravity*0.5; 

				i++;
			}

			

			speed_counter --;
			if(second_counter==60) second_counter=0;
//			frame_counter ++;
			

			// some stuff to do with the 'dash' test ability
			if(dash_two<dash_value){
				if(dash_ok>=1) dash_two=dash_two+0.25;
				else dash_two=dash_two+1;
			}				
			else dash_two=dash_value;
			if(dash_ok==0 and dash_two==dash_value and !key[KEY_SPACE]) dash_ok=2;

			if(!key[KEY_SPACE] and dash_two<dash_value) dash_ok=0;

			

			// this stuff deals with grobin's animations

			// -- start of the part that deals with grobin's 'state' --
				// this part is to do with grobin's 'state' (whether he's normal, hurt,
				// using a special move, or something that would cause him to use some special animation)
			
			// return grobin to normal after he uses his 'dash'
			if(dash_ok==0 and grobin.state==1) grobin.state=0;

			// -- end of the part that deals with grobin's 'state' --
			
			// stuff to do with changing the frame_counter variables
			
			if(abs(grobin.momentum_h)<10) frame_counter=frame_counter+fabs(grobin.momentum_h)/4.f;
			else frame_counter=frame_counter+fabs(grobin.momentum_h)/8.f;
			if(grobin.momentum_h==0) frame_counter=12;
			if(grobin.onsurface==0) frame_counter=0;
			
			if(grobin.onsurface==1) frame_counter_jump=16;
			else {
//				if(grobin.momentum_v<0){
					if(frame_counter_jump>0) frame_counter_jump-=1.5;
					else frame_counter_jump=0;
/*				}
				else {
					if(frame_counter_jump/4<8) frame_counter_jump+=2;
					if(frame_counter_jump/4>8) frame_counter_jump-=2;
					if(frame_counter_jump/4>7 and frame_counter_jump/4<8) frame_counter_jump=32;
				}*/
			}

//			if(abs(grobin.momentum_h)<10) frame_counter=frame_counter+1;
//			else frame_counter=frame_counter+2;

			if(frame_counter > 63)
				{
					frame_counter = 0;
				}
			if(frame_counter_jump > 63)
				{
					frame_counter_jump = 0;
				}

			rest(1);
		} // This is the closing bracket for the (speed_counter > 0) test
		
		/* Notice that we do all blitting and drawing in this next section of code. 
		 * We only want to draw when the speed counter is less than 0, when the computer 
		 * is finished doing enough logic. Make sure you put it before the closing bracket
		 * for the (key[KEY_ESC]) test, or else nothing will be drawn (until you hit Escape,
		 * and then the program will shut down so fast you still won't be able to see anything!
		 */


//			if(abs(grobin.momentum_h)>0) draw_sprite(buffer, frames[5], grobin.x, grobin.y);
//			else draw_sprite(buffer, frames[16], grobin.x, grobin.y);

//		blit(background, buffer, 0,0,0,0,800,600);
		
		// these indicate when the scrolling should stop, you can change these pretty freely
		int stopx1=200, stopx2=levelw-400, stopy1 = 100, stopy2=400;
		
		// these are just the dimensions of the bmp file
		int bg_w=1200;
		int bg_h=900;
		
		int xoffset = 300;
		int yoffset = 220;
		
		// 800,600 are the width and height of the screen
		int bgxmax = bg_w-800;
		int bgymax = bg_h-600;
		vp_x=grobin.x-xoffset;
		vp_y=grobin.y-yoffset;
		int vpxmin = -xoffset+stopx1, vpxmax = stopx2-xoffset;
		int vpymin = -yoffset+stopy1, vpymax = stopy2-yoffset;
		if(grobin.x<stopx1) vp_x = -xoffset+stopx1;
		if(grobin.x>stopx2) vp_x = -xoffset+stopx2;
		if(grobin.y<stopy1) vp_y = -yoffset+stopy1;
		if(grobin.y>stopy2) vp_y = -yoffset+stopy2;

		int bg_x =  (vp_x-vpxmin)/(vpxmax-vpxmin) * bgxmax;
		int bg_y =  (vp_y-vpymin)/(vpymax-vpymin) * bgymax;
		//int bg_x=0;
		blit(bgpic, buffer, bg_x,bg_y,0,0,800,600); //Draw the buffer to the screen


		//draw grobin
		if(grobin.state==0){
		if(grobin.direction==1){
			if(grobin.onsurface==1)
				{
				if(abs(grobin.momentum_h)>0) draw_sprite(buffer, frames[(int)(frame_counter/4)], grobin.x-vp_x, grobin.y-vp_y);
				else draw_sprite(buffer, frames[16], grobin.x-vp_x, grobin.y-vp_y);
				}
			else draw_sprite(buffer, frames[(int)(frame_counter_jump/4)], grobin.x-vp_x, grobin.y-vp_y);
			}
		else {
			if(grobin.onsurface==1)
				{
				if(abs(grobin.momentum_h)>0) draw_sprite_h_flip(buffer, frames[(int)(frame_counter/4)], grobin.x-vp_x, grobin.y-vp_y);
				else draw_sprite_h_flip(buffer, frames[16], grobin.x-vp_x, grobin.y-vp_y);
				}
			else draw_sprite_h_flip(buffer, frames[(int)(frame_counter_jump/4)], grobin.x-vp_x, grobin.y-vp_y);
		     }
		}
		else if(grobin.state==1){
		if(grobin.direction==1) draw_sprite(buffer, grobin_dash, grobin.x-vp_x, grobin.y-vp_y);
		else draw_sprite_h_flip(buffer, grobin_dash, grobin.x-vp_x, grobin.y-vp_y);
		}
		

		
		//draw the test object
		draw_sprite(buffer, testobject, objects[0].x-vp_x, objects[0].y-vp_y);

//		draw_sprite(buffer, objects[0], grobin.x, grobin.y); //Draw the picture to the buffer
//		rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);

		int i=0;
		while(i<num_surfaces){
			rectfill(buffer, surfaces[i].x1-vp_x, surfaces[i].y1-vp_y, surfaces[i].x2-vp_x, surfaces[i].y2-vp_y, makecol(255,0,0));
			i++;
		}

		rect(buffer, 0, 0, 10, 40, makecol(255,255,255));
		rectfill(buffer, 2, (dash_two*3.8), 8, 38, makecol(255,255,255));

		blit(buffer, screen, 0,0,0,0,800,600); //Draw the buffer to the screen
		rest(1);
		clear_bitmap(buffer); // Clear the contents of the buffer bitmap 



	} // This is the closing bracket for the (key[KEY_ESC]) test

	/* Do all the destroying, finishing parts of the program. */
	destroy_bitmap(frames[0]); //Release the bitmap data
	destroy_bitmap(frames[1]); //Release the bitmap data
	destroy_bitmap(frames[2]); //Release the bitmap data
	destroy_bitmap(frames[3]); //Release the bitmap data
	destroy_bitmap(frames[4]); //Release the bitmap data
	destroy_bitmap(frames[5]); //Release the bitmap data
	destroy_bitmap(frames[6]); //Release the bitmap data
	destroy_bitmap(frames[7]); //Release the bitmap data
	destroy_bitmap(frames[8]); //Release the bitmap data
	destroy_bitmap(frames[9]); //Release the bitmap data
	destroy_bitmap(frames[10]); //Release the bitmap data
	destroy_bitmap(frames[11]); //Release the bitmap data
	destroy_bitmap(frames[12]); //Release the bitmap data
	destroy_bitmap(frames[13]); //Release the bitmap data
	destroy_bitmap(frames[14]); //Release the bitmap data
	destroy_bitmap(frames[15]); //Release the bitmap data
	destroy_bitmap(frames[16]); //Release the bitmap data
//	destroy_bitmap(background); //FACKIN' BIG PENIS!!

//	destroy_bitmap(objects[0]); //Release the bitmap data
	destroy_bitmap(buffer); //Release the bitmap data 
	return 0; // Exit with no errors
}
END_OF_MAIN() // This must be called right after the closing bracket of your MAIN function.
			  // It is Allegro specific.
