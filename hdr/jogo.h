#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED

#include "types.h"
#include "mapa.h"


typedef struct PlayerInJogo
{
    int coracoes;
    int estado_atual;
    int estado_passado;
    int coyote_time;
    bool costas;
    bool coli_h;
    bool coli_v;
    bool coli_v_m;
    float vida;
    float dano;
    float dano_sofrido;
    double frame;
    double pulo;
    double ataque;
    double tempo_hit;
    double tempo_safe;
    double acelera;
    double vel_max_x;
    double vel_max_y;
    double velocidade_x;
    double velocidade_y;
    double posicao_x;
    double posicao_y;
    double posicao_x_back;
    double posicao_y_back;
    VMM_Retangulo retangulo_dano;
    VMM_Retangulo retangulo_coli;
    VMM_Retangulo retangulo_img;
    ALLEGRO_BITMAP *sprite_atlas;
} PlayerInJogo;

typedef struct Inimigo
{
    int estado_atual;
    int estado_passado;
    int pulo;
    int coyote_time;
    bool costas;
    bool coli_h;
    bool coli_v;
    bool coli_v_m;
    float vida;
    float dano;
    double frame;
    double tempo_safe;
    double acelera;
    double vel_max_x;
    double vel_max_y;
    double velocidade_x;
    double velocidade_y;
    double posicao_x;
    double posicao_y;
    VMM_Retangulo retangulo_area;
    VMM_Retangulo retangulo_coli;
    VMM_Retangulo retangulo_coli_h;
    VMM_Retangulo retangulo_coli_v;
    VMM_Retangulo retangulo_img;
    TiposInimigo index;
} Inimigo;


typedef struct VariveisJogo
{
    ALLEGRO_COLOR cor_fundo;
    ALLEGRO_BITMAP *sprite_atlas_inimigos[3];

    PlayerInJogo jogador;
    Inimigo inimigos[10];
    Camera camera;
    Mapa mapa;

    int tamanho_bloco[2];
    double tempo;
} VariveisJogo;

// Funções principais
void InitCenaJogo(VariveisGerais *geral, VariveisJogo *jogo);
void CalcularCenaJogo(VariveisGerais *geral, VariveisJogo *jogo);
void LoopCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, double  delta_t);
void DesenharCenaJogo(VariveisGerais geral, VariveisJogo jogo);


void DesenharHud(VariveisGerais geral, VariveisJogo jogo);

PlayerInJogo InitPlayer(VMM_Retangulo retangulo_img, VMM_Retangulo retangulo_coli, char *img, float vida, int coracoes);
void CalcularPlayer(ALLEGRO_KEYBOARD_STATE *teclado, PlayerInJogo *player, double  delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]);
void DesenharPlayer(PlayerInJogo player, Camera camera);

Inimigo InitInimigo(VMM_Retangulo retangulo_img, VMM_Retangulo retangulo_area, VMM_Retangulo retangulo_coli, float vida, float dano, int index);
void CalcularInimigo(Inimigo *inimigo, double  delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]);
void DesenharInimigo(Inimigo inimigo, ALLEGRO_BITMAP *sprite_atlas, Camera camera);

TiposVMMA CalcularTipoVMMA(int n);
bool ColisaoComMapa(VMM_Retangulo *retangulo, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2], Camera camera);
void ColisaoPlayerInimigo(PlayerInJogo *jogador, Inimigo *inimigo);

#endif