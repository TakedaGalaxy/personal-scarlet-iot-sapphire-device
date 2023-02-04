#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Teclas
#define TECLA_1_PIN 19
#define TECLA_2_PIN 22
#define TECLA_3_PIN 21

#define QUANTIDADE 3

enum Teclas
{
  T_ANTERIOR = 0, // TECLA_1_PIN
  T_OK,           // TECLA_2_PIN
  T_PROXIMO       // TECLA_3_PIN
};

typedef struct
{
  bool precionada;
  bool statusLido;
  bool callBackChamada;
} StatusTecla;

typedef struct
{
  Teclas tecla;
  void (*p)(void);
} SetCallback;

namespace teclado
{
  // Inicializa pinos para leitura
  // Inicia task para realizar a leitura
  void inicializa();

  // Atualiza e retorna valor atual da tecla
  StatusTecla getTecla(Teclas tecla);

  // Chamda callback quando tecla precionada
  void setCallback(Teclas tecla, void (*callback)(void));
}