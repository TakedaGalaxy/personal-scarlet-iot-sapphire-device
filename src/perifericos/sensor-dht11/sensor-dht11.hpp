#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DATA_PIN 23

namespace sensorDht11
{
  void inicializa();

  // Cria uma task para fazer a leitura dos dados do DHT11
  void atualizaDados();

  float getTemperatura();
  float getUmidade();
}