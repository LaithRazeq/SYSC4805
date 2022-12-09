//Library For Ultrasonic sensors
#include <EZDist.h>

/** Defining Pins **/
//Motors
//Rear Direction HIGH = Backwards // LOW = Forward
#define RearLeftDirection 6
#define RearLeftPower 11
#define RearRightDirection 5
#define RearRightPower 10
//Front Direction HIGH = Forward // LOW = Backward
#define FrontLeftDirection 4
#define FrontLeftPower 9
#define FrontRightDirection 3
#define FrontRightPower 8

//Ultrasonic Sensors
#define TrigPinLeft 22
#define EchoPinLeft 23
#define TrigPinRight 24
#define EchoPinRight 25

//LineFollower Sensor
#define LFS1_M A2
#define LFS1_L A4
#define LFS1_R A3

//Front IR Sensor
#define FrontSensor 13

//Using EZDist To Track Distance, Initializing the sensor pins
EZDist UltraLeft(TrigPinLeft, EchoPinLeft);
EZDist UltraRight(TrigPinRight, EchoPinRight);

//Setting Up Distance Variables and Constants
int distanceLeft = 500;
int distanceRight = 500;

//Set distances for Ultrasonic Turning
const int minDistance = 15;
const int stopDistance = 8;

//Front Sensor
bool frontSensorOnOff;
bool frontSensorFlag = 0;

//1 is right, 0 is left
bool turnDirection = 0;

//Speed Constants
const int maxSpeed = 200;
const int normalSpeed = 150;
const int halfSpeed = 100;

//Setting up LaneFollower Variables
int LFS1L;
int LFS1M;
int LFS1R;

const int frontBlackThreshold = 950;

void setup() {
//Begin Monitor  
  Serial.begin(9600);

//Settup Motor Pins
  pinMode(FrontRightDirection, OUTPUT);
  pinMode(FrontLeftDirection, OUTPUT);
  pinMode(RearRightDirection, OUTPUT);
  pinMode(RearRightPower, OUTPUT);
  pinMode(RearLeftDirection, OUTPUT);
  pinMode(RearLeftPower, OUTPUT);
  pinMode(FrontLeftDirection, OUTPUT);
  pinMode(FrontLeftPower, OUTPUT);
  pinMode(FrontRightDirection, OUTPUT);
  pinMode(FrontRightPower, OUTPUT);

//Settup Sensor Pins
  pinMode(FrontSensor, INPUT);

//Settup LineFollowerPins
  pinMode(LFS1_L, INPUT);
  pinMode(LFS1_M, INPUT);
  pinMode(LFS1_R, INPUT);
}

/** Main Loop, to be run over and over **/
void loop() {
  //Start by polling the sensors
  sensorRead();
  //Seeing if we detect black on our linefollower
  if((LFS1L > frontBlackThreshold) || (LFS1M > frontBlackThreshold) || (LFS1R > frontBlackThreshold))
  {
    blackDetected();
  }
  //If one of our sensors is too close to an object, hardAvoid it
  if((distanceLeft <= stopDistance) || (distanceRight <= stopDistance))
  {
    Serial.println("UltraSonic Stop");
    hardAvoid();
  }
  //If one of our sensors is slightly close to an object or our front sensor sees something
  if((distanceLeft <= minDistance) || (distanceRight <= minDistance) || (!frontSensorOnOff)){  
    //Front sensor detects something, we must be about to hit an object, so hard Avoid
    if((frontSensorFlag) && (!frontSensorOnOff))
    {
      Serial.println("Front Sensor Stop");
      hardAvoid();
      delay(300);
    }
    //If both ultrasonics see something, we need to back up out of there
    else if((distanceLeft < minDistance ) && (distanceRight < minDistance))
    {
      Serial.println("Both UltraSonic Stop");
      driveBwd();   
      delay(300);   
    }
    //This means only one ultrasonic sees something, so turn away from it
    else
    {
      Serial.println("UltraSonic turn");
      turn(turnDirection, normalSpeed);
      delay(300);
    }
  }
  //If we see nothing, default to driving forward
  else{
    driveFwd();
  }  
}
void sensorRead () {
  //Read Left and Right UltraSonics
  int tempLeft = UltraLeft.cm();
  int tempRight = UltraRight.cm();
  
  //Debouncing
  if((tempLeft > 0)) 
  {
    distanceLeft = tempLeft;
  }
  if((tempRight > 0))
  {
    distanceRight = tempRight;; 
  }
  //Setting turn direction
  if(distanceRight<25 || distanceLeft<25 && distanceLeft!=distanceRight)
  {
    if((distanceLeft > distanceRight))            
    {
      turnDirection = 0;        
    }
    else
    {
      turnDirection = 1;
    }
  }
  //Reading Front Sensor
  frontSensorOnOff = digitalRead(FrontSensor);

  //Front Sensor Flag
  if((frontSensorFlag == 0) && (frontSensorOnOff == 1))
  {
    frontSensorFlag = 1;   
  }

  //Read LaneFollowers
  LFS1L = analogRead(LFS1_L);
  LFS1M = analogRead(LFS1_M);
  LFS1R = analogRead(LFS1_R);
}

void blackDetected(bool dir)
{
  //Stop for a split seconf
  stop();
  delay(300);
  //Drive back
  driveBwd();
  delay(400);
  //Turn based on which sensor triggered the detection
  if(LFS1L > frontBlackThreshold)
  {
    turn(1, maxSpeed);
    delay(800);
  }
  else if(LFS1R > frontBlackThreshold)
  {
    turn(0, maxSpeed);
    delay(800);
  }
  //If it was middle, turn based on which ultrasonic sees something farther away
  else
  {
    turn(turnDirection, maxSpeed);
    delay(800);
  }
}
//Our hard avoid function, drives back and then turns
void hardAvoid()
{
  Serial.println("Hard Avoid");
  driveBwd();
  delay(400);
  stop();
  delay(300);
  turn(turnDirection, maxSpeed);
  delay(500);  
  Serial.println("HARD AVOIDED");  
}

void driveFwd()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 150);
  analogWrite(RearRightPower, 150);
  analogWrite(FrontLeftPower, 150);
  analogWrite(FrontRightPower, 150);
}
void driveBwd()
{
  digitalWrite(RearLeftDirection, HIGH);
  digitalWrite(RearRightDirection, HIGH);
  digitalWrite(FrontLeftDirection, LOW);
  digitalWrite(FrontRightDirection, LOW);

  analogWrite(RearLeftPower, 100);
  analogWrite(RearRightPower, 100);
  analogWrite(FrontLeftPower, 100);
  analogWrite(FrontRightPower, 100);
}

void stop()
{
  analogWrite(RearLeftPower, 0);
  analogWrite(RearRightPower, 0);
  analogWrite(FrontLeftPower, 0);
  analogWrite(FrontRightPower, 0);
}

//Turn based on input direction and speed
void turn(int dir, int speed)
{
  // int inv  = !dir;
  digitalWrite(RearLeftDirection, !dir);
  digitalWrite(RearRightDirection, dir);
  digitalWrite(FrontLeftDirection, dir);
  digitalWrite(FrontRightDirection, !dir);

  analogWrite(RearLeftPower, speed);
  analogWrite(RearRightPower, speed);
  analogWrite(FrontLeftPower, speed);
  analogWrite(FrontRightPower, speed);
}