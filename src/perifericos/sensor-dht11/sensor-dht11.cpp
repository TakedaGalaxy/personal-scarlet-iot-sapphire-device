#include "./sensor-dht11.hpp"

namespace sensorDht11
{
  namespace
  {

    QueueHandle_t dadosQueueHandler;

    TaskHandle_t dht11TaskHandle;

    DadosDht11 ultimoDadoRecebido = {0, 0, 0};

    DHT_Unified dht(DATA_PIN, DHT11);

    void taskDht11(void *pvParameters)
    {
      while (1)
      {

        DadosDht11 novoDado = {0, 0, 0};

        sensors_event_t event;

        dht.temperature().getEvent(&event);
        if (!isnan(event.temperature))
          novoDado.temperatura = event.temperature;
        else
          novoDado.erro = true;

        dht.humidity().getEvent(&event);
        if (!isnan(event.relative_humidity))
          novoDado.umidade = event.relative_humidity;
        else
          novoDado.erro = true;

        if (xQueueSend(dadosQueueHandler, (void *)&novoDado, 0) != pdTRUE)
        {
          // Caso erro
        }

        // Necessário delay entre as leituras, especificaod pelo datasheet
        vTaskDelay(250);
      }
    }

  }

  void inicializa()
  {
    // Inicializando sensor de temperatura e umidade
    dht.begin();

    dadosQueueHandler = xQueueCreate(1, sizeof(DadosDht11));

    // Criar taks para ser processada no core 1
    xTaskCreatePinnedToCore(
        taskDht11,
        "DHT11",
        10000,
        NULL,
        1,
        &dht11TaskHandle,
        1);
  }

  DadosDht11 getDados()
  {
    xQueueReceive(dadosQueueHandler, (void *)&ultimoDadoRecebido, 0);
    return ultimoDadoRecebido;
  }
}