#include "./display.hpp"

namespace display
{
  namespace
  {
    void renderizaBarraSuperior(DadosBarraSuperior dados);
    void renderizaBarraInferior(char *escrita);

    Adafruit_SSD1306 display(X_MAX, Y_MAX, &Wire, RESET_PIN);

    TaskHandle_t displayTaskHandler;

    QueueHandle_t barraSuperiorDadosQueueHandler;
    QueueHandle_t barraInferiorDadosQueueHandler;

    void taskDisplay(void *pvParameters)
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.printf("Hello world !");
      display.display();

      DadosBarraSuperior dadosBarraSuperior = {0, 0};

      char *dadosBarraInferior = NULL;

      while (1)
      {
        xQueueReceive(barraSuperiorDadosQueueHandler, (void *)&dadosBarraSuperior, 0);
        xQueueReceive(barraInferiorDadosQueueHandler, (void *)&dadosBarraInferior, 0);

        display.clearDisplay();

        renderizaBarraSuperior(dadosBarraSuperior);
        
        if (dadosBarraInferior != NULL)
          renderizaBarraInferior(dadosBarraInferior);

        display.display();
        
        vTaskDelay(1);
      }
    }

    void renderizaBarraSuperior(DadosBarraSuperior dados)
    {
      display.setCursor(0, 0);

      display.printf("  %s%.1f C    ", dados.temperatura < 10 ? " " : "", dados.temperatura);

      display.printf("%s%.1f %%",
                     dados.umidade < 100 ? " " : dados.umidade < 10 ? "  "
                                                                    : "",
                     dados.umidade);

      display.drawLine(0, 8, X_MAX, 8, WHITE);
    }

    void renderizaBarraInferior(char *escrita)
    {
      display.setCursor(0, 64 - 7);
      display.printf("%s", escrita);
      display.drawLine(0, 64 - 9, X_MAX, 64 - 9, WHITE);
    }
  }

  void inicializa()
  {
    // Inicializando I2C para display
    Wire.begin(4, 15);

    // Inicializando display com tensão interna de 3.3V
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
      Serial.println(F("Falha ao iniciar display !"));
      for (;;)
        ;
    }

    barraSuperiorDadosQueueHandler = xQueueCreate(1, sizeof(DadosBarraSuperior));

    barraInferiorDadosQueueHandler = xQueueCreate(1, sizeof(char *));

    // Criar taks para ser processada no core 1
    xTaskCreatePinnedToCore(
        taskDisplay,
        "Display",
        10000,
        NULL,
        1,
        &displayTaskHandler,
        1);
  }

  void setDadosBarraSuperior(float temperatura, float umidade)
  {
    DadosBarraSuperior dados = {temperatura, umidade};
    xQueueSend(barraSuperiorDadosQueueHandler, (void *)&dados, 0);
  }

  void setDadosBarraInferior(char *escrita)
  {
    xQueueSend(barraInferiorDadosQueueHandler, (void *)&escrita, 0);
  }
}