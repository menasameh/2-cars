/*
* Program : 2-Cars game implementation on a vga screen 
* Inputs  : dual-joyStick controller  
* Outputs : the game on vga screen
*/
#include <VGAX.h>
#include <math.h>
#include <EEPROM.h>
#include <VGAXUtils.h>


//joyStick inputs
#define JOYSTICK_LEFT A1
#define JOYSTICK_RIGHT A2

////////////////////////////// Constants //////////////////////////////
//start of game border 
#define border_start_x 36
#define border_start_y 1
//quarter = 0.25 * width of border
#define quarter ((VGAX_WIDTH - border_start_x)/4)
//inner border lines
#define left_lane (border_start_x + quarter)
#define separator (border_start_x + 2 * quarter)
#define right_lane (border_start_x + 3 * quarter)
//object positions
#define pos_1 10
#define pos_2 32
#define pos_3 53
#define pos_4 73

VGAX vga;
VGAXUtils vgaU;

////////////////////////////// Font //////////////////////////////
//font height for typing strings on screen (used for printPROGMEM)
#define FNT_NANOFONT_HEIGHT 6
//number of symbols in the character set "fnt_nanofont_data" (used for printPROGMEM)
#define FNT_NANOFONT_SYMBOLS_COUNT 95

//data size=570 bytes
const unsigned char fnt_nanofont_data[FNT_NANOFONT_SYMBOLS_COUNT][1+FNT_NANOFONT_HEIGHT] PROGMEM={
{ 1, 128, 128, 128, 0, 128, 0, }, //glyph '!' code=0
{ 3, 160, 160, 0, 0, 0, 0, }, //glyph '"' code=1
//{ 5, 80, 248, 80, 248, 80, 0, },  //glyph '#' code=2-
{ 5, 248, 248, 248, 248, 248, 0, },  //glyph '#' code=2 - full rectangle
{ 5, 120, 160, 112, 40, 240, 0, },  //glyph '$' code=3
{ 5, 136, 16, 32, 64, 136, 0, },  //glyph '%' code=4
{ 5, 96, 144, 104, 144, 104, 0, },  //glyph '&' code=5
{ 2, 128, 64, 0, 0, 0, 0, },  //glyph ''' code=6
{ 2, 64, 128, 128, 128, 64, 0, }, //glyph '(' code=7
{ 2, 128, 64, 64, 64, 128, 0, },  //glyph ')' code=8
{ 3, 0, 160, 64, 160, 0, 0, },  //glyph '*' code=9
{ 3, 0, 64, 224, 64, 0, 0, }, //glyph '+' code=10
{ 2, 0, 0, 0, 0, 128, 64, },  //glyph ',' code=11
{ 3, 0, 0, 224, 0, 0, 0, }, //glyph '-' code=12
{ 1, 0, 0, 0, 0, 128, 0, }, //glyph '.' code=13
{ 5, 8, 16, 32, 64, 128, 0, },  //glyph '/' code=14
{ 4, 96, 144, 144, 144, 96, 0, }, //glyph '0' code=15
{ 3, 64, 192, 64, 64, 224, 0, },  //glyph '1' code=16
{ 4, 224, 16, 96, 128, 240, 0, }, //glyph '2' code=17
{ 4, 224, 16, 96, 16, 224, 0, },  //glyph '3' code=18
{ 4, 144, 144, 240, 16, 16, 0, }, //glyph '4' code=19
{ 4, 240, 128, 224, 16, 224, 0, },  //glyph '5' code=20
{ 4, 96, 128, 224, 144, 96, 0, }, //glyph '6' code=21
{ 4, 240, 16, 32, 64, 64, 0, }, //glyph '7' code=22
{ 4, 96, 144, 96, 144, 96, 0, },  //glyph '8' code=23
{ 4, 96, 144, 112, 16, 96, 0, },  //glyph '9' code=24
{ 1, 0, 128, 0, 128, 0, 0, }, //glyph ':' code=25
{ 2, 0, 128, 0, 0, 128, 64, },  //glyph ';' code=26
{ 3, 32, 64, 128, 64, 32, 0, }, //glyph '<' code=27
{ 3, 0, 224, 0, 224, 0, 0, }, //glyph '=' code=28
{ 3, 128, 64, 32, 64, 128, 0, },  //glyph '>' code=29
{ 4, 224, 16, 96, 0, 64, 0, },  //glyph '?' code=30
{ 4, 96, 144, 176, 128, 112, 0, },  //glyph '@' code=31
{ 4, 96, 144, 240, 144, 144, 0, },  //glyph 'A' code=32
{ 4, 224, 144, 224, 144, 224, 0, }, //glyph 'B' code=33
{ 4, 112, 128, 128, 128, 112, 0, }, //glyph 'C' code=34
{ 4, 224, 144, 144, 144, 224, 0, }, //glyph 'D' code=35
{ 4, 240, 128, 224, 128, 240, 0, }, //glyph 'E' code=36
{ 4, 240, 128, 224, 128, 128, 0, }, //glyph 'F' code=37
{ 4, 112, 128, 176, 144, 112, 0, }, //glyph 'G' code=38
{ 4, 144, 144, 240, 144, 144, 0, }, //glyph 'H' code=39
{ 3, 224, 64, 64, 64, 224, 0, },  //glyph 'I' code=40
{ 4, 240, 16, 16, 144, 96, 0, },  //glyph 'J' code=41
{ 4, 144, 160, 192, 160, 144, 0, }, //glyph 'K' code=42
{ 4, 128, 128, 128, 128, 240, 0, }, //glyph 'L' code=43
{ 5, 136, 216, 168, 136, 136, 0, }, //glyph 'M' code=44
{ 4, 144, 208, 176, 144, 144, 0, }, //glyph 'N' code=45
{ 4, 96, 144, 144, 144, 96, 0, }, //glyph 'O' code=46
{ 4, 224, 144, 224, 128, 128, 0, }, //glyph 'P' code=47
{ 4, 96, 144, 144, 144, 96, 16, },  //glyph 'Q' code=48
{ 4, 224, 144, 224, 160, 144, 0, }, //glyph 'R' code=49
{ 4, 112, 128, 96, 16, 224, 0, }, //glyph 'S' code=50
{ 3, 224, 64, 64, 64, 64, 0, }, //glyph 'T' code=51
{ 4, 144, 144, 144, 144, 96, 0, },  //glyph 'U' code=52
{ 3, 160, 160, 160, 160, 64, 0, },  //glyph 'V' code=53
{ 5, 136, 168, 168, 168, 80, 0, },  //glyph 'W' code=54
{ 4, 144, 144, 96, 144, 144, 0, },  //glyph 'X' code=55
{ 3, 160, 160, 64, 64, 64, 0, },  //glyph 'Y' code=56
{ 4, 240, 16, 96, 128, 240, 0, }, //glyph 'Z' code=57
{ 2, 192, 128, 128, 128, 192, 0, }, //glyph '[' code=58
{ 5, 128, 64, 32, 16, 8, 0, },  //glyph '\' code=59
{ 2, 192, 64, 64, 64, 192, 0, },  //glyph ']' code=60
{ 5, 32, 80, 136, 0, 0, 0, }, //glyph '^' code=61
{ 4, 0, 0, 0, 0, 240, 0, }, //glyph '_' code=62
{ 2, 128, 64, 0, 0, 0, 0, },  //glyph '`' code=63
{ 3, 0, 224, 32, 224, 224, 0, },  //glyph 'a' code=64
{ 3, 128, 224, 160, 160, 224, 0, }, //glyph 'b' code=65
{ 3, 0, 224, 128, 128, 224, 0, }, //glyph 'c' code=66
{ 3, 32, 224, 160, 160, 224, 0, },  //glyph 'd' code=67
{ 3, 0, 224, 224, 128, 224, 0, }, //glyph 'e' code=68
{ 2, 64, 128, 192, 128, 128, 0, },  //glyph 'f' code=69
{ 3, 0, 224, 160, 224, 32, 224, },  //glyph 'g' code=70
{ 3, 128, 224, 160, 160, 160, 0, }, //glyph 'h' code=71
{ 1, 128, 0, 128, 128, 128, 0, }, //glyph 'i' code=72
{ 2, 0, 192, 64, 64, 64, 128, },  //glyph 'j' code=73
{ 3, 128, 160, 192, 160, 160, 0, }, //glyph 'k' code=74
{ 1, 128, 128, 128, 128, 128, 0, }, //glyph 'l' code=75
{ 5, 0, 248, 168, 168, 168, 0, }, //glyph 'm' code=76
{ 3, 0, 224, 160, 160, 160, 0, }, //glyph 'n' code=77
{ 3, 0, 224, 160, 160, 224, 0, }, //glyph 'o' code=78
{ 3, 0, 224, 160, 160, 224, 128, }, //glyph 'p' code=79
{ 3, 0, 224, 160, 160, 224, 32, },  //glyph 'q' code=80
{ 3, 0, 224, 128, 128, 128, 0, }, //glyph 'r' code=81
{ 2, 0, 192, 128, 64, 192, 0, },  //glyph 's' code=82
{ 3, 64, 224, 64, 64, 64, 0, }, //glyph 't' code=83
{ 3, 0, 160, 160, 160, 224, 0, }, //glyph 'u' code=84
{ 3, 0, 160, 160, 160, 64, 0, },  //glyph 'v' code=85
{ 5, 0, 168, 168, 168, 80, 0, },  //glyph 'w' code=86
{ 3, 0, 160, 64, 160, 160, 0, },  //glyph 'x' code=87
{ 3, 0, 160, 160, 224, 32, 224, },  //glyph 'y' code=88
{ 2, 0, 192, 64, 128, 192, 0, },  //glyph 'z' code=89
{ 3, 96, 64, 192, 64, 96, 0, }, //glyph '{' code=90
{ 1, 128, 128, 128, 128, 128, 0, }, //glyph '|' code=91
{ 3, 192, 64, 96, 64, 192, 0, },  //glyph '}' code=92
{ 3, 96, 192, 0, 0, 0, 0, },  //glyph '~' code=93
{ 4, 48, 64, 224, 64, 240, 0, },  //glyph '£' code=94
};

