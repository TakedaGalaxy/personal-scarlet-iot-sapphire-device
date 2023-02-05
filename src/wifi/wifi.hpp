#include <Arduino.h>
#include <WiFi.h>
#include <String>

enum Periferico
{
  P_NADA = 0,
  P_BUZZER
};

typedef struct
{
  Periferico idPeriferico;
  float dado;
} ComandoPeriferico;

typedef struct
{
  Periferico idPeriferico;
  char nome[20];
  float dado;
  char contexto[20];
}DadoPeriferico;

namespace wifi
{
  void inicizaliza();
  void conectar();
  IPAddress getIp();
  const char *getStatus();
  ComandoPeriferico getCommando();
  void enviarDado(DadoPeriferico dado);
}