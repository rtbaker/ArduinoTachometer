/*
 * Tachometer V1
 * 
 * Richard Baker <rich@mondaymorning.org> 30 April 2014
 *
 * Code for the LCD Display based on the Arduino tutorial:
 *   http://www.arduino.cc/en/Tutorial/LiquidCrystal
 *   
 *
 * Tachometer code based on:
 *   http://duino4projects.com/arduino-rpm-counter-tachometer-code/
 * 
 */

// include the library code:
#include <LiquidCrystal.h>

/* Initialise the library with the numbers of the interface pins.
 * 
 * Since we use pin 2 for the Hall effect sensor interrupt, these are different from the 
 * original tutorial.
 *
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 8
 * LCD D5 pin to digital pin 7
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 5
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 *
 */
LiquidCrystal lcd(12, 11, 8, 7, 6, 5);

// State
volatile byte rpmcount;
unsigned long rpm;
unsigned long timeold;

// Setup
int sensorPin = 2;


/*
 * Interrupt handler called once per revolution as the magent passes the sensor
 */
void rpm_tick(){
   rpmcount++;
}
 
void setup() {
  // Hall effect sensor triggers on FALLING (change from HIGH to LOW)
  attachInterrupt(0, rpm_tick, FALLING);
  
  // Sensor requires pull up resistor
  digitalWrite(sensorPin, HIGH);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Tachometer V1");
  
  rpmcount = 0;
  rpm = 0;
  timeold = 0;
  
  Serial.begin(9600);
  Serial.println("Setup complete !");
}

void loop() {
  //Update RPM every second or so
  delay(1000);
  
  //Don't process interrupts during calculations
  detachInterrupt(0);
  
  // Time between now and previous display update
  unsigned long now = millis();
  unsigned long diff = now - timeold;
  
  // Debugging messages
  Serial.print(now);
  Serial.print(" ");
  Serial.print(diff);
  Serial.print(" ");
  Serial.print(rpmcount);
  
  rpm = (60.0*1000.0/diff)*rpmcount;
  
  Serial.print(" = ");
  Serial.print(rpm);
  Serial.print("\n\r");
    
  // Display the result  
  lcd.setCursor(0, 1);
  lcd.print(rpm);
  lcd.print(" rpm ");
 // lcd.print(rpmcount);
  lcd.print ("          ");
  
  // Reset
  rpmcount = 0;
  timeold = millis();
  
  //Restart the interrupt processing
  attachInterrupt(0, rpm_tick, FALLING);
  digitalWrite(sensorPin, HIGH);
}
