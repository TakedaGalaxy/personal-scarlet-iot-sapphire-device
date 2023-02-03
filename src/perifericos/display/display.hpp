#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h> 

#define SDA_PIN 4
#define SCL_PIN 15
#define RESET_PIN 16
#define X_MAX 128
#define Y_MAX 64

namespace display{
  void inicializa();
  void logica();
}