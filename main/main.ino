  //include libraries
#include <Dynamixel2Arduino.h>
#include <Wire.h>
#include <Servo.h>

#define KF 0.7 //accelerometer constant to reduce error

//servo ports
#define SERVO1 3 //servo port
#define SERVO2 4 //servo port
#define SERVO3 5 //servo port

//ultrasonic ports
#define ECHO1 41//echo ultrasonic left
#define TRIG1 43 //trig ultrasonic left
#define ECHO2 45 //echo ultrasonic right
#define TRIG2 47 //trig ultrasonic right
#define ECHO3 39 //echo ultrasonic top
#define TRIG3 37 //trig ultrasonic top
#define ECHO4 33//echo4 ultrasonic bottom 33
#define TRIG4 35 //trig ultrasonic bottom 35

//mercury port
#define MERC 6

//line constants
#define BLACK 500 //value of the black line
#define MIDDLE_BLACK 350 //value of the black line for the middle sensor
#define WHITE 100 //value of the white line
#define SILVER_GREEN 800
#define MWHITE 36 // Media of the refletance sensor when in white

//array ports
#define ERS A2 // external right sensor in the analog 2
#define RS A3 // Right sensor in the analog 3
#define MS A4 // Middle sensor in the analog 4
#define LS A5 // Left sensor in the analog 5
#define ELS A6 // external left sensor in the analog 6
#define R A14 // Red LED in the A14 port
#define G A13 // Green LED in the A13 port
#define B A12 // Blue LED in the A12 port
#define RCS A7 // Right color sensor analog 7
#define LCS A8 // Left color sensor analog 8

//rescue ports
#define RECS A9 // color sensor analog 7 rescue
#define RED_RESCUE 34 // Red LED in the 34 port (rescue)
#define GREEN_RESCUE 36 // Green LED in the 36 port (rescue)
#define BLUE_RESCUE 38 // Blue LED in the 38 port (rescue)

//test room3 directives

//90x90
#define R_MID_90 34.5
#define D_TRI_90 30.0

//120x120
#define R_MID_120 49.5

//old green detection
#define TOL 100 //Creates the value of tolerance for the green values
#define RRCSV 1002 // Right red sensor value when in green line
#define RGCSV 960 // Right green sensor value when in green line
#define RBCSV 987 // Right blue sensor value when in green line
#define LRCSV 985 // Left red sensor value when in green line
#define LGCSV 949 // Left green sensor value when in green line
#define LBCSV 970 // Left blue sensor value when in green line
#define REFERENCE_LEFT 855 //highest value on white + 100
#define REFERENCE_RIGHT 885 //highest value on white + 100
#define REFERENCE_DELTA_LEFT 27 //value of delta on green - 10
#define REFERENCE_DELTA_RIGHT 31 //value of delta on green - 10

//Board led ports
#define LEDG 22 //define green led port
#define LEDR 24 //define red led port

//constants of proportional, integral and derivative
#define KP 3.9  //percentage of proportional band
#define KP2 6.0 //KP for hyphotetical array
#define KPGYRO 0.675 //percentage of proportional band at gyroscope
#define KI 0.00 //inverse of integral time
#define KD 0.5 //derivative of error

//maximum and minimum values of i
#define MIN_INTEGRAL -100
#define MAX_INTEGRAL 100

//maximum and minimum values of u
#define U_MIN -2040
#define U_MAX 2040

#define WE 2.0 //outer sensors weight aproximation
#define WI 1.0 //inner sensors weight aproximation

//battery port
#define BAT A15

//dynamixel configuration
#define DEBUG_SERIAL Serial   // Serial for debug (USB)
#define DXL_SERIAL Serial3  // Serial for dynamixel

//buzzer port
#define BUZZER 8 //buzzer port

//gyroscope average error on z axis
#define GYZ_ERROR -261.42

//touch sensors ports
#define TOUCH1 31 //left touch sensor
#define TOUCH2 29 //right touch sensor

//debug button
#define BUTTON 23 

//straight walking
#define SWL 540
#define SWR 500

//battery value
int battery = 0;

