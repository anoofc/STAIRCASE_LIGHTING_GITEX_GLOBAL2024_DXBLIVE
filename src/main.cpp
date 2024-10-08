/**
 * Written by : 	Anoof Chappangathil
 * Email      :   anoofdoc@gmail.com
 * Phone      :   +971 50 762 1347
 * Company    :   Interactive Technical Service LLC
 * Date       :   08-10-2024
 * Description:   This code is written for a 16 step Staircase lighting system. 
 *                The system has 16 steps and each step has a sensor. 
 *                When a sensor is triggered, the corresponding step will light up for 5 seconds and then turn off.
 */

#define DEBUG           1

#define SENSOR_PIN_1    31
#define SENSOR_PIN_2    32
#define SENSOR_PIN_3    33 
#define SENSOR_PIN_4    34
#define SENSOR_PIN_5    35
#define SENSOR_PIN_6    36
#define SENSOR_PIN_7    37
#define SENSOR_PIN_8    38
#define SENSOR_PIN_9    39
#define SENSOR_PIN_10   40
#define SENSOR_PIN_11   41
#define SENSOR_PIN_12   42
#define SENSOR_PIN_13   43
#define SENSOR_PIN_14   44
#define SENSOR_PIN_15   45
#define SENSOR_PIN_16   46

#define LED_PIN         8

#define DEBOUNCE_DELAY    500
#define STRIP_CLEAR_DELAY 5000
#define NUM_OF_STEPS      16
#define STEP_LENGTH       28


#define RED             255, 0, 0
#define WHITE           255, 255, 255

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>  

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_OF_STEPS*STEP_LENGTH, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t lastmillisUpdate = 0;

uint8_t   sensorPins[NUM_OF_STEPS]        = {SENSOR_PIN_1, SENSOR_PIN_2, SENSOR_PIN_3, SENSOR_PIN_4, SENSOR_PIN_5, SENSOR_PIN_6, SENSOR_PIN_7, SENSOR_PIN_8, SENSOR_PIN_9, SENSOR_PIN_10, SENSOR_PIN_11, SENSOR_PIN_12, SENSOR_PIN_13, SENSOR_PIN_14, SENSOR_PIN_15, SENSOR_PIN_16};
uint32_t  ledStripClearTime[NUM_OF_STEPS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool      ledStatus[NUM_OF_STEPS]         = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void io_setup() {
  strip.begin();
  strip.clear();
  strip.setBrightness(255);
  strip.show();

  for (int i = 0; i < 16; i++) {
    pinMode(sensorPins[i], INPUT_PULLUP);
  }
}

void debugInputPins(){
  for (int i =0; i < 16; i++) {
    Serial.print("S"); Serial.print(i+1); Serial.print(":"); Serial.print(String(digitalRead(sensorPins[i])) + " ");
  }
  Serial.println();
  delay(500);
}

void showStep(int step){
  for (int i = ((step*STEP_LENGTH)-STEP_LENGTH); i < step*STEP_LENGTH; i++) {
    strip.setPixelColor(i, WHITE);
  }
  strip.show();
  ledStripClearTime[step-1] = millis();
  if (DEBUG) {Serial.print("Showing Step: "); Serial.println(step);}
}

void clearStep(int step){
  for (int i = ((step*STEP_LENGTH)-STEP_LENGTH); i < step*STEP_LENGTH; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
  if (DEBUG) {Serial.print("Clearing Step: "); Serial.println(step);}
}

void allon () {
  for (int i = 0; i < NUM_OF_STEPS*STEP_LENGTH; i++) {
    strip.setPixelColor(i, WHITE);
  }
  strip.show();
}

void alloff () {
  for (int i = 0; i < NUM_OF_STEPS*STEP_LENGTH; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void checkClearStrip(){
  for (int i = 0; i < NUM_OF_STEPS; i++) {
    if (ledStatus[i] == 1){
      if (millis() - ledStripClearTime[i] > STRIP_CLEAR_DELAY) {
        clearStep(i+1);
        ledStatus[i] = 0;
      }
    }  
  }
}

void readSensors(){
  for (int i = 0; i < NUM_OF_STEPS; i++) {
    if (digitalRead(sensorPins[i]) == LOW) {
      if (millis() - lastmillisUpdate < DEBOUNCE_DELAY) { return; }
      lastmillisUpdate = millis();
      if (DEBUG) {Serial.print("Sensor "); Serial.print(i+1); Serial.println(" triggered");}
      showStep(i+1);
      ledStatus[i] = 1;
    }
  }
}

void setup() {
  Serial.begin(9600);
  io_setup();
}

void loop() {
  readSensors();
  checkClearStrip();
  // if (DEBUG) {debugInputPins();}
}
