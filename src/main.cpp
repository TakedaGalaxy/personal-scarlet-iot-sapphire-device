#include "./main.hpp"

void setup()
{
  Serial.begin(115200);

  display::inicializa();
  sensorDht11::inicializa();
  buzzer::inicializa();

}

void loop()
{

  //buzzer::disparaBip();
  sensorDht11::atualizaDados();
  Serial.printf("%.2f - %.2f\n",sensorDht11::getTemperatura(), sensorDht11::getUmidade());
  //delay(500);

}