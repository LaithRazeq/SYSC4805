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
}

void loop() {
  motorTest();
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
  analogWrite(FrontRightPower, 0);
  stop();
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

void turnRight()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, HIGH);
  digitalWrite(FrontLeftDirection, HIGH);
  digitalWrite(FrontRightDirection, LOW);

  analogWrite(RearLeftPower, 150);
  analogWrite(RearRightPower, 150);
  analogWrite(FrontLeftPower, 150);
  analogWrite(FrontRightPower, 150);
}

void turnLeft()
{
  digitalWrite(RearLeftDirection, HIGH);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, LOW);
  digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 200);
  analogWrite(RearRightPower, 200);
  analogWrite(FrontLeftPower, 200);
  analogWrite(FrontRightPower, 200);
}

void turn(int dir)
{
  digitalWrite(RearLeftDirection, !dir);
  digitalWrite(RearRightDirection, dir);
  digitalWrite(FrontLeftDirection, dir);
  digitalWrite(FrontRightDirection, !dir);

  analogWrite(RearLeftPower, 150);
  analogWrite(RearRightPower, 150);
  analogWrite(FrontLeftPower, 150);
  analogWrite(FrontRightPower, 150);
}