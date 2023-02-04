#include "./buzzer.hpp"

namespace buzzer
{
  namespace
  {

    QueueHandle_t comandoQueuHandler;

    TaskHandle_t buzzerTask;

    int frequenciaAtual = 500;

    void taskBip(void *pvParameters)
    {

      Comando comando;

      while (1)
      {
        if (xQueueReceive(comandoQueuHandler, (void *)&comando, 0) == pdTRUE)
        {

          if (comando.frequencia != frequenciaAtual)
          {
            frequenciaAtual = comando.frequencia;
            ledcSetup(CANAL, frequenciaAtual, RESOLUCAO);
          }

          ledcWrite(CANAL, 125);

          vTaskDelay(comando.duracao >= 1 ? comando.duracao : 1);

          ledcWrite(CANAL, 0);

          continue;
        }
      }
    }
  }

  void inicializa()
  {
    // Configurando pwm para buzzer
    ledcSetup(CANAL, frequenciaAtual, RESOLUCAO);
    ledcAttachPin(BUZZER_PIN, CANAL);

    // Canal para comunicação entre uma task e a taskBuzzer
    comandoQueuHandler = xQueueCreate(1, sizeof(Comando));

    xTaskCreatePinnedToCore(
        taskBip,
        "Buzzer",
        10000,
        NULL,
        1,
        &buzzerTask,
        1);
  }

  bool emiteSom(Comando comando)
  {
    return xQueueSend(comandoQueuHandler, (void *)&comando, 0) == pdTRUE;
  }
}