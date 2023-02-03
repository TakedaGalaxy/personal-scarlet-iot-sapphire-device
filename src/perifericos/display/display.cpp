#include "./display.hpp"

namespace display
{
  namespace
  {
    Adafruit_SSD1306 display(X_MAX, Y_MAX, &Wire, RESET_PIN);
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

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.printf("Hello world !");
    display.display();
  }

  void logica()
  {
  }
}