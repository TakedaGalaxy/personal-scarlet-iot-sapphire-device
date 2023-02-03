#include "./teclado.hpp"

namespace teclado
{
  namespace
  {
    TaskHandle_t tecladoTaskHandler;

    const int teclasPins[QUANTIDADE] = {TECLA_1_PIN, TECLA_2_PIN, TECLA_3_PIN};

    StatusTecla teclasStatus[QUANTIDADE] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

    void (*Callbacks[QUANTIDADE])(void) = {NULL, NULL, NULL};

    void taskTeclado(void *pvParameters)
    {
      for (int i = 0; i < QUANTIDADE; i++)
      {

        bool estado = false;
        int estouro = 0;

        // Filtro para evitar erro de leitura, no caso de flutuação do estado do pino
        while (!digitalRead(teclasPins[i]) && !estado)
        {
          estouro++;
          estado = estouro >= 50;
          delay(1);
        }

        if (estado)
        {
          teclasStatus[i].precionada = true;

          if (Callbacks[i] != NULL && !teclasStatus[i].callBackChamada)
          {
            teclasStatus[i].callBackChamada = true;
            Callbacks[i]();
          }
          continue;
        }

        teclasStatus[i] = {0, 0, 0, 0};
      }

      vTaskDelete(tecladoTaskHandler);
    }

  }

  void inicializa()
  {
    for (int i = 0; i < QUANTIDADE; i++)
      pinMode(teclasPins[i], INPUT_PULLUP);
  }

  void atualizaStatus()
  {
    if (tecladoTaskHandler == NULL || eTaskGetState(tecladoTaskHandler) == 4)
    {
      // Criar taks para ser processada no core 0
      xTaskCreatePinnedToCore(
          taskTeclado,
          "Teclado",
          10000,
          NULL,
          1,
          &tecladoTaskHandler,
          0);
    }
  }

  StatusTecla getTeclaSatus(Teclas tecla)
  {
    if (tecla >= QUANTIDADE)
      return {0, 0, 1};

    StatusTecla retorno = teclasStatus[tecla];

    teclasStatus[tecla].statusLido = teclasStatus[tecla].precionada;

    return retorno;
  }

  void setCallback(Teclas tecla, void (*p)(void))
  {
    Callbacks[tecla] = p;
  }
}