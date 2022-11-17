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
#define LFS1_M 34
#define LFS1_L 32
#define LFS1_R 33

//Front Right
#define LFS2_R 30
#define LFS2_M 31
#define LFS2_L 41

//Other Sensors
#define FrontSensor 13

//Using EZDist To Track Distance
EZDist UltraLeft(TrigPinLeft, EchoPinLeft);
EZDist UltraRight(TrigPinRight, EchoPinRight);

//Setting Up Distance Variables and Constants
int distanceLeft = 500;
int distanceRight = 500;
const int minDistance = 15;
const int stopDistance = 10;

//Front Sensor
bool frontSensorOnOff;
bool frontSensorFlag = 0;

//1 is right, 0 is left
bool turnDirection = 0;

//Speed
const int maxSpeed = 200;
const int halfSpeed = 100;

//Setting up LaneFollower Variables
bool LFS1L;
bool LFS1M;
bool LFS1R;
bool LFS2L;
bool LFS2M;
bool LFS2R;

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
  //sensorTest();
  sensorRead();
  //motorTest();  
  if((distanceLeft <= stopDistance) || (distanceRight <= stopDistance))
  {
    driveBwd();
    delay(300);
    if(turnDirection)
    {   
      // turnRight();
      turn(turnDirection);
    }
    else
    {
      // turnLeft();
      turn(turnDirection);
    }
    delay(300);
  }
  if((distanceLeft <= minDistance) || (distanceRight <= minDistance) || (!frontSensorOnOff)){  
    if((frontSensorFlag) && (!frontSensorOnOff))
    {
      hardAvoid();
    }
    else if((distanceLeft < minDistance ) && (distanceRight < minDistance))
    {
      driveBwd();      
    }
    else if(distanceLeft <= minDistance)
    {
      // turnRight();
      turn(turnDirection);
    }
    else if(distanceRight <= minDistance)
    {
      // turnLeft();
      turn(turnDirection);
    }
  }
  else{
    driveFwd();
  }  
}

void motorTest()
{
  stop();
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, maxSpeed);
  delay(2000);
  analogWrite(RearLeftPower, 0);
  analogWrite(RearRightPower, maxSpeed);
  delay(2000);
  analogWrite(RearRightPower, 0);
  analogWrite(FrontLeftPower, maxSpeed);
  delay(2000);
  analogWrite(FrontLeftPower, 0);  
  analogWrite(FrontRightPower, maxSpeed);
  delay(2000);
  stop();
}

void sensorTest()
{
  distanceRight = UltraRight.cm();  
  distanceLeft = UltraLeft.cm();
  if((UltraLeft.cm() > 0) && (UltraLeft.cm() < 500)) 
  {
    distanceLeft = UltraLeft.cm();
    //Serial.print("Left: ")  ;
    //Serial.println(distanceLeft);
  }

  if((UltraRight.cm() > 0) && (UltraRight.cm() < 500))
  {
    distanceRight = UltraRight.cm();
    //Serial.print("Right: ")  ;
    //Serial.println(distanceRight); 
  }
  delay(1000);
}

void sensorRead () {
  //Read Left and Right UltraSonics

  //frontSensorFlag = 0;

  int tempLeft = UltraLeft.cm();
  int tempRight = UltraRight.cm();
  
  if((tempLeft > 0)) 
  {
    distanceLeft = tempLeft;
    // Serial.print("Left: ")  ;
    // Serial.println(distanceLeft);
  }

  if((tempRight > 0))
  {
    distanceRight = tempRight;
    // Serial.print("Right: ") ;
    // Serial.println(distanceRight); 
  }
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
  frontSensorOnOff = digitalRead(FrontSensor);

  //Front Sensor
  if((frontSensorFlag == 0) && (frontSensorOnOff == 1))
  {
    frontSensorFlag = 1;   
  }

  //Serial.print("Front: ");
  //Serial.println(frontSensorOnOff);

  //Read LaneFollowers
  LFS1L = digitalRead(LFS1_L);
  LFS1M = digitalRead(LFS1_M);
  LFS1R = digitalRead(LFS1_R);
  LFS2L = digitalRead(LFS2_L);
  LFS2M = digitalRead(LFS2_M);
  LFS2R = digitalRead(LFS2_R);
}

void hardAvoid()
{
  Serial.println("Hard Avoid");
  driveBwd();
  delay(300);
  // if(turnDirection)
  // {   
  //   while((!frontSensorOnOff))
  //   {
  //     turnRight();
  //     sensorRead();
  //   }
  // }
  // else
  // {
    while((!frontSensorOnOff))
    {
      // turnLeft();
      turn(turnDirection);
      sensorRead();
    }    
  // }
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

void veerRight()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 250);
  analogWrite(RearRightPower, 0);
  analogWrite(FrontLeftPower, 250);
  analogWrite(FrontRightPower, 0);
}
void veerLeft()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 0);
  analogWrite(RearRightPower, 250);
  analogWrite(FrontLeftPower, 0);
  analogWrite(FrontRightPower, 250);
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

// void turnRight()
// {
//   digitalWrite(RearLeftDirection, LOW);
//   digitalWrite(RearRightDirection, HIGH);
//   digitalWrite(FrontLeftDirection, HIGH);
//   digitalWrite(FrontRightDirection, LOW);

//   analogWrite(RearLeftPower, 150);
//   analogWrite(RearRightPower, 150);
//   analogWrite(FrontLeftPower, 150);
//   analogWrite(FrontRightPower, 150);
// }

// void turnLeft()
// {
//   digitalWrite(RearLeftDirection, HIGH);
//   digitalWrite(RearRightDirection, LOW);
//   digitalWrite(FrontLeftDirection, LOW);
//   digitalWrite(FrontRightDirection, HIGH);

//   analogWrite(RearLeftPower, 200);
//   analogWrite(RearRightPower, 200);
//   analogWrite(FrontLeftPower, 200);
//   analogWrite(FrontRightPower, 200);
// }

void turn(int dir)
{
  // int inv  = !dir;

  digitalWrite(RearLeftDirection, !dir);
  digitalWrite(RearRightDirection, dir);
  digitalWrite(FrontLeftDirection, dir);
  digitalWrite(FrontRightDirection, !dir);

  analogWrite(RearLeftPower, 150);
  analogWrite(RearRightPower, 150);
  analogWrite(FrontLeftPower, 150);
  analogWrite(FrontRightPower, 150);
}
