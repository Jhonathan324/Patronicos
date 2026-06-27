#ifndef GERAl_H_INCLUDED
#define GERAl_H_INCLUDED

#include "types.h"

// Funções especificas
void GetTamanhos(Tamanhos *tamanhos);

// Efeitos (fogo)
ALLEGRO_COLOR CorFogo(char cor);
void DesenharFogo(float tamanho_tela[2], char matriz[][64]);

// Eventos e inicialização geral
void ModuloEvento(VariveisGerais *geral);
void InitCenaGeral(VariveisGerais *geral);
void CalcularGeral(VariveisGerais *geral);

// Função de erro 
void ErroFatal(const char *mensagem);

#endif
