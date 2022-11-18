//LineFollower Sensors
#define LeftLineFollower 35
#define MiddleLineFollower 36
#define RightLineFollower 37

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
  int left = digitalRead(LeftLineFollower);
  int middle = digitalRead(MiddleLineFollower);
  int right = digitalRead(RightLineFollower);

  Serial.print("Left Sensor: ");
  Serial.println(left);

  Serial.print("Middle Sensor: ");
  Serial.println(middle);

  Serial.print("Right Sensor: ");
  Serial.println(right);

  delay(500);
}