//room3 variables
float val_bot = 0, val_top = 0, prev_detec_top=0, prev_detec_bot=0, prev_detec_top_2=0;
float media_movel_top[5], val_media_movel_top=0;
float diff=0, prev_diff=0;
int edge_state = 0, media_cantos[3], edge_confirm = 0;
int cont = 0, a=0, cont_tri=0, cont_canto=0;
int map_corner[3] = {-1, -1, -1}; //map_corner: front, left, right. 0 is exit, 1 triangleint map_room [8] = {0,-1,-1,-1,-1,-1,-1,-1}; //0=entrada, 1=wall
int map_room [8] = {0,-1,-1,-1,-1,-1,-1,-1}; //0=entrada, 1=wall
int mapper=1;
int cont_edge = 0, cont_cont = 0, ex=0; 
bool ra = 0, Ro=true, st_tri=0, st_confirm=0;
bool saida = false, config=false, color_detected=0, exit_state=0;
int triangle = 0, ctri=0;
int cont_a = 0; //accelerometer counter
int accel_f = 0;
int ram = 0, victims=0, dels1=0, dels2=0, turns=0, ao=0, flag_ex=-9000;
float sum_diff=0, variacao_top=0, dball=0;
int size=0;
long prev_ctri=0;
float val_left = 0.0;
float val_right =0.0;

// Test room 3 variables
float vt = 0.0 ,vb = 0.0,vl = 0.0,vr = 0.0,prev_vb = 0.0,prev_vt = 0.0, media_vb = 0.0, media_vt = 0.0, media_vl = 0.0, media_vr = 0.0, delta_vb = 0.0,delta_vt = 0.0,delta_mbt = 0.0;
int c_angle=0, entrance_angle, exit_angle=1023, tri_red, tri_green;
unsigned long delta_timer_balls = 0;

bool paredes[12], ent_m=0;

unsigned long flag = 0; //timer for pid
double previous_error=0; //"previous run" timer

//Optic variables
int ers, rs, ms, ls, els, count_u = 0;   // Middle, left, right, external left and external right sensor defined
int rr, rl, gr, gl, br, bl; // Creates the color sensor variables, in RGB for each side
int red_rescue, green_rescue, blue_rescue; // Creates the color sensor variables, in RGB (rescue)
int MIS();
int ram2 = 0; //

const int DXL_DIR_PIN = 53; // direction PIN
 
const uint8_t DXL_ID = 4;
const uint8_t DXL_ID2 = 6;
const float DXL_PROTOCOL_VERSION = 1.0;

int ent_s = 0, avs=0, balls_in_claw=0;

//dynamixel objects
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
Dynamixel2Arduino dxl2(DXL_SERIAL, DXL_DIR_PIN);

//Servo objects
Servo s1;
Servo s2;
Servo s3;

//dynamixel controltable items
using namespace ControlTableItem;

//declaring functions:

//gyroscope/aceletometer functions
void CalculateErrorGyro();
void turn(int alfa);
void oldturn(int alfa);
void Simple_turn(int alfa); 
float DetectAccel();

//Mercury sensor function
void DetectInclination();

// Optic:

//LEDs control functions
void LEDcontrol (bool r, bool g, bool b);
void LEDRescue(bool r, bool g, bool b);

//Reading LEDs functions
void readLED();
void readLEDRescue();

//Printing functions
void print_left_color();
void print_right_color();
void color_print();
void color_rescue_print();

//Array reading and printing functions
void array_read();
void array_print();

bool left_green();
bool right_green();
bool oldleft_green();
bool oldright_green();
bool dead_end();

bool in_range(int read_value, int ref_value, int tol);
int higher(int val1, int val2, int val3);
int lower(int val1, int val2, int val3);

bool finish_line();
bool finish_right();
bool finish_left();

//Board LEDs function
void Sinalize(bool gled,int blinksg,bool rled,int blinksr,int t);

// Batery alarm:
double getBatteryAlarm();
void BuzzerAlert();


// Walking functions
void PIDwalk(float a);

void walk(int left, int right);
void back(int vel);

void right(int vel);
void left(int vel);

void freeze(int del);

void walk_straight(int s);
float getDynaVelocity(int a);

//crossroad
void analyze_green();

// Obstacle and Ultrassonic
double getUltra(int u);
float getmUltra(int u, int den);
void getObstacle();
void Obstacle(char c);

//Servos 
void set_servo (int n, int f_pos);
void Servo_ipos();

// Room 3 and rescue
void Room3();
void DetectRoom();
void Deposit_alive();
void adjust_on_wall();
void go_to_middle();
// void toExit(int edge);

void dists(int d);
float getMaxLeftRight();
void distsLR();

void touch(int lim_p);

int detectTri();

