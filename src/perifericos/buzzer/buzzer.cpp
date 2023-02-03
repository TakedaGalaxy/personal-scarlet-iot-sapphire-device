#include "./buzzer.hpp"

namespace buzzer
{
  namespace
  {
    TaskHandle_t buzzerTask;

    int frequencia = 500;
    int quantidadeBips;
    int tempoBipMs;

    bool inicializado = false;

    void setQuantidadeBips(int valor) { quantidadeBips = valor; }

    void setTempoBip(int valor) { tempoBipMs = valor; }

    void taskBip(void *pvParameters)
    {
      ledcWrite(CANAL, 125);

      delay(tempoBipMs);

      ledcWrite(CANAL, 0);

      vTaskDelete(buzzerTask);
    }
  }

  void inicializa()
  {
    // Configurando pwm para buzzer
    ledcSetup(CANAL, frequencia, RESOLUCAO);
    ledcAttachPin(BUZZER_PIN, CANAL);
    inicializado = true;
  }

  void setFrequencia(int valor)
  {
    frequencia = valor;
    ledcSetup(CANAL, frequencia, RESOLUCAO);
  }

  void disparaBip(int tempoBipMs, int quantidadeBips)
  {
    if (buzzerTask == NULL || eTaskGetState(buzzerTask) == 4)
    {
      setQuantidadeBips(tempoBipMs);
      setTempoBip(tempoBipMs);
      //Criar taks para ser processada no core 0
      xTaskCreatePinnedToCore(
          taskBip,
          "buzzer",
          10000,
          NULL,
          1,
          &buzzerTask,
          0);
    }
  }
}