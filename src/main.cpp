#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Display Oled
#define SDA_PIN 4
#define SCL_PIN 15
#define RESET_PIN 16
#define DISPLAY_X_MAX 128
#define DISPLAY_Y_MAX 64

// Botoes
#define BOTAO_1_PIN 19
#define BOTAO_2_PIN 22
#define BOTAO_3_PIN 21

#define BOTAO_1 !digitalRead(BOTAO_1_PIN)
#define BOTAO_2 !digitalRead(BOTAO_2_PIN)
#define BOTAO_3 !digitalRead(BOTAO_3_PIN)

// Led embutido
#define LED 25

// DHT11
#define DHT11_DATA_PIN 23

// Buzzer sem oscilador interno
#define BUZZER_PIN 18
#define FREQUENCIA 1000
#define CANAL 0
#define RESOLUCAO 8

Adafruit_SSD1306 display(DISPLAY_X_MAX, DISPLAY_Y_MAX, &Wire, RESET_PIN);

DHT_Unified dht(DHT11_DATA_PIN, DHT11);

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

  // Inicializando sensor de temperatura e umidade
  dht.begin();

  // Incializando botões
  pinMode(BOTAO_1_PIN, INPUT_PULLUP);
  pinMode(BOTAO_2_PIN, INPUT_PULLUP);
  pinMode(BOTAO_3_PIN, INPUT_PULLUP);

  // Configurando pwm para buzzer
  ledcSetup(CANAL, FREQUENCIA, RESOLUCAO);
  ledcAttachPin(BUZZER_PIN, CANAL);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.printf("Hello world !");
  display.display();
}

float umidade = 0;
float temperatura = 0;
long ultimoTempoGravado = 0;

void loop()
{

  display.clearDisplay();

  display.setCursor(0, 0);

  if (millis() - ultimoTempoGravado >= 250)
  {
    sensors_event_t event;

    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature))
      temperatura = event.temperature;

    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity))
      umidade = event.relative_humidity;

    ultimoTempoGravado = millis();
  }

  display.printf("TEMP : %.1f C\n", temperatura);
  display.printf("UMID : %.1f %%\n", umidade);
  display.printf("BT 1 : %s\n", BOTAO_1 ? "ON" : "OFF");
  display.printf("BT 2 : %s\n", BOTAO_2 ? "ON" : "OFF");
  display.printf("BT 3 : %s\n", BOTAO_3 ? "ON" : "OFF");
  display.printf("BUZZ : %s\n", BOTAO_1 && BOTAO_3 ? "ON" : "OFF");
  ledcWrite(CANAL, BOTAO_1 && BOTAO_3 ? 125 : 0);

  display.display();
}