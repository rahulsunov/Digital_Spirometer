#include "MAX30100_PulseOximeter.h"
 
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "OakOLED.h"
#define REPORTING_PERIOD_MS 1000
OakOLED oled;
 
PulseOximeter pox;
 
uint32_t tsLastReport = 0;


const int RunningAverageCount = 16;
float RunningAverageBuffer[RunningAverageCount];
int NextRunningAverage;
int i=0;
float val = 0.0;
float strength=0.0;
float bpm=0.0;
int spo2=0;
 
void onBeatDetected()
{
Serial.println("Beat!");
}
 
void setup()
{
Serial.begin(9600);
pinMode(A1,INPUT);
oled.begin();
oled.clearDisplay();
oled.setTextSize(1);
oled.setTextColor(1);
oled.setCursor(0, 0);
 
oled.println("Initializing pulse oximeter..");
Serial.println("Initializing pulse oximeter..");
oled.display();
 
if (!pox.begin()) {
oled.clearDisplay();
oled.setTextSize(1);
oled.setTextColor(1);
oled.setCursor(0, 0);
oled.println("FAILED");
Serial.println("FAILED");
oled.display();

} else {
oled.clearDisplay();
oled.setTextSize(1);
oled.setTextColor(1);
oled.setCursor(0, 0);
oled.println("SUCCESS");
oled.display();
Serial.println("SUCCESS");
}
pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
  val= analogRead(A1);
 
  RunningAverageBuffer[NextRunningAverage++] = val;
  if (NextRunningAverage >= RunningAverageCount)
  {
    NextRunningAverage = 0; 
  }
  float RunningAverageVal = 0;
  for(int i=0; i< RunningAverageCount; ++i)
  {
    RunningAverageVal += RunningAverageBuffer[i];
  }
  RunningAverageVal /= RunningAverageCount;
  strength = RunningAverageVal;

  delay(10);
  
  pox.update();
 
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
  {
      bpm = pox.getHeartRate();
      spo2 = pox.getSpO2();
      
      tsLastReport = millis();
  }
  
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(1);
  oled.setCursor(0,0);

  oled.print("Strength : ");
  oled.println(strength);
  oled.println("");
  oled.print("Heart: ");
  oled.print(bpm);
  oled.println(" BPM");
  oled.println("");
  oled.print("Spo2: ");
  oled.print(spo2);
  oled.println(" %");
  oled.display();
  bluetooth();
  strength=0.0;
  delay(1000);
}


void bluetooth() { //loop from the bluetooth code is renamed to "bluetooth" void
  Serial.print("Strength : ");
  Serial.println(strength);
  Serial.println("");
  Serial.print("Heart: ");
  Serial.print(bpm);
  Serial.println(" BPM");
  Serial.println("");
  Serial.print("Spo2: ");
  Serial.print(spo2);
  Serial.println(" %");
  //delay(1000);
  Serial.print("Failed to detect any components");
}
