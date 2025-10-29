#include <Arduino.h>
#include <Stepper.h>
#include <HX711_ADC.h>
#include <EEPROM.h>

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 30
#define MICROSTEPS 1
#define DIR 8
#define STEP 7
float calVal = 287784;


// 2-wire basic config, microstepping is hardwired on the driver
Stepper stepperRun(MOTOR_STEPS, DIR, STEP);


// Initialize the HX711 library
const int  DOUT = 11;
const int CLK = 12;
HX711_ADC loadCell(DOUT, CLK);


// Variable to store the measured load cell value
//int loadCellValue = 0;

// Pins for the buttons
const int buttonPin1 = 10;  // Change this to the pin connected to the first button
const int buttonPin2 = 9;  // Change this to the pin connected to the second button
const int buttonPin3 = 6;  // Change this to the pin connected to the second button
int counter = 1; // define a counter to keep track of the motors steps


void setup() {
    // Initialize serial communication
  Serial.begin(9600);

   // Set the speed of the stepper motor (in RPM)
  stepperRun.setSpeed(RPM);
  analogReadResolution(14);

  // Initialize the HX711 library
  loadCell.begin();
  loadCell.setCalFactor(calVal);
    loadCell.start(2000, 1);

  // Set button pins as inputs
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  analogReadResolution(14);
}


void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');

    if (input == "test1") {
      test1Program();
    } else if (input == "test2") {
      test2Program();
    } else if (input == "test3") {
      test3Program();
    } else {
      Serial.println("Invalid input. Please enter 'test1', 'test2', or 'test3'.");
    }
  }
}

void test1Program() {
  Serial.println("Running test1 program, Manual movement");
  
  while (true) {
    // Read button states
    int buttonState1 = digitalRead(buttonPin1);
    int buttonState2 = digitalRead(buttonPin2);
    int buttonState3 = digitalRead(buttonPin3);

loadCell.update();
float loadCellValue = loadCell.getData();
// Measure analog displacement value
int displacement = analogRead(A0);
float motor_actuation = (counter * 0.01 / 27);         // Gives the motor position according to the stepps sent to the motor. 27 is the gear ratio
float real_displacement = displacement * 12.8 / 16384;  // Converts the digital signal back too mm units (change 1024 to 4096 if 12 bit is used)


    
    // Check button states and move the motor accordingly
    if (buttonState1 == HIGH ) {
            digitalWrite(DIR,HIGH);
digitalWrite(STEP, HIGH);
delayMicroseconds(200);
digitalWrite(STEP, LOW);
delayMicroseconds(200);
digitalWrite(STEP, HIGH);
delayMicroseconds(200);
digitalWrite(STEP, LOW);
delayMicroseconds(200);
Serial.print(loadCellValue,4);
Serial.print(",");
Serial.println(real_displacement,4);

    } else if (buttonState2 == HIGH) {
      //stepper.rotate(-360);  // Move counter-clockwise
      digitalWrite(DIR,LOW);
digitalWrite(STEP, HIGH);
delayMicroseconds(200);
digitalWrite(STEP, LOW);
delayMicroseconds(200);
digitalWrite(STEP, HIGH);
delayMicroseconds(200);
digitalWrite(STEP, LOW);
delayMicroseconds(200);
Serial.print(loadCellValue,4);
Serial.print(",");
Serial.println(real_displacement,4);
    } else if (buttonState3 == HIGH) {
      Serial.println("Manual Movement Terminated, Chose Program to run");
      break;
    } else {

    }
    
    // Delay for stability (adjust as necessary)
    //delay(10);
  }
}


void test2Program() {


  // This programs actuates the machine until the load cell is above a threshhold value
  Serial.println("Started Test 2: automatic pre-tensioning of sample");
  Serial.println("Force:, Displacement:");

  // Threshold value for load cell to stop motor
 float loadThreshold = 0.02; // Adjust this value according to your setuptes
  while (true) {
    // Read load cell value
    loadCell.update();
    float loadCellValue = loadCell.getData();
    // Measure analog displacement value
    int displacement = analogRead(A0);
    // Measure emergency termination button
    int buttonState3 = digitalRead(buttonPin3);
    int goback = 1;
    
    for (int i = 0; i < 10; i++){
    // Check if load cell value is below the threshold
     if (buttonState3 == HIGH) {
      int goback = 0;
      return;
     } else if (loadCellValue > loadThreshold) {
      int goback = 0;
      return;
     }else if (displacement >9500000) {
      Serial.println("Platform moved beyond safe limit, Program aborted");
      int goback = 0;
      return;
      } else  {
      digitalWrite(DIR,LOW);
      digitalWrite(STEP, HIGH);
      delayMicroseconds(1400);
      digitalWrite(STEP, LOW);
      delayMicroseconds(1400);
      digitalWrite(STEP, HIGH);
      delayMicroseconds(1400);
      digitalWrite(STEP, LOW);
      delayMicroseconds(1400);
    } 
    }
   
    // Print motor position and load cell value
    Serial.print(loadCellValue);
    Serial.print(",");
    Serial.println((int)displacement);
    if (goback == 0){
      Serial.println("Pretension mesured, program finished");
      return;
    }
  }
}


void test3Program() {
  analogReadResolution(14);
  int counter = 1;
  Serial.println("Force:, Displacement:, Motor Actuation:");
// This program changes the calibration value of the load cell.
// It functions by applying two knowen loads to the load cell.
while(true){
 // Read load cell value
 loadCell.update();
 float loadCellValue = loadCell.getData();
  // Measure analog displacement value
 int displacement = analogRead(A0);
 // Measure emergency termination button
 int buttonState3 = digitalRead(buttonPin3);


if (displacement < 30 ){
return;
} else if (displacement > 150000000) {
return;
} else if (buttonState3 == LOW ){
 digitalWrite(DIR,LOW);
digitalWrite(STEP, HIGH);
delayMicroseconds(500);
digitalWrite(STEP, LOW);
delayMicroseconds(500);
 //counter = counter + 1;
 ++counter;
 float motor_actuation = (counter*0.01/27); // Gives the motor position according to the stepps sent to the motor. 27 is the gear ratio 
 float real_displacement = displacement*12.8/16383; // Converts the digital signal back too mm units (change 1024 to 4096 if 12 bit is used)

Serial.print(loadCellValue,4);
Serial.print(",");
Serial.print(real_displacement,4);
Serial.print(",");
Serial.println(motor_actuation);

} else {
  Serial.println("Test 3 Terminated, Tensile test Complete");
  return;
}
}
}
