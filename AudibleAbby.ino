#include <Wire.h>
#include "SparkFunBME280.h"
BME280 ThisSensor;
// To work with range instead of single value Real Value = 100 and Test Value = 10
int Tolerance = 100;//10;
// Establish baseline
int LocalAltitude = 0;
int AGL = 0;
int PrevAGL = 0;
// Up is 1 and Down is 0
int UpDown = 0;
// Range for Seat Belt Real Value = 1500ish and Test Value = 30ish
int SeatBelt = 15; //3;
// Range for Seat Belt Real Value = 5500ish and Test Value = 150ish
constexpr int Alarm1 = 55;//15;
// Range for Seat Belt Real Value = 3500ish and Test Value = 100ish
constexpr int Alarm2 = 35;//10;
// Range for Seat Belt Real Value = 2500ish and Test Value = 60ish
constexpr int Alarm3 = 25;//6;

void setup()
{
  // Output pins
  pinMode(3, OUTPUT);//BUZZER
  pinMode(13, OUTPUT);//Onboard LED
  digitalWrite(13, LOW);
  // Setup I2C
  Wire.begin();
  // This is important because the default address will not work
  ThisSensor.setI2CAddress(0x76);
  ThisSensor.beginI2C();
  ThisSensor.setFilter(4);
  ThisSensor.setTempOverSample(5);
  ThisSensor.setPressureOverSample(5);
  ThisSensor.setHumidityOverSample(0);
  for (int i = 1; i <= 3; i++) {
    digitalWrite(3, HIGH);
    delay(100);
    digitalWrite(3, LOW);
    delay(50);
  }
  delay(2000);
  // Need temp for calibration
  ThisSensor.readTempC();
  // Starting Altitude
  LocalAltitude = (int)ThisSensor.readFloatAltitudeFeet();
}

void loop()
{
  delay(100);
  // Need temp for calibration
  ThisSensor.readTempC();
  // How far from where we started?
  AGL = (int)ThisSensor.readFloatAltitudeFeet() - LocalAltitude;
  // If more than previous AGL we are going up (within limit because we are only intrested in the sign)
  if (AGL > PrevAGL && UpDown < Tolerance / 2) UpDown++;
  // If less than previous AGL we are going down (within limit because we are only intrested in the sign)
  if (AGL < PrevAGL && UpDown > Tolerance / -2) UpDown--;
  // If we are going up
  if (UpDown > 0) {
    // Turn LED on
    digitalWrite(13, HIGH);
    // Make sound at Seat Belt range
    if (AGL / Tolerance == SeatBelt) SeatBeltSound();
  }
  else {
    // We are going down, turn LED off
    digitalWrite(13, LOW);
    // This is why we declared Alarm* values as constexpr
    switch (AGL / Tolerance) {
      case Alarm1:
        AlarmSound1();
        break;
      case Alarm2:
        AlarmSound2();
        break;
      case Alarm3:
        AlarmSound3();
        break;
    }
  }
  PrevAGL = AGL;
}

void SeatBeltSound() {
  for (int i = 1; i <= 5; i++) {
    digitalWrite(3, HIGH);
    delay(150);
    digitalWrite(3, LOW);
    delay(100);
  }
  delay(500);
  digitalWrite(3, HIGH);
  delay(200);
  digitalWrite(3, LOW);
  delay(3000);
}

void AlarmSound1() {
  for (int i = 1; i <= 3; i++) {
    for (int j = 1; j <= 3; j++) {
      digitalWrite(3, HIGH);
      delay(200);
      digitalWrite(3, LOW);
      delay(100);
    }
    digitalWrite(3, HIGH);
    delay(400);
    digitalWrite(3, LOW);
    delay(100);
  }
}

void AlarmSound2()  {
  for (int i = 1; i <= 16; i++) {
    digitalWrite(3, HIGH);
    delay(100);
    digitalWrite(3, LOW);
    delay(100);
  }
}

void AlarmSound3() {
  for (int i = 1; i <= 10; i++) {
    for (int j = 1; j <= 5; j++) {
      digitalWrite(3, HIGH);
      delay(25);
      digitalWrite(3, LOW);
      delay(25);
    }
    for (int j = 1; j <= 4; j++) {
      digitalWrite(3, HIGH);
      delay(100);
      digitalWrite(3, LOW);
      delay(100);
    }
  }
}
