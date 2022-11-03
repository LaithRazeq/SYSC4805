//Interrupt Flag and Value
volatile uint32_t CaptureCountA;
volatile boolean CaptureFlag;

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

void setup() {

//Setting up interrupt timer for front ultrasonic 
  PMC->PMC_PCER0 |= PMC_PCER0_PID27;//TC0 power ON - Timer Counter 0 channel 0
  PIOB->PIO_PDR |= PIO_PDR_P25; // The pin is no more driven by GPIO
  PIOB->PIO_ABSR |= PIO_PB25B_TIOA0;// B Assign B25 to alternative periph_B (TIOA0):
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 //MCK/2 = 42 MHz,
  | TC_CMR_WAVE //Waveform mode
  | TC_CMR_WAVSEL_UP_RC //Count UP mode till RC
  | TC_CMR_ACPA_CLEAR //Clear TIOA0 on RA compare match
  | TC_CMR_ACPC_SET; // Set TIOA0 on RC compare match
  TC0->TC_CHANNEL[0].TC_RC = 2520000-1; //Set the frequency to 66.667Hz (Period 60 ms)
  TC0->TC_CHANNEL[0].TC_RA = 420-1; //Set the duty cycle (Pulse of 10 usec)
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG //Software trigger TC0 channel 0 counter
  | TC_CCR_CLKEN;//and enable
  //-----------Setting Registers for the Echo Signal---------------------
  PMC->PMC_PCER0 |= PMC_PCER0_PID28; // Timer Counter 0 channel 1 IS TC1, TC1 power ON
  TC0->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 // capture mode, MCK/2 = 42 MHz
  | TC_CMR_ABETRG // TIOA is used as the external trigger
  | TC_CMR_LDRA_FALLING// load RA on falling edge of TIOA
  | TC_CMR_ETRGEDG_RISING; // Trigger on rising edge
  TC0->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Reset TC counter and enable
  TC0->TC_CHANNEL[1].TC_IER |= TC_IER_LDRAS; // Trigger interrupt on Load RA
  NVIC_EnableIRQ(TC1_IRQn); // Enable TC1 interrupts



//begin monitor  
 Serial.begin(9600);

//settup motor pins
 pinMode(FrontRightDirection, OUTPUT);
 pinMode(FrontLeftDirection, OUTPUT);
 pinMode(RearRightDirection, OUTPUT);
 pinMode(RearRightPower, OUTPUT);
 pinMode(FrontLeftDirection, OUTPUT);
 pinMode(FrontLeftPower, OUTPUT);
 pinMode(FrontRightDirection, OUTPUT);
 pinMode(FrontRightPower, OUTPUT);
}
void loop() {
  
  if(CaptureFlag)
  { 
    // stop();   
    CaptureFlag = 0;
  }
}

void driveFwd()
{
  digitalWrite(RearLeftDirection, LOW);
  digitalWrite(RearRightDirection, LOW);
  digitalWrite(FrontLeftDirection, HIGH);
  // digitalWrite(FrontRightDirection, HIGH);

  analogWrite(RearLeftPower, 200);
  analogWrite(RearRightPower, 200);
  analogWrite(FrontLeftPower, 200);
  // analogWrite(FrontRightPower, 200);
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

void TC1_Handler() {
  uint32_t status = TC0->TC_CHANNEL[1].TC_SR; //Read status register, Clear status
  if (status & TC_SR_LDRAS) { // If ISR is fired by LDRAS then ....
    CaptureCountA = TC0->TC_CHANNEL[1].TC_RA; //read TC_RA
    if((340.0*CaptureCountA/(42000000.0)/2*100) < 5)
    {
      CaptureFlag = 1; //Inform the main loop of an update.
      Serial.println("interrupt!");
    }
  }
}
