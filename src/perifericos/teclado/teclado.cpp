#include "./teclado.hpp"

namespace teclado
{
  namespace
  {

    QueueHandle_t teclasPrecionadasQueueHandler;
    QueueHandle_t teclaVisualizadaQueueHandler;
    QueueHandle_t setCallbackQueueHandler;

    StatusTecla statusTeclasUltimoRecebido[QUANTIDADE] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    TaskHandle_t tecladoTaskHandler;

    const int teclasPins[QUANTIDADE] = {TECLA_1_PIN, TECLA_2_PIN, TECLA_3_PIN};

    void taskTeclado(void *pvParameters)
    {

      StatusTecla statusTeclas[QUANTIDADE] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
      void (*callbacks[QUANTIDADE])(void) = {NULL, NULL, NULL};

      while (1)
      {

        int teclaVisualizada;

        if (xQueueReceive(teclaVisualizadaQueueHandler, (void *)&teclaVisualizada, 0) == pdTRUE)
          statusTeclas[teclaVisualizada].statusLido = true;

        SetCallback setRecebido;

        if (xQueueReceive(setCallbackQueueHandler, (void *)&setRecebido, 0) == pdTRUE)
          callbacks[setRecebido.tecla] = setRecebido.p;

        for (int i = 0; i < QUANTIDADE; i++)
        {

          bool estado = false;
          int estouro = 0;

          // Filtro para evitar erro de leitura, no caso de flutuação do estado do pino
          while (!digitalRead(teclasPins[i]) && !estado)
          {
            estouro++;
            estado = estouro >= 5;
            vTaskDelay(1);
          }
          if (estado)
          {

            statusTeclas[i].precionada = true;

            if (callbacks[i] != NULL && !statusTeclas[i].callBackChamada)
            {
              statusTeclas[i].callBackChamada = true;
              callbacks[i]();
            }

            continue;
          }

          statusTeclas[i] = {0, 0, 0};
        }

        xQueueSend(teclasPrecionadasQueueHandler, (void *)&statusTeclas, 0);
        vTaskDelay(1);
      }
    }

  }

  void inicializa()
  {

    for (int i = 0; i < QUANTIDADE; i++)
      pinMode(teclasPins[i], INPUT_PULLUP);

    teclasPrecionadasQueueHandler = xQueueCreate(1, sizeof(StatusTecla[QUANTIDADE]));

    teclaVisualizadaQueueHandler = xQueueCreate(1, sizeof(Teclas));

    setCallbackQueueHandler = xQueueCreate(5, sizeof(SetCallback));

    // Criar taks para ser processada no core 1
    xTaskCreatePinnedToCore(
        taskTeclado,
        "Teclado",
        10000,
        NULL,
        1,
        &tecladoTaskHandler,
        1);
  }

  StatusTecla getTecla(Teclas tecla)
  {

    xQueueReceive(teclasPrecionadasQueueHandler, (void *)&statusTeclasUltimoRecebido, 20);

    StatusTecla retorno = statusTeclasUltimoRecebido[tecla];

    if (!retorno.statusLido && retorno.precionada)
      xQueueSend(teclaVisualizadaQueueHandler, (void *)&tecla, 0);

    return retorno;
  }

  void setCallback(Teclas tecla, void (*callback)(void))
  {
    SetCallback set = {tecla, callback};
    xQueueSend(setCallbackQueueHandler, (void *)&set, 10);
  }
}