#include <Arduino.h>

#include <SerialLogger.hpp>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Log::init(Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  Log::d() << "qweqwe" << Log::endl;
  delay(1000);
}