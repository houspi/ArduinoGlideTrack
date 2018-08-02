#include <AccelStepper.h>
#include <MultiStepper.h>

#define EN        8  

//Direction pin
#define X_DIR     6
#define Y_DIR     5

//Step pin
#define X_STP     3
#define Y_STP     2 

//Total steps 360 degree
// 6400 OR 3200 - 360 degree
#define T_STP     3200

byte debug = 0;

int step_full=3200;// 360 degree
int round_length=12; // cm
int rail_lenght=12; //cm
int rspeed, yspeed, xspeed; 
int ydistance, xdistance;
int ydirection, xdirection;
int rounds;

int pass_count;

AccelStepper Xaxis(1, X_STP, X_DIR); // pin X step, Y direction
AccelStepper Yaxis(1, Y_STP, Y_DIR); // pin Y step, Y direction

void setup() {
  // put your setup code here, to run once:


  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

// Set initial parameters
  round_length=12; // cm
  rail_lenght=36; //cm
  rspeed=1;

  pass_count=11;

// Calculate start parameters
  rounds = rail_lenght/round_length;
  ydistance = rounds * T_STP;
  yspeed = rspeed * T_STP;
  ydirection = 1;

  xdistance = 1200;
  xspeed = yspeed/(ydistance/xdistance);
  xdirection = 1;

// Set initial motors parameters
  Xaxis.setMaxSpeed(6400);
  Xaxis.setSpeed(xspeed);
  Xaxis.setAcceleration(xspeed);
  Xaxis.setCurrentPosition(0);

  Yaxis.setMaxSpeed(6400);
  Yaxis.setSpeed(yspeed);
  Yaxis.setAcceleration(yspeed);
  Yaxis.setCurrentPosition(0);

  if(debug==1) {
    Serial.begin(9600);
    Serial.println("Hello");
    Serial.print("rounds  ");
    Serial.println(rounds);
    Serial.print("ydistance ");
    Serial.println(ydistance);
    Serial.print("yspeed ");
    Serial.println(yspeed);
    Serial.print("ydirection ");
    Serial.println(ydirection);
    Serial.print("xspeed ");
    Serial.println(xspeed);
  }
//debug=0;
}

void loop() {
  long dtg;

  dtg = Yaxis.distanceToGo();
  if(dtg==0 && pass_count >=0) {
//    Serial.println("End Point"); 
    ydirection = (ydirection>0) ? -1 : 1;
    xdirection = (xdirection>0) ? -1 : 1;
//    Serial.print("New direction "); 
//    Serial.println(ydirection); 
//    Serial.print("pass count "); 
//    Serial.println(pass_count); 
    ydistance = ydistance * ydirection;
    xdistance = xdistance * xdirection;
    Yaxis.setCurrentPosition(0);
    Yaxis.moveTo(ydistance);
    Yaxis.setSpeed(yspeed);

    Xaxis.setCurrentPosition(0);
    Xaxis.moveTo(xdistance);
    Xaxis.setSpeed(xspeed);
    pass_count--;
  }
  if(debug==1) {
    long dtg;
//    Serial.print("loop ");
//    Serial.println(loop_count); 
//    Serial.print("ToGo ");
//    Serial.println(dtg); 
  }
  if(pass_count >=0) {
    Yaxis.runSpeedToPosition();
    Xaxis.runSpeedToPosition();
  } else {
//    Yaxis.stop();
//    Xaxis.stop();
  Xaxis.setMaxSpeed(6400);
  Xaxis.setSpeed(3200);
  Xaxis.setCurrentPosition(0);
  Xaxis.setCurrentPosition(0);
  Xaxis.moveTo(0);

  Yaxis.setMaxSpeed(6400);
  Yaxis.setSpeed(3200);
  Yaxis.setCurrentPosition(0);
  Yaxis.setCurrentPosition(0);
  Yaxis.moveTo(0);

//    Serial.println("stop work"); 
  Xaxis.setSpeed(0);
  Xaxis.setAcceleration(0);
  Yaxis.setSpeed(0);
  Yaxis.setAcceleration(0);
    digitalWrite(EN, HIGH);
    digitalWrite(X_DIR, LOW);
    digitalWrite(X_STP, LOW);
    digitalWrite(Y_DIR, LOW);
    digitalWrite(Y_STP, LOW);
  }
}

