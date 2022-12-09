#define FrontSensor 13

void setup() {
  Serial.begin(9600);

  //Settup Sensor Pins
  pinMode(FrontSensor, INPUT);

}

void loop() {
  Serial.print("Sensor Reading: ");
  Serial.println(digitalRead(FrontSensor));
}
