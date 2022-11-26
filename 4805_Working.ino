#include <EZDist.h>

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
//LineFollower Sensors

//Front
#define LFS1_M A2
#define LFS1_L A4
#define LFS1_R A3
#define LFS2_M A8
#define LFS2_L A9
#define LFS2_R A7

//Other Sensors
#define FrontSensor 13

//Using EZDist To Track Distance
EZDist UltraLeft(TrigPinLeft, EchoPinLeft);
EZDist UltraRight(TrigPinRight, EchoPinRight);

//Setting Up Distance Variables and Constants
int distanceLeft = 500;
int distanceRight = 500;
const int minDistance = 17;
const int stopDistance = 10;

//Front Sensor
bool frontSensorOnOff;
bool frontSensorFlag = 0;

//1 is right, 0 is left
bool turnDirection = 0;

//Speed
const int maxSpeed = 200;
const int normalSpeed = 150;
const int halfSpeed = 100;

//Setting up LaneFollower Variables
int LFS1L;
int LFS1M;
int LFS1R;
int LFS2L;
int LFS2M;
int LFS2R;


const int frontBlackThreshold = 950;
const int rearBlackThreshold = 950;

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
  pinMode(LFS2_L, INPUT);
  pinMode(LFS2_M, INPUT);
  pinMode(LFS2_R, INPUT);
}

void loop() {
  sensorRead();
  if((LFS1L > frontBlackThreshold) || (LFS1M > frontBlackThreshold) || (LFS1R > frontBlackThreshold))
  {
    // Serial.println("FRONT LANE FOLLOWER");
    blackDetected(1);
  }
  if((LFS2L > rearBlackThreshold) || (LFS2M > rearBlackThreshold) || (LFS2R > rearBlackThreshold))
  {
    Serial.println("BACK LANE FOLLOWER");
    blackDetected(0);
  }
  if((distanceLeft <= stopDistance) || (distanceRight <= stopDistance))
  {
    Serial.println("UltraSonic Stop");
    hardAvoid();
  }
  if((distanceLeft <= minDistance) || (distanceRight <= minDistance) || (!frontSensorOnOff)){  
    if((frontSensorFlag) && (!frontSensorOnOff))
    {
      Serial.println("Front Sensor Stop");
      hardAvoid();
      delay(300);
    }
    else if((distanceLeft < minDistance ) && (distanceRight < minDistance))
    {
      Serial.println("Both UltraSonic Stop");
      driveBwd();   
      delay(300);   
    }
    else
    {
      Serial.println("UltraSonic turn");
      turn(turnDirection, normalSpeed);
      delay(300);
    }
  }
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
  //setting turn direction
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
  LFS2L = analogRead(LFS2_L);
  LFS2M = analogRead(LFS2_M);
  LFS2R = analogRead(LFS2_R);

  Serial.print("LANE 1: ");
  Serial.println(LFS2L);
  Serial.print("LANE 2: ");
  Serial.println(LFS2M);
  Serial.print("LANE 3: ");
  Serial.println(LFS2R);
}

void blackDetected(bool dir)
{
  stop();
  delay(300);
  //fwd
  if(dir)
  {
    driveBwd();
    delay(300);
    if(LFS1L > 850)
    {
      turn(1, maxSpeed);
      delay(400);
    }
    else if(LFS1R > 850)
    {
      turn(0, maxSpeed);
      delay(400);
    }
    else
    {
      turn(turnDirection, maxSpeed);
      delay(400);
    }
  }
  //bwd
  else
  {
    turn(turnDirection, maxSpeed);
    delay(300);
  }
}

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