////////////////////////////// Constant Strings //////////////////////////////
static const char str0[] PROGMEM="0"; 
static const char str1[] PROGMEM="1"; 
static const char str2[] PROGMEM="2"; 
static const char str3[] PROGMEM="3"; 
static const char str4[] PROGMEM="4"; 
static const char str5[] PROGMEM="5"; 
static const char str6[] PROGMEM="6"; 
static const char str7[] PROGMEM="7"; 
static const char str8[] PROGMEM="8"; 
static const char str9[] PROGMEM="9";
static const char str10[] PROGMEM="#";
static const char str22[] PROGMEM="Game Over"; 
static const char str23[] PROGMEM="Score"; 
static const char str24[] PROGMEM="Max"; 



///////////////////////////// Object //////////////////////////////
/*
 x    : x position of the object [pos1, pos2, pos3, pos4]
 y    : y position of the object [1<=y<=35]
 type : type of the object       [0=circle, 1=rectangle]
*/
struct object{
  byte x;
  byte y;
  byte type;
};
//x-positions of objects 
byte pos[]={pos_1, pos_2, pos_3, pos_4};
//variable used to keep the distance between the objects at minimum 10
byte ticks=0;
/*
                player : variable saving the state of the two cars
                               bit 0 -> state of left car 
                               bit 1 -> state of right car
            +--------------+------------------------+-------------------------+
            |player bit\car|        bit = 0         |       bit = 1           |
            +--------------+------------------------+-------------------------+
            |    bit 0     | left car on left lane  | left car on right lane  |
            +--------------+------------------------+-------------------------+
            |    bit 1     | right car on left lane | right car on right lane |
            +--------------+------------------------+-------------------------+
*/
byte player=0;
/*
 state : variable to indicate the status of the game [begining, running, game over]
*/
byte state = 1;
//score of the current game
byte score = 0; 
//the maximum score achieved for all the games played, saved in EEPROM
byte score_max; 
//the base delay between game turns
byte speedDelay = 100; 


