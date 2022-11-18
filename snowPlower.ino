#include <EZDist.h>
#include <avr/wdt.h>

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

//Motor Constant Speed Variables
const int maxSpeed = 200;
const int normalSpeed = 150;
const int slowSpeed = 100;
const int turningSpeed = 150;

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

  //Setting up WATCHDOG timer
  delay(3000);
  wdt_enable(WDTO_8S);
}

void loop() {
  sensorRead(); 
  if((distanceLeft <= stopDistance) || (distanceRight <= stopDistance))
  {
    driveBwd();
    delay(300);
    if(turnDirection)
    {   
      turn(turnDirection);
    }
    else
    {
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
    // else if(distanceLeft <= minDistance)
    // {
    //   turn(turnDirection);
    // }
    // else if(distanceRight <= minDistance)
    // {
    //   turn(turnDirection);
    // }
    else
    {
      turn(turnDirection);
    }
  }
  else{
    driveFwd();
  }
  //Watchdog timer reset
  wdt_reset();
  //infinity loop to hang MCU
  while(1){}
}

void sensorRead () {

  //Ultra Sonic Sensors
  int tempLeft = UltraLeft.cm();
  int tempRight = UltraRight.cm();
  if((tempLeft > 0)) 
  {
    distanceLeft = tempLeft;
  }
  if((tempRight > 0))
  {
    distanceRight = tempRight;
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
  
  //Front Sensor
  frontSensorOnOff = digitalRead(FrontSensor);
  if((frontSensorFlag == 0) && (frontSensorOnOff == 1))
  {
    frontSensorFlag = 1;   
  }

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
  while((!frontSensorOnOff))
  {
    turn(turnDirection);
    sensorRead();
    //Watchdog timer reset
    wdt_reset();  
  }    
}

void driveFwd()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, normalSpeed);
  analogWrite(RearRightPower, normalSpeed);
  analogWrite(FrontLeftPower, normalSpeed);
  analogWrite(FrontRightPower, normalSpeed);
}

void driveBwd()
{
  digitalWrite(RearLeftDirection, HIGH);
  digitalWrite(RearRightDirection, HIGH);
  digitalWrite(FrontLeftDirection, LOW);
  digitalWrite(FrontRightDirection, LOW);

  analogWrite(RearLeftPower, slowSpeed);
  analogWrite(RearRightPower, slowSpeed);
  analogWrite(FrontLeftPower, slowSpeed);
  analogWrite(FrontRightPower, slowSpeed);
}

void stop()
{
  analogWrite(RearLeftPower, 0);
  analogWrite(RearRightPower, 0);
  analogWrite(FrontLeftPower, 0);
  analogWrite(FrontRightPower, 0);
}

void turn(int dir)
{
  //If dir is 1, the car will turn right
  //If dir is 0, the car will turn left
  digitalWrite(RearLeftDirection, !dir);
  digitalWrite(RearRightDirection, dir);
  digitalWrite(FrontLeftDirection, dir);
  digitalWrite(FrontRightDirection, !dir);

  analogWrite(RearLeftPower, turningSpeed);
  analogWrite(RearRightPower, turningSpeed);
  analogWrite(FrontLeftPower, turningSpeed);
  analogWrite(FrontRightPower, turningSpeed);
}
