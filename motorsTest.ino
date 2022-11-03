#include <EZDist.h>

int trigPin = 12;
int echoPin = 13;

//Rear Direction HIGH = Backwards // LOW = Forward
#define RearLeftDirection 5
#define RearLeftPower 11
#define RearRightDirection 4
#define RearRightPower 10
//Rear Direction HIGH = Forward // LOW = Backward
#define FrontLeftDirection 3
#define FrontLeftPower 9
#define FrontRightDirection 2
#define FrontRightPower 8



EZDist EZDist(trigPin, echoPin);

void setup() {

//begin monitor  
 Serial.begin(9600);

//settup motor pins
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);
 pinMode(5, OUTPUT);
 pinMode(8, OUTPUT);
 pinMode(9, OUTPUT);
 pinMode(10, OUTPUT);
 pinMode(11, OUTPUT);
}
void loop() {
//  driveFwd();
//  delay(5000);
//  stop();
 delay(3000);
 turn90Left();
//  turn90Left();
 delay(5000);
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
