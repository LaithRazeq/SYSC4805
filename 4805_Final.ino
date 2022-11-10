#include <EZDist.h>

//Rear Direction HIGH = Backwards // LOW = Forward
#define RearLeftDirection 6
#define RearLeftPower 11
#define RearRightDirection 5
#define RearRightPower 10
//Rear Direction HIGH = Forward // LOW = Backward
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
//Front Left
#define LFS1_L 26
#define LFS1_M 27
#define LFS1_R 28
//Front Right
#define LFS2_L 29
#define LFS2_M 30
#define LFS2_R 31
//Other Sensors
#define FrontSensor 32

//Using EZDist To Track Distance
EZDist UltraLeft(TrigPinLeft, EchoPinLeft);
EZDist UltraRight(TrigPinRight, EchoPinRight);

//Setting Up Distance Variables and Constants
int distanceLeft;
int distanceRight;
const int minDistance = 15;

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
  if((distanceLeft <= minDistance) || (distanceRight < minDistance)){
    if(LFS1M || LFS2M)
    {
      turn90Right();
      turn90Right();            
    }
    else if(distanceLeft > distanceRight)
    {
      veerLeft();      
    }
    else if(distanceLeft <= distanceRight)
    {
      veerRight();
    }
  }
  else{
    driveFwd();
  }  
}

void sensorRead () {
  //Read Left and Right UltraSonics
  distanceLeft = UltraLeft.cm();
  distanceRight = UltraRight.cm();

  //TODO: Front Sensor

  //Read LaneFollowers
  LFS1L = digitalRead(LFS1_L);
  LFS1M = digitalRead(LFS1_M);
  LFS1R = digitalRead(LFS1_R);
  LFS2L = digitalRead(LFS2_L);
  LFS2M = digitalRead(LFS2_M);
  LFS2R = digitalRead(LFS2_R);
}

void driveFwd()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 200);
  analogWrite(RearRightPower, 200);
  analogWrite(FrontLeftPower, 200);
  analogWrite(FrontRightPower, 200);
}

void veerRight()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 200);
  analogWrite(RearRightPower, 100);
  analogWrite(FrontLeftPower, 200);
  analogWrite(FrontRightPower, 100);
}
void veerLeft()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 100);
  analogWrite(RearRightPower, 200);
  analogWrite(FrontLeftPower, 100);
  analogWrite(FrontRightPower, 200);
}

void driveBwd()
{
  digitalWrite(RearLeftDirection, HIGH);
  digitalWrite(RearRightDirection, HIGH);
  digitalWrite(FrontLeftDirection, LOW);
  digitalWrite(FrontRightDirection, LOW);

  analogWrite(RearLeftPower, 200);
  analogWrite(RearRightPower, 200);
  analogWrite(FrontLeftPower, 200);
  analogWrite(FrontRightPower, 200);
}

void stop()
{
  analogWrite(RearLeftPower, 0);
  analogWrite(RearRightPower, 0);
  analogWrite(FrontLeftPower, 0);
  analogWrite(FrontRightPower, 0);
}

void turn90Right()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, HIGH);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, LOW);

  analogWrite(RearLeftPower, 200);
  analogWrite(RearRightPower, 200);
  analogWrite(FrontLeftPower, 200);
  analogWrite(FrontRightPower, 200);

  delay(1750);

  stop();
}

void turn90Left()
{
  digitalWrite(RearLeftDirection, HIGH);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, LOW);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 200);
  analogWrite(RearRightPower, 200);
  analogWrite(FrontLeftPower, 200);
  analogWrite(FrontRightPower, 200);

  delay(2000);

  stop();
}