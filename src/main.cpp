#include "./main.hpp"

void testCallback()
{
  Serial.println("Callback hehehe !");
}

void setup()
{
  Serial.begin(115200);

  display::inicializa();
  sensorDht11::inicializa();
  buzzer::inicializa();

  teclado::inicializa();
  teclado::setCallback(T_ANTERIOR, testCallback);
}

void loop()
{

  // buzzer::disparaBip();
  // delay(500);
  // sensorDht11::atualizaDados();
  // Serial.printf("%.2f - %.2f\n",sensorDht11::getTemperatura(), sensorDht11::getUmidade());

  teclado::atualizaStatus();

  StatusTecla resOK = teclado::getTeclaSatus(T_OK);
  if (resOK.precionada && !resOK.statusLido)
    Serial.println("Apenas uma vez !");

  if (teclado::getTeclaSatus(T_PROXIMO).precionada)
    Serial.println("Varias vezes lido");
}