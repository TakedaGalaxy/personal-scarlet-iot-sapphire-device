#include "./main.hpp"

void testCallback()
{
  DadoPeriferico dado= {P_NADA,"opa",0,"test"};

  wifi::enviarDado(dado);
}

void setup()
{
  Serial.begin(115200);

  wifi::inicizaliza();

  display::inicializa();

  sensorDht11::inicializa();

  buzzer::inicializa();

  teclado::inicializa();
  teclado::setCallback(T_ANTERIOR, testCallback);
}

void loop()
{

  DadosDht11 resDht11 = sensorDht11::getDados();
  display::setDadosBarraSuperior(resDht11.temperatura, resDht11.umidade);

  char statusWifi[20];
  strcpy(statusWifi, wifi::getStatus());
  display::setDadosBarraInferior(statusWifi);

  StatusTecla resOK = teclado::getTecla(T_OK);
  if (!resOK.statusLido && resOK.precionada)
  {
    buzzer::emiteSom();
    Serial.println("Apenas uma vez !");
    wifi::conectar();
  }

  if (teclado::getTecla(T_PROXIMO).precionada)
  {
    Serial.printf("%.2f - %.2f\n", resDht11.temperatura, resDht11.umidade);
    Serial.printf("%s\n", wifi::getIp().toString().c_str());
  }

  ComandoPeriferico resWifi = wifi::getCommando();
  if (resWifi.idPeriferico != P_NADA)
    Serial.printf("BUZZER %d %f\n", resWifi.idPeriferico, resWifi.dado);
}