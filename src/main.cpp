#include "./main.hpp"

void setup()
{
  Serial.begin(115200);

  display::inicializa();

  buzzer::inicializa();

}

void loop()
{

  buzzer::disparaBip();

  delay(500);

}