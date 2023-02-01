#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display Oled
#define SDA_PIN 4
#define SCL_PIN 15
#define RESET_PIN 16
#define DISPLAY_X_MAX 128
#define DISPLAY_Y_MAX 64

// Botoes
#define BOTAO_1 19
#define BOTAO_2 22
#define BOTAO_3 21

// Led embutido
#define LED 25

// DHT11
#define DHT11_DATA_PIN 23

// Buzzer sem oscilador interno
#define BUZZER_PIN 18

Adafruit_SSD1306 display(DISPLAY_X_MAX, DISPLAY_Y_MAX, &Wire, RESET_PIN);

void setup()
{
  Serial.begin(115200);

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

void loop()
{
}