//pool of objects to reduce memory usage
object objects[5];
//variable containing the index of the next object in the pool 
byte curr_object=0;

///////////////////////////// functions //////////////////////////////

void setup() {
  vga.begin();
  randomSeed(analogRead(5));
  //reading the max score from address 0 in EEPROM
  score_max = EEPROM.read(0);
  //initialaise the pool of objects
  clear_objects();
}


void clear_objects(){
for(short i=0;i<5;++i){
	//an object with x = 0 is considered in pool 
	//other than that the object is in the game 
    objects[i].x=0;
  }
}

/*
 function to initiate objects from pool to the game
*/
void object_init() {
/*
  if the current object is in the game 
  random(100) > 50 ==> is to reduce number of objects initiated
  ticks++ <5       ==> is to make sure that the minimum y distance 
                       between 2 objects is 7
*/
  if(objects[curr_object].x!=0 || random(100) > 50 || ticks++ <7)
    return;
  objects[curr_object].x    = border_start_x+pos[random(4)];
  objects[curr_object].y    = border_start_y;
  // get a random object type a circle or a rectangle 
  objects[curr_object].type = random(2);
  //advance the curr_object pointer to the next place
  curr_object=(curr_object+1)%5;
  //reset the ticks variable
  ticks=0;
}


/*
 function to draw the object according to its type
*/
void draw_object(short pos){
  if(objects[pos].type==1){
    vgaU.draw_rect(objects[pos].x-4, objects[pos].y, 8, 8, 2,2);
  }
  else{
    vgaU.draw_circle(objects[pos].x, objects[pos].y+4, 4, 2,2);
  }
}

