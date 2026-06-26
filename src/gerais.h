#ifndef GERAIS_H_INCLUDED
#define GERAIS_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/mouse.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "X Macros.h"

#define CORCORFUNDO (20 , 20 , 20 )
#define COR1        (20 , 20 , 20 )
#define PRETO       (0  , 0  , 0  )
#define BRANCO      (255, 255, 255)
#define VERMELHO    (255, 20 , 20 )
#define VERDE       (20 , 255, 20 )
#define AZUL        (20 , 20 , 255)
#define AZUL_CLARO  (140, 210, 230)
#define SEMI_PRETO  (0  , 0  , 0  )

#define EscalaHud 16
#define EscalaMoldura 16
#define EscalaMarcador 16
#define EscalaBotao 8
#define CantoFixo 48
#define MedidaImgPlayerX 120
#define MedidaImgPlayerY 80
#define MedidaImgPlayerColiX 21
#define MedidaImgPlayerColiY 38
#define MedidaImgInimigo1X 48
#define MedidaImgInimigo1Y 32
#define MedidaImgInimigo2X 120
#define MedidaImgInimigo2Y 80
#define MedidaImgBloco 16

// enuns
typedef enum {
    CENA_MENU,
    CENA_JOGO,
    CENA_PAUSE,
    CENA_CRIACAO,
    CENA_CONF,
    CENA_VITORIA,
    CENA_DERROTA,
    CENA_CREDITOS,
    CENA_SAIR,
} ESTADO_JOGO;

// elementos essenciais
typedef struct VMM_Retangulo {
    float x;
    float y;
    float w;
    float h;
} VMM_Retangulo;

typedef struct VMM_Ponto {
    float x, y;
} VMM_Ponto;

// elementos UI
typedef struct CampoTexto {
    VMM_Retangulo retangulo;
    char *texto;
    ALLEGRO_COLOR cor_fundo;
    ALLEGRO_BITMAP *textura_texto;
    ALLEGRO_BITMAP *imagem;
    float proporcao;
} CampoTexto;

typedef struct Moldura {
    VMM_Retangulo retangulo;
    ALLEGRO_BITMAP *textura;
} Moldura;

typedef struct Marcador {
    VMM_Retangulo retangulo;
    int timer;
    bool sobre;
    bool ativo;
    ALLEGRO_COLOR cor1;
    ALLEGRO_COLOR cor2;
    ALLEGRO_BITMAP *imagem1;
} Marcador;

typedef struct Botao {
    VMM_Retangulo retangulo;
    char *texto;
    float proporcao;
    int timer;
    bool sobre;
    int indice;
    ALLEGRO_COLOR cor1;
    ALLEGRO_COLOR cor2;
    ALLEGRO_BITMAP *textura;
    ALLEGRO_BITMAP *imagem;
} Botao;

typedef struct BotaoExpansivo {
    int n;
    Botao botao_pai;
    Botao *botao_filho;
    bool expandido;
} BotaoExpansivo;

typedef struct Tamanhos {
    int escala;
    float tela[2];
    float menu[2];
    float botao1[2];
    float botao2[2];
    float barra_vida[2];
    float jogador[2];
    float jogador_coli[2];
    float inimigo1[2];
    float inimigo2[2];
    float bloco1[2];
} Tamanhos;

typedef struct Player {
    int vida;
    int coracoes;
} Player;

typedef struct VariveisGerais {
    ALLEGRO_DISPLAY *janela;
    ALLEGRO_EVENT_QUEUE *fila_evento;
    ALLEGRO_EVENT evento;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *fonte;
    VMM_Ponto ponto_mouse;
    VMM_Ponto ponto_mouse_back;
    ALLEGRO_BITMAP *textura_hud;
    VMM_Retangulo barra_de_vida;
    bool debug;
    bool rodando;
    bool fullscrean;
    bool troca_reso;
    bool carregar_mapa;
    char matriz_fogo[40][64];
    int botao_mouse_direito;
    int botao_mouse_esquerdo;
    int botao_mouse_meio;
    int botao_mouse_gira;
    int resolucao_atual[2];
    int resolucao_antiga[2];
    Tamanhos tamanhos;
    Player jogador;
    ESTADO_JOGO cena;
    ESTADO_JOGO cena_passada;
    ESTADO_JOGO cena_continuar;
} VariveisGerais;

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

// Funções primordiais
bool ColisaoRetangulo(VMM_Retangulo r1, VMM_Retangulo r2);
void DesenharRetangulo(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor);
void DesenharImagemEscala(ALLEGRO_BITMAP *textura, VMM_Retangulo *retangulo_in,
                          float left_width, float right_width,
                          float top_height, float bottom_height,
                          float escala, VMM_Retangulo *retangulo_destino);

// Funções especificas
void GetTamanhos(Tamanhos *tamanhos);

// Ajustes dinâmicos
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

// Efeitos (fogo)
ALLEGRO_COLOR CorFogo(char cor);
void DesenharFogo(float tamanho_tela[2], char matriz[][64]);

// Eventos e inicialização geral
void ModuloEvento(VariveisGerais *geral);
void InitCenaGeral(VariveisGerais *geral);
void CalcularGeral(VariveisGerais *geral);

// Cenas
void InitCenaMenu(VariveisGerais *geral, VariveisMenu *menu);
void LoopCenaMenu(VariveisGerais *geral, VariveisMenu *menu);
void DesenharCenaMenu(VariveisGerais geral, VariveisMenu menu);

void InitCenaPause(VariveisGerais *geral, VariveisPause *pause);
void LoopCenaPause(VariveisGerais *geral, VariveisPause *pause);
void DesenharCenaPause(VariveisGerais geral, VariveisPause pause);

void InitCenaConf(VariveisGerais *geral, VariveisConf *conf);
void LoopCenaConf(VariveisGerais *geral, VariveisConf *conf);
void DesenharCenaConf(VariveisGerais geral, VariveisConf conf);

// Função de erro 
void ErroFatal(const char *mensagem);

#endif
