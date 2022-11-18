#include <EZDist.h>

#define TrigPinLeft 22
#define EchoPinLeft 23
#define TrigPinRight 24
#define EchoPinRight 25

EZDist UltraLeft(TrigPinLeft, EchoPinLeft);
EZDist UltraRight(TrigPinRight, EchoPinRight);

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorTest();
}

void sensorTest()
{
  distanceRight = UltraRight.cm();  
  distanceLeft = UltraLeft.cm();
  Serial.print("Left Sensor: ");
  Serial.println(distanceLeft);
  Serial.print("Right Sensor: ");
  Serial.println(distanceRight);
  delay(1000);
}