/*
 function to clear the object at position pos 
 by drawing a rectangle with color 0(black) on top of it
*/
void clear_object(short pos){
    vgaU.draw_rect(objects[pos].x-4, objects[pos].y, 9, 8, 0,0);
}


/*
 function to draw the objects one by one
*/
void draw_objects(){
  for(short i =0;i<5;++i){
  	//if the object is in the game draw it
    if(objects[i].x!=0)
      draw_object(i);
  }
}


/*
 function to check if the object facing 
 the player car is in the right place 
*/
void check_collision(short i){
  switch(objects[i].x-border_start_x){
  	//object on the first lane(left lane of left car)
    case pos_1:
      //left car on left lane and object is circle
      if(!(player&1) && objects[i].type==0){
        ++score;
        objects[i].x=0;
        return;
      }
      //left car on right lane and object is rectangle
      else if((player&1) && objects[i].type==1){
        objects[i].x=0;
        return;
      }
      break;
     //object on the second lane(right lane of left car)
     case pos_2:
      //left car on right lane and object is circle
      if((player&1) && objects[i].type==0){
        ++score;
        objects[i].x=0;
        return;
      }
      //left car on left lane and object is circle
      else if(!(player&1) && objects[i].type==1){
        objects[i].x=0;
        return;
      }
      break;
     //object on the third lane(left lane of right car)
     case pos_3:
      //right car on left lane and object is circle
      if(!((player&2)>>1) && objects[i].type==0){
        ++score;
        objects[i].x=0;
        return;
      }
      //right car on right lane and object is rectangle
      else if(((player&2)>>1) && objects[i].type==1){
        objects[i].x=0;
        return;
      }
      break;
     //object on the fourth lane(right lane of right car)
     case pos_4:
      //right car on right lane and object is circle
      if(((player&2)>>1) && objects[i].type==0){
        ++score;
        objects[i].x=0;
        return;
      }
      //right car on left lane and object is rectangle
      else if(!((player&2)>>1) && objects[i].type==1){
        objects[i].x=0;
        return;
      }
      break;
  }
  //object is circle and the car is not under it 
  //or the object is rectangle and the car under it 
  //then it's end of the game switching to state 3
  state = 3;
}


