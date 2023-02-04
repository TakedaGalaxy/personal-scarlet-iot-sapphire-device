#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Buzzer sem oscilador interno
#define BUZZER_PIN 18
#define CANAL 0
#define RESOLUCAO 8

typedef struct
{
  int duracao;
  int repeticoes;
  int frequencia;
} Comando;

namespace buzzer
{
  // Realiza a configuração do PWM
  // Cria task pra emissão sonora
  void inicializa();

  // Emite um som de acordo com o comando
  // Enviando o comando para task
  // Retorna True se for evniado para queue
  // Retorna False se não for enviado para queue
  bool emiteSom(Comando comando = {100, 1, 500});
}