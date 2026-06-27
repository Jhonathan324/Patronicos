#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

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
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

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

#define TamanhosMapaX 512
#define TamanhosMapaY 128


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

typedef struct Camera
{
    float x;
    float y;
} Camera;

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



#endif // TYPES_H_INCLUDED
