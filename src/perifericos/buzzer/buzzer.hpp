#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Buzzer sem oscilador interno
#define BUZZER_PIN 18
#define CANAL 0
#define RESOLUCAO 8

namespace buzzer
{
  // Realzia a configuração do PWM
  void inicializa();

  // Configura a frquencia para PWM enviado para o buzzer
  void setFrequencia(int valor);

  // Dispara uma task no core 0, para realizar o bip do buzzer
  void disparaBip(int tempoBipMs = 100, int quantidadeBips = 1);
}