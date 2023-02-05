#include "./wifi.hpp"

#define SSID ""
#define PASSWD ""

namespace wifi
{
  namespace
  {
    WiFiClient client;

    TaskHandle_t wifiTaskHandler;

    QueueHandle_t comandosQueueHandler;
    QueueHandle_t dadosQueueHandler;

    void taskWifi(void *pvParameters)
    {

      ComandoPeriferico comando = {P_BUZZER, 100};

      DadoPeriferico dado;

      while (1)
      {

        if (xQueueReceive(dadosQueueHandler, (void *)&dado, 0) == pdTRUE)
        {
          Serial.printf("%s", dado.nome);
          xQueueSend(comandosQueueHandler, (void *)&comando, 0);
        }

        vTaskDelay(1);
      }
    }

  }

  void inicizaliza()
  {

    comandosQueueHandler = xQueueCreate(5, sizeof(ComandoPeriferico));

    dadosQueueHandler = xQueueCreate(5, sizeof(DadoPeriferico));

    // Criar taks para ser processada no core 1
    xTaskCreatePinnedToCore(
        taskWifi,
        "Wifi",
        10000,
        NULL,
        1,
        &wifiTaskHandler,
        1);
  }

  void conectar()
  {
    if (WiFi.status() != WL_CONNECTED)
      WiFi.begin(SSID, PASSWD);
  }

  IPAddress getIp()
  {
    return WiFi.localIP();
  }

  const char *getStatus()
  {
    switch (WiFi.status())
    {
    case WL_CONNECTED:
      return "CON : ONLINE";

    case WL_CONNECTION_LOST:
    case WL_DISCONNECTED:
    case WL_NO_SHIELD:
      return "CON : OFFLINE";

    case WL_IDLE_STATUS:
      return "CON : CONECTANDO";

    default:
      return "CON : ERRO";
    }
  }

  ComandoPeriferico getCommando()
  {

    ComandoPeriferico retorno = {P_NADA, 0};

    xQueueReceive(comandosQueueHandler, (void *)&retorno, 0);

    return retorno;
  }

  void enviarDado(DadoPeriferico dado)
  {
    xQueueSend(dadosQueueHandler, (void *)&dado, 0);
  };

}