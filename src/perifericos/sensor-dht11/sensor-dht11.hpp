#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define DATA_PIN 23

typedef struct
{
  float temperatura;
  float umidade;
  bool erro;
} DadosDht11;

namespace sensorDht11
{
  // Inicializa DHT11 e inicia task de leitura
  void inicializa();

  // Realiza a atualização dos dados e retorna
  DadosDht11 getDados();
}