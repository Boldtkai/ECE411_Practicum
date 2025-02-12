/*
Social Distancing Device
ECE411 Practicum Project
Team 6: Kai Boldt, Ryan Nand, Ranvir Sandhu, Elijah Penn

Functionality:
This program allows the Social Distancing Device to measure the distance that the
User is away from another person. If they fail to meet social distancing standards
of 6' then the device will alarm the user. The program will measure the person's 
distance from the user and their respective body heat. To do this the program 
will take 1,000 Samples of Distance and Temperature and determine the average from
the total number of samples taken. Then it'll store the average distance and temperature 
into variables for the test conditions to determine failure/success of social distancing 
standards.

The Test Conditions:
1)  3' < Distance < =6' and Temperature > 70.5 degrees Fahrenheit
2) Distance <= 3' and Temperature >70.5 degrees Fahrenheit

If Test Case 1 is true: LED Display only
If Test Case 2 is true: LED Display and Auditroy Alarm

This program will run an ednless loop till the user depowers the system with the slide
switch that's located on the PCB
*/


//Libraries to utilize the appropiate sensors
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "SparkFun_VL53L1X.h"
 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
SFEVL53L1X distanceSensor;

#define SPEAKERpin 9
#define LEDpin1 10                                           //LED pin attaches to Digital Output 10 of Arduino
#define LEDpin2 11                                           //LED pin attaches to Digital Outpu 11 of Arduino

void setup() {
  pinMode(LEDpin1, OUTPUT);                                  // Sets Digital Pin 10 as an Output
  pinMode(LEDpin2, OUTPUT);                                  // Sets Digital Pin 11 as an Output
  Serial.begin(9600);

  mlx.begin();                                               // Initialize MLX90614 Temperature Sensor
  Serial.println("Adafruit MLX90614 test");                  // Serial Print MLX90614 test

  Wire.begin();
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() != 0)                                                          //Begin returns 0 on a good initialization
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("Sensor online!");
}


void loop() {
  //Temperature Code MLX90614 BCC
  float AverageTemp = 0;                                                     //Average Temperature; to be used for summation within For loop
  int Samples = 1000;                                                        //Number of Samples (1000 Samples)
 
  for (int i=0; i < Samples; i++) {                                          //For Loop
  AverageTemp = AverageTemp + mlx.readObjectTempF();                         //For 1,000 samples takes a summation of the current Temperature Reading to the last total value from the last
  }
  AverageTemp = AverageTemp/Samples;                                         //Determine Temperature Average by taking the quotient of the total values and the number of samples taken
  Serial.print("Average Temperature = "); Serial.println(AverageTemp);
  delay(100);


  //Distance Code VL53L1X
  distanceSensor.startRanging();                            //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  int distance = distanceSensor.getDistance();              //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();                          //Clears the interrupt on the time of flight sensor
  distanceSensor.stopRanging();                             //Stops Measuring the Distance

  float distanceInches = distance * 0.0393701;              //Converts metric distance to inches 
  float distanceFeet = distanceInches / 12.0;               //Converts inches into feet

  float Total = 0;
  float AverageDist = 0;
 
  for (int i=0; i < Samples; i++) {
  Total = Total + distanceFeet;
  }
  AverageDist = Total/Samples;
  Serial.print("Average Distance = "); Serial.println(AverageDist);
  delay(100);

  //Checks Conditions of Distance <= 6' and >3' and Temperature > 70.5degrees Fahrenheit
  if (AverageDist <= 6.00 && AverageDist > 3.00 && AverageTemp > 70.5) 
  {
    Serial.print("Within Dangerous Range\n");
    
    for (int j=0; j<=3; j++) {
    digitalWrite(LEDpin1, HIGH);                                    // turn the LED on (HIGH is the voltage level)
    delay(300);                                                     // wait for 0.3 seconds
    digitalWrite(LEDpin1, LOW);                                     // turn the LED off by making the voltage LOW
    delay(100);                                                     // wait for 0.1 seconds
    digitalWrite(LEDpin2, HIGH);                                    // turn the LED on (HIGH is the voltage level)
    delay(300);                                                     // wait for 0.3 seconds
    digitalWrite(LEDpin2, LOW);                                     // turn the LED off by making the voltage LOW
    delay(100);                                                     // wait for 0.1 seconds
    }
  } 
  else 
  {
    digitalWrite(LEDpin1, LOW);                                     // turn the LED off by making the voltage LOW
    digitalWrite(LEDpin2, LOW);                                     // turn the LED off by making the voltage LOW
  }

  //Checks Conditions of Distance <= 3' and Temperature > 70.5 degress Fahrenheit
  if (AverageDist <= 3.00 && AverageTemp > 70.5) 
  {
    Serial.print("Within Dangerous Range\n");

    digitalWrite(LEDpin1, HIGH);                                    // turn the LED on (HIGH is the voltage level)
    delay(200);                                                     // wait for a second
    digitalWrite(LEDpin1, LOW);                                     // turn the LED off by making the voltage LOW
    delay(100);                                                     // wait for a second
    digitalWrite(LEDpin2, HIGH);                                    // turn the LED on (HIGH is the voltage level)
    delay(200);                                                     // wait for a second
    digitalWrite(LEDpin2, LOW);                                     // turn the LED off by making the voltage LOW
    delay(100);                                                     // wait for a second
    
  // Whoop up
  for(int hz = 700; hz < 1000; hz++){                               //For Loop to incrementally play 700Hz tp 1000Hz
    tone(SPEAKERpin, hz, 50);
    delay(5);
  }
  noTone(SPEAKERpin);                                               //Turns OFF Speaker 

  // Whoop down
  for(int hz = 1000; hz > 700; hz--){                               //For Loop to decrementally play 1000Hz tp 700Hz
    tone(SPEAKERpin, hz, 50);
    delay(5);
  }
  noTone(SPEAKERpin);

    digitalWrite(LEDpin1, HIGH);                                    // turn the LED on (HIGH is the voltage level)
    delay(200);                                                     // wait for a second
    digitalWrite(LEDpin1, LOW);                                     // turn the LED off by making the voltage LOW
    delay(100);                                                     // wait for a second
    digitalWrite(LEDpin2, HIGH);                                    // turn the LED on (HIGH is the voltage level)
    delay(200);                                                     // wait for a second
    digitalWrite(LEDpin2, LOW);                                     // turn the LED off by making the voltage LOW
    delay(100);                                                     // wait for a second
  } 
  else 
  {
    digitalWrite(LEDpin1, LOW);                                     // turn the LED off by making the voltage LOW
    digitalWrite(LEDpin2, LOW);                                     // turn the LED off by making the voltage LOW
  }
}
