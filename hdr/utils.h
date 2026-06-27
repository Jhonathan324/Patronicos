#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "types.h"

// Retangulos
bool ColisaoRetangulo(VMM_Retangulo *r1, VMM_Retangulo *r2);
void DesenharRetangulo(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor);
void DesenharImagemEscala(ALLEGRO_BITMAP *textura, VMM_Retangulo *retangulo_in,
                          float left_width, float right_width,
                          float top_height, float bottom_height,
                          float escala, VMM_Retangulo *retangulo_destino);

void CentralizarRectInRect(VMM_Retangulo *rect_pai, VMM_Retangulo *rect_filho);
void CentralizarRectsInRectV(VMM_Retangulo *pai, VMM_Retangulo *filho[], int n, float borda_x, float borda_y);

// Texto
CampoTexto InitTexto(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor_fundo,
                     char *texto, char *imagem, ALLEGRO_FONT *fonte,
                     ALLEGRO_COLOR cor_fonte, bool alinhado);
void DesenharTexto(CampoTexto texto);
void DestruirTexto(CampoTexto *texto);

// Moldura
Moldura InitMoldura(VMM_Retangulo *retangulo, char *file);
void DesenharMoldura(Moldura moldura);
void DestruirMoldura(Moldura *moldura);

// Marcador
Marcador InitMarcador(VMM_Retangulo *retangulo, bool ativo, char *imagem1,
                      ALLEGRO_COLOR cor1, ALLEGRO_COLOR cor2);
void DesenharMarcador(Marcador marcador);
bool VerificarMarcador(Marcador *marcador, VMM_Ponto mouse, bool click);
void DestruirMarcador(Marcador *marcador);

// Botão simples
Botao InitBotao(VMM_Retangulo *retangulo, char *imagem, char *texto,
                ALLEGRO_COLOR cor1, ALLEGRO_COLOR cor2, int indice,
                ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor_fonte);
void DesenharBotao(ALLEGRO_FONT *font, Botao botao);
bool VerificarBotao(Botao *botao, VMM_Ponto mouse, bool click);
void DestruirBotao(Botao *botao);

// Botão expansivo
BotaoExpansivo InitBotaoExpansivo(VMM_Retangulo *retangulo, char *imagem, char *texto,
                                  char *textos[], ALLEGRO_COLOR cor, ALLEGRO_COLOR cor2,
                                  int indice, ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor_fonte,
                                  int n);
void DesenharBotaoExpansivo(BotaoExpansivo botao);
void DestruirBotaoExpansivo(BotaoExpansivo *botao);

#endif // UTILS_H_INCLUDED
