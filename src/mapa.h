#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include "jogo.h"

typedef struct MarcadorBloco {
    VMM_Retangulo retangulo;
    bool sobre;
    bool ativo;
    ALLEGRO_COLOR cor1;
    ALLEGRO_COLOR cor2;
} MarcadorBloco;

typedef struct VariaveisMapa {
    ALLEGRO_COLOR cor_fundo;
    VMM_Retangulo selecao;
    VMM_Ponto selecao_coli;
    VMM_Ponto selecao_coli_back;
    VMM_Ponto selecao_coli_preencimento;
    int rolada;
    int bloco_uso;
    int velocidade;
    int tamanho_bloco[2];
    bool preencher;
    Mapa mapa;
    Camera camera;
    Camera camera_back;
    Botao botao_salvar;
    Botao botao_carregar;
    Moldura moldura_bloco;
    Marcador marcador_preencher;
    MarcadorBloco marcador[600];
} VariaveisMapa;

void InitCenaMapa(VariveisGerais *geral, VariaveisMapa *mapa);
void LoopCenaMapa(VariveisGerais *geral, VariaveisMapa *mapa);
void DesenharCenaMapa(VariveisGerais geral, VariaveisMapa mapa);

#endif