/*
 function to update the y value of each object
*/
void update_objects(){
  for(short i=0;i<5;++i){
    if(objects[i].x!=0){
      clear_object(i);
      short value = objects[i].y;
      //car on top of the cars 
      if(value>35){
         check_collision(i);
         return;
        } 
      objects[i].y += 1;
    }
  }
}


/*
 function to draw the cars 
 num = 0 --> draw left car
 num = 1 --> draw right car
*/
void draw_car(int num){
  /*
   x-position of the car is as the following 
   border_start_x+7       --> to get the first lane position
   (42*(num))             --> to select between left car and right car
   (21*((player>>num)&1)) --> to select between left lane and right lane
  */
  int x=border_start_x+7+(21*((player>>num)&1))+(42*(num)), y=VGAX_HEIGHT-15;
  vgaU.draw_line(x, y, x, y+4, 2);
  vgaU.draw_line(x+1, y, x+1, y+4, 2);
  
  vgaU.draw_line(x+6, y, x+6, y+4, 2);
  vgaU.draw_line(x+7, y, x+7, y+4, 2);
  
  vgaU.draw_line(x+2, y+2, x+2, y+11, (1+2*(num)));
  vgaU.draw_line(x+3, y+2, x+3, y+11, (1+2*(num)));
  vgaU.draw_line(x+4, y+2, x+4, y+11, (1+2*(num)));
  vgaU.draw_line(x+5, y+2, x+5, y+11, (1+2*(num)));
  
  vgaU.draw_line(x, y+8, x, y+14, 2);
  vgaU.draw_line(x+1, y+8, x+1, y+14, 2);

  vgaU.draw_line(x+6, y+8, x+6, y+14, 2);
  vgaU.draw_line(x+7, y+8, x+7, y+14, 2);
}

/*
 function to clear the car at position num
*/
void clear_car(short num){
  int x=border_start_x+num, y=VGAX_HEIGHT-15;
  vgaU.draw_rect(x, y, 8, 13, 0, 0);
}


/*
 function to draw the two cars
*/
void draw_cars(){
  draw_car(0);
  draw_car(1);
}

/*
 function to interface the 'clear_car' function 
*/
void clear_cars(short i){
  switch(i){
    case 0:
      clear_car(7);
      break;
    case 1:
      clear_car(28);
      break;
    case 2:
      clear_car(49);
      break;
    case 3:
      clear_car(70);
      break;
  }
}

/*
 function to get the input from the two joysticks 
 and take the required actions 
*/
bool processInputs() {
  /*
   value of the analogRead of the  joystick is as the following 
   +--------+---------+----------+----------+                                   
   |        |   ()    |   ()     |      ()  |
   |        |   ||    |    \\    |     //   | 
   |        |+------+ | +------+ | +------+ | 
   |joyStick||      | | |      | | |      | | 
   |        |+------+ | +------+ | +------+ | 
   +--------+---------+----------+----------+
   | value  |   500   |  0 -400  | 600-1000 |
   +--------+---------+----------+----------+

  */
  int value = analogRead(JOYSTICK_LEFT);
  //ret is to indicate whether any key is pressed or not
  bool ret = false;
  if(value <=400){
  	//if the car is in the opposite side
  	//clear the car to be drawn in the other lane 
    if(bitRead(player,0)){
       clear_cars(1);
    }
    bitClear(player, 0);
    ret = true;
  }
  else if(value >= 600){
  	//if the car is in the opposite side
  	//clear the car to be drawn in the other lane 
    if(!bitRead(player,0)){
       clear_cars(0);
    }
    bitSet(player, 0);
    ret = true;
  }

  value = analogRead(JOYSTICK_RIGHT); 
  if(value <= 400){
  	//if the car is in the opposite side
  	//clear the car to be drawn in the other lane 
    if(bitRead(player,1)){
       clear_cars(3);
    }
   bitClear(player, 1); 
  ret = true;
  }
  else if(value >= 600){
  	//if the car is in the opposite side
  	//clear the car to be drawn in the other lane 
    if(!bitRead(player,1)){
       clear_cars(2);
    }
   bitSet(player, 1);
   ret = true;
  }
  return ret;
}

