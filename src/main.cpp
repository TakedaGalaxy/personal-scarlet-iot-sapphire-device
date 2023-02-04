#include "./main.hpp"

void testCallback()
{
  Serial.println("Callback hehehe !");
}

void setup()
{
  Serial.begin(115200);
  sensorDht11::inicializa();

  // display::inicializa();
  //teclado::setCallback(T_ANTERIOR, testCallback);

  //buzzer::inicializa();
  //teclado::inicializa();

}

void loop()
{
  Serial.printf("%.2f - %.2f\n", sensorDht11::getDados().temperatura, sensorDht11::getDados().umidade);
  
  // if (!digitalRead(TECLA_1_PIN))
  // {
  //   buzzer::emiteSom();
  //   while (!digitalRead(TECLA_1_PIN))
  //     ;
  // }

  // teclado::atualizaStatus();

  // StatusTecla resOK = teclado::getTeclaSatus(T_OK);
  // if (resOK.precionada && !resOK.statusLido)
  // {
  //   buzzer::disparaBip();
  //   Serial.println("Apenas uma vez !");
  // }

  // if (teclado::getTeclaSatus(T_PROXIMO).precionada)
  //   Serial.printf("%.2f - %.2f\n", sensorDht11::getTemperatura(), sensorDht11::getUmidade());
}