#ifndef GERAl_H_INCLUDED
#define GERAl_H_INCLUDED

#include "types.h"
#include "utils.h"

// Funções especificas
void GetTamanhos(Tamanhos *tamanhos);

// Eventos e inicialização geral
void ModuloEvento(VariveisGerais *geral);
void InitCenaGeral(VariveisGerais *geral);
void CalcularGeral(VariveisGerais *geral);

// Função de erro 
void ErroFatal(const char *mensagem);

#endif
