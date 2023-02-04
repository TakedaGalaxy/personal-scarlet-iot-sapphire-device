#include "./sensor-dht11.hpp"

namespace sensorDht11
{
  namespace
  {

    TaskHandle_t dht11TaskHandle;

    DHT_Unified dht(DATA_PIN, DHT11);

    float umidade = 0;
    float temperatura = 0;

    void taskDht11(void *pvParameters)
    {
      sensors_event_t event;

      dht.temperature().getEvent(&event);
      if (!isnan(event.temperature))
        temperatura = event.temperature;

      dht.humidity().getEvent(&event);
      if (!isnan(event.relative_humidity))
        umidade = event.relative_humidity;

      // Necessário delay entre as leituras, especificaod pelo datasheet
      vTaskDelay(250);

      vTaskDelete(dht11TaskHandle);
    }

  }

  void inicializa()
  {
    // Inicializando sensor de temperatura e umidade
    dht.begin();
  }

  void atualizaDados()
  {
    if (dht11TaskHandle == NULL || eTaskGetState(dht11TaskHandle) == 4)
    {
      // Criar taks para ser processada no core 0
      xTaskCreatePinnedToCore(
          taskDht11,
          "DHT11",
          10000,
          NULL,
          1,
          &dht11TaskHandle,
          0);
    }
  }

  float getTemperatura() { return temperatura; }

  float getUmidade() { return umidade; }

}