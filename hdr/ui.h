#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "utils.h"

// Texto
CampoTexto InitTexto(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor_fundo, char *texto, char *imagem, ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor_fonte, bool alinhado);
void DesenharTexto(CampoTexto texto);
void DestruirTexto(CampoTexto *texto);

// Moldura
Moldura InitMoldura(VMM_Retangulo *retangulo, char *file);
void DesenharMoldura(Moldura moldura);
void DestruirMoldura(Moldura *moldura);

// Marcador
Marcador InitMarcador(VMM_Retangulo *retangulo, bool ativo, char *imagem1, ALLEGRO_COLOR cor1, ALLEGRO_COLOR cor2);
void DesenharMarcador(Marcador marcador);
bool VerificarMarcador(Marcador *marcador, VMM_Ponto mouse, bool click);
void DestruirMarcador(Marcador *marcador);

// Botão simples
Botao InitBotao(VMM_Retangulo *retangulo, char *imagem, char *texto, ALLEGRO_COLOR cor1, ALLEGRO_COLOR cor2, int indice, ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor_fonte);
void DesenharBotao(ALLEGRO_FONT *font, Botao botao, VMM_Retangulo *dentro);
bool VerificarBotao(Botao *botao, VMM_Ponto mouse, bool click);
void DestruirBotao(Botao *botao);

// Botão expansivo
BotaoExpansivo InitBotaoExpansivo(VMM_Retangulo *retangulo, char *imagem, char *texto, char *textos[], ALLEGRO_COLOR cor, ALLEGRO_COLOR cor2, int indice, ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor_fonte, int n);
void DesenharBotaoExpansivo(ALLEGRO_FONT *font, BotaoExpansivo botao, VMM_Retangulo *dentro);
void DestruirBotaoExpansivo(BotaoExpansivo *botao);

#endif // UI_H_INCLUDED
