#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Teclas
#define TECLA_1_PIN 19
#define TECLA_2_PIN 22
#define TECLA_3_PIN 21

#define QUANTIDADE 3

typedef struct
{
  bool precionada;
  bool statusLido;
  bool callBackChamada;
  bool erro;
} StatusTecla;

enum Teclas
{
  T_ANTERIOR = 0, // TECLA_1_PIN
  T_OK,           // TECLA_2_PIN
  T_PROXIMO       // TECLA_3_PIN
};

namespace teclado
{
  void inicializa();

  void atualizaStatus();

  StatusTecla getTeclaSatus(Teclas tecla);

  void setCallback(Teclas tecla, void (*p)(void));
}