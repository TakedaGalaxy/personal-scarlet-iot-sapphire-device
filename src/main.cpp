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

  DadosDht11 resDht11 = sensorDht11::getDados();
  display::setDadosBarraSuerior(resDht11.temperatura, resDht11.umidade);

  StatusTecla resOK = teclado::getTecla(T_OK);
  if (!resOK.statusLido && resOK.precionada)
  {
    buzzer::emiteSom();
    Serial.println("Apenas uma vez !");
  }

  if (teclado::getTecla(T_PROXIMO).precionada)
  {
    Serial.printf("%.2f - %.2f\n", resDht11.temperatura, resDht11.umidade);
  }
}