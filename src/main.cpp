#include <Arduino.h>

void setup()
{
  Serial.begin(115200);

  Serial.printf("Ola, inicializando projeto!");

  pinMode(25, OUTPUT);
}

void loop()
{
  digitalWrite(25, HIGH);
  delay(500);
  digitalWrite(25, LOW);
  delay(500);
}