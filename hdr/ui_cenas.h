#ifndef UI_CENAS_H_INCLUDED
#define UI_CENAS_H_INCLUDED
#include "types.h"
#include "utils.h"


typedef struct VariveisMenu {
    ALLEGRO_COLOR cor_fundo;
    ALLEGRO_BITMAP *imagem;
    ALLEGRO_FONT *fonte;
    Moldura moldura;
    Botao botao_iniciar;
    Botao botao_criacao;
    Botao botao_conf;
    Botao botao_sair;
} VariveisMenu;

typedef struct VariveisPause {
    ALLEGRO_COLOR cor_fundo;
    ALLEGRO_BITMAP *imagem;
    ALLEGRO_FONT *fonte;
    Moldura moldura;
    Botao botao_iniciar;
    Botao botao_conf;
    Botao botao_sair;
} VariveisPause;

typedef struct VariveisConf {
    ALLEGRO_COLOR cor_fundo;
    ALLEGRO_BITMAP *imagem;
    ALLEGRO_FONT *fonte;
    bool valida_fullscrean;
    int reso_inicial;
    Marcador troca_fullscreen;
    Botao botao_sair;
    BotaoExpansivo botao_reso;
    Moldura moldura;
    CampoTexto texto_reso;
    CampoTexto texto_full;
} VariveisConf;

void InitCenaMenu(VariveisGerais *geral, VariveisMenu *menu);
void LoopCenaMenu(VariveisGerais *geral, VariveisMenu *menu);
void DesenharCenaMenu(VariveisGerais geral, VariveisMenu menu);

void InitCenaPause(VariveisGerais *geral, VariveisPause *pause);
void LoopCenaPause(VariveisGerais *geral, VariveisPause *pause);
void DesenharCenaPause(VariveisGerais geral, VariveisPause pause);

void InitCenaConf(VariveisGerais *geral, VariveisConf *conf);
void LoopCenaConf(VariveisGerais *geral, VariveisConf *conf);
void DesenharCenaConf(VariveisGerais geral, VariveisConf conf);


#endif // UI_CENAS_H_INCLUDED