// Test Room
void Troom();
void go_to_tri();
void go_to_exit();
void scan_entrance();
void scan_edge();
void scan_exit();
void get_vbt();
void print_all();
void print_vbt();
void scanner(int area_scanned,int apt,int dist_to_ball);
void go_to_angle(int f_angle);
void update_angle();

//rescue
void SwallowBalls();
void GotoBalls();

void BackMiddle(int d1, int t, int d2);

void swoosh();
void swoosh2();
void swoosh_back();
void swoosh_back2();

// Test 2:
void go_to_dist(float);
void walk_distance(float d);

void setup() {
  //Servo communication
  s1.attach(SERVO1);
  s2.attach(SERVO2);
  s3.attach(SERVO3);

  // Set initial position for the three Servos
  Servo_ipos();
  /* s3.write(80);
  s2.write(180);
  s1.write(10);*/

  //Initialize Wire communication
  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(1000000);

  //initialize communication with dynamixel
  dxl.begin(1000000);
  dxl2.begin(1000000);

  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  dxl2.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  // Get DYNAMIXEL information
  if( dxl.ping(DXL_ID) ) Serial.println("Ok (1)");
  else Serial.println("Problema (1)");

  if( dxl2.ping(DXL_ID2) ) Serial.println("Ok (2)");
  else Serial.println("Problema (2)");
  
  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID);
  dxl2.torqueOff(DXL_ID2);
  dxl.setOperatingMode(DXL_ID, OP_VELOCITY);
  dxl2.setOperatingMode(DXL_ID2, OP_VELOCITY);
  dxl.torqueOn(DXL_ID);
  dxl2.torqueOn(DXL_ID2);


  //pinmodes
  pinMode(MERC, INPUT);
  
  //ultrasonic
  pinMode(ECHO1, INPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO3, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO4, INPUT);
  pinMode(TRIG4, OUTPUT);

  //rgb rescue
  pinMode(RED_RESCUE, OUTPUT);
  pinMode(GREEN_RESCUE, OUTPUT);
  pinMode(BLUE_RESCUE, OUTPUT);

  //rgb led
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);

  //turns them off
  digitalWrite(R, 1);
  digitalWrite(G, 1);
  digitalWrite(B, 1);
  LEDRescue(0, 0, 0);

  //color sensor
  
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);

  
  //button and buzzer
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);

  //touch sensors
  pinMode(TOUCH1, INPUT);
  pinMode(TOUCH2, INPUT);
  
}

void loop() {
  s1.write(145);
  while(1)
  {
    walk(-450, -450);
  }
  Troom();
  // Serial.println(exit_angle);
  while(1);
   
  
  //debug
  while(0) {
    // print rescue led
    // Serial.println(analogRead(A1));
    // readLEDRescue();
    // Serial.println("R G B");
    // Serial.print(red_rescue);
    // Serial.print(" ");
    // Serial.print(green_rescue);
    // Serial.print(" ");
    // Serial.println(blue_rescue);
    // Serial.println(" ");

    // print array colors while walking
    // readLED();
    // walk(600, 600);
    // color_print();


    // rescue victims
    // SwallowBalls();

    // follow room without crossroad detect
    // array_read();
    // array_print();
    // PIDwalk(0.6);
    // DetectRoom();
    // dists(1);

    // print ultrasonic sensor 
    /* 
     val_top=getUltra(3);
     val_bot=getUltra(4);
     val_left=getUltra(1);
     val_right=getUltra(2);
    
     Serial.print(val_left);
     Serial.print(" ");
     Serial.print(val_right);
     Serial.print(" ");
     Serial.print(val_top);
     Serial.print(" ");
     Serial.print(val_bot);
     Serial.println(" ");     
     */

      //go_to_middle();

    // readLEDRescue();
    // color_rescue_print();
    
    //Room3();
    //Troom();
    //scanner(360);
    print_vbt();
    go_to_dist(32.0);
  }
  //Room3();
  //battery alert and array read
  BuzzerAlert();
  array_read();

  //detect when it goes up
  DetectInclination();

  //Crossroad
  if((ms >=  MIDDLE_BLACK && NOSIB() >= 2) || NOSIB()>=3) {
    //Stop the robot when enters crossroad
    back(1000);
    delay(30);
    back(500);
    delay(100);

    //analyzes green
    analyze_green();
  }

  // Normal line follower
  else {
    DetectRoom();
    PIDwalk(0.6);
    LEDcontrol(0, 0, 0);
    getObstacle();
    finish_line();
  }

  // Wait 5ms for next cycle
  delay(5);
}
