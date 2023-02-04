#include "./display.hpp"

namespace display
{
  namespace
  {
    void renderizaBarraSuperior(DadosBarraSuperior dados);

    Adafruit_SSD1306 display(X_MAX, Y_MAX, &Wire, RESET_PIN);

    TaskHandle_t displayTaskHandler;
    QueueHandle_t barraSuperiorDadosQueueHandler;

    void taskDisplay(void *pvParameters)
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.printf("Hello world !");
      display.display();

      DadosBarraSuperior dadosBarraSuperior = {0, 0};

      while (1)
      {
        xQueueReceive(barraSuperiorDadosQueueHandler, (void *)&dadosBarraSuperior, 0);

        display.clearDisplay();

        renderizaBarraSuperior(dadosBarraSuperior);

        display.display();
      }
    }

    void renderizaBarraSuperior(DadosBarraSuperior dados)
    {
      display.setCursor(0, 0);
      
      display.printf("  %s%.1f C    ", dados.temperatura < 10 ? " " : "", dados.temperatura);
      
      display.printf("%s%.1f %%",
                     dados.umidade < 100 ? " " : 
                     dados.umidade < 10 ? "  " : "",
                     dados.umidade);

      display.drawLine(0, 8, X_MAX, 8, WHITE);
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

  void setDadosBarraSuerior(float temperatura, float umidade)
  {
    DadosBarraSuperior dados = {temperatura, umidade};
    xQueueSend(barraSuperiorDadosQueueHandler, (void *)&dados, 0);
  }
}