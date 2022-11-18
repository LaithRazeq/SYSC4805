//LineFollower Sensors
#define LeftLineFollower 32
#define MiddleLineFollower 33
#define RightLineFollower 34

void setup() {
//Begin Monitor  
  Serial.begin(9600);

  pinMode(LeftLineFollower, INPUT);
  pinMode(MiddleLineFollower, INPUT);
  pinMode(RightLineFollower, INPUT);
}

void loop() {
  lineFollowerTest();
}

void lineFollowerTest()
{
  bool left = digitalRead(LeftLineFollower);
  bool middle = digitalRead(MiddleLineFollower);
  bool right = digitalRead(RightLineFollower);

  Serial.print("Left Sensor: ");
  Serial.println(left);

  Serial.print("Middle Sensor: ");
  Serial.println(middle);

  Serial.print("Right Sensor: ");
  Serial.println(right);

  delay(500);
}