/*
 function to draw the borders and the inner lines
*/
void drawBorder() {
    //outer lines
    vgaU.draw_line(border_start_x, 0, VGAX_WIDTH-1, 0, 3);
    vgaU.draw_line(border_start_x, VGAX_HEIGHT-1, VGAX_WIDTH-1, VGAX_HEIGHT-1, 3);
    vgaU.draw_line(border_start_x, 0, border_start_x, VGAX_HEIGHT-1, 3);
    vgaU.draw_line(VGAX_WIDTH-1, 0, VGAX_WIDTH-1, VGAX_HEIGHT, 3);
    //inner lines
    vgaU.draw_line(left_lane, 0, left_lane, VGAX_HEIGHT, 3);
    vgaU.draw_line(separator, 0, separator, VGAX_HEIGHT, 3);
    vgaU.draw_line(separator+1, 0, separator+1, VGAX_HEIGHT, 3);
    vgaU.draw_line(right_lane, 0, right_lane, VGAX_HEIGHT, 3);
}

/*
 function to draw a number on the screen
 i  : the number to be printed
 x,y: position of the first digit from the right
*/
void draw_int(byte i, byte x, byte y){
  byte shift=0;
  if(i==0){
    vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str0, x - shift*5, y, 2);
    return;
  }
  while(i>0){
    byte val = i%10;
                   //clear the current position
                   vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1,str10, x - shift*5, y, 0);
    if (val == 0) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str0, x - shift*5, y, 2);}
    if (val == 1) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str1, x - shift*5, y, 2);}
    if (val == 2) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str2, x - shift*5, y, 2);}
    if (val == 3) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str3, x - shift*5, y, 2);}
    if (val == 4) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str4, x - shift*5, y, 2);}
    if (val == 5) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str5, x - shift*5, y, 2);}
    if (val == 6) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str6, x - shift*5, y, 2);}
    if (val == 7) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str7, x - shift*5, y, 2);}
    if (val == 8) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str8, x - shift*5, y, 2);}
    if (val == 9) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str9, x - shift*5, y, 2);}
    i/=10;
    shift++;
  }
}

/*
 function to draw the word "score" and the current score
*/
void drawScore() {
  vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str23, 10, 3, 2);
  draw_int(score, 20, 10);
}


/*
 function to draw the word "Max" and the max score
*/
void draw_score_max() {
  vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str24, 13, 30, 2);
  draw_int(score_max, 20, 37);
}



void drawStartScreen() {
   vga.clear(0);
   drawBorder(); 
   drawScore(); 
   vga.delay(200);
} 

/*
 function to re-initialise the game
*/
void newMatch(){
  score = 0; 
  vga.clear(0); 
  clear_objects(); 
  drawBorder();
  drawScore();
  draw_score_max();   
  state = 1;
}




void loop() {
 //the game is in the begining (initialisation)
 if(state == 1) { 
	vga.delay(10);
	vga.clear(0);
	drawStartScreen(); 
	draw_score_max(); 
	draw_cars();
	state = 2;
  }
 // the game is running 
 if(state == 2){
  processInputs();
  drawScore(); 
  draw_cars();
  object_init();
  update_objects();
  draw_objects();
  //delay between the game turns decreases when score is increased
  vga.delay(max(20,speedDelay-score*3)); 
 }
 //player has lost 
 if(state == 3){
    vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str22, 58, 24, 1);
    vga.delay(1000);
    //if the score is bigger than max_score write it to EEPROM address 0
    if(score > score_max){
      EEPROM.write(0, score);
      score_max = score;
    } 
    //take any input to continue
    while(1){if(processInputs())break;}
    newMatch();
  }
} 

