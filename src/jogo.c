#include "../hdr/jogo.h"
#include <math.h>

// -------------------------------------------------------------
// HUD
// -------------------------------------------------------------
void DesenharHud(VariveisGerais geral, VariveisJogo jogo)
{
    VMM_Retangulo barra = {CantoFixo/3,CantoFixo/3,geral.tamanhos.barra_vida[0],geral.tamanhos.barra_vida[1]};
    al_draw_scaled_bitmap(
            geral.textura_hud,
            geral.barra_de_vida.x,geral.barra_de_vida.y,geral.barra_de_vida.w,geral.barra_de_vida.h,
            barra.x,barra.y,barra.w,barra.h,0);

    VMM_Retangulo vida  = geral.barra_de_vida;
    vida.y = EscalaHud * 5;
    al_draw_scaled_bitmap(
            geral.textura_hud,
            vida.x,vida.y,vida.w,vida.h,
            barra.x,barra.y,barra.w,barra.h,0);

    vida.y = EscalaHud * 7;
    vida.w *= (jogo.jogador.vida/100);
    barra.w *= (jogo.jogador.vida/100);
    al_draw_scaled_bitmap(
            geral.textura_hud,
            vida.x,vida.y,vida.w,vida.h,
            barra.x,barra.y,barra.w,barra.h,0);
}


// -------------------------------------------------------------
// Colisões unificadas
// -------------------------------------------------------------
bool ColisaoComMapa(VMM_Retangulo *retangulo, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2], Camera camera)
{
    int i = camera.y/tamanho_bloco[1];
    if(i<0) i =0;
    for(; i*tamanho_bloco[1] < tamanhos_tela[1] + camera.y && i < TamanhosMapaY; i++){
        int j = camera.x/tamanho_bloco[0];
        if (j < 0);
        j = 0;
        for(; j*tamanho_bloco[0] < tamanhos_tela[0] + camera.x && j < TamanhosMapaX; j++) {
            if (mapa.tiles[i][j]) {
                TiposVMMA tipo = CalcularTipoVMMA(mapa.tiles[i][j]);
                // Verifica se o bloco tem colisão
                switch(tipo){
                    case VMMA_GRAMA_ON:
                    case VMMA_PEDRA_ON:
                    case VMMA_MADEIRA_ON:{
                                    VMM_Retangulo bloco = {
                                        j*tamanho_bloco[0], 
                                        i*tamanho_bloco[1], 
                                        tamanho_bloco[0], 
                                        tamanho_bloco[1]};
                                    if(ColisaoRetangulo(&bloco, retangulo))
                                                    return true;
                    }break;
                }
            }
        }
    }
    return false;
}

TiposVMMA CalcularTipoVMMA(int n)
{
    TiposVMMA x = VMMA_SEM_COLI;
    #define X(index, x_loc, y_loc, tipo) \
        case index: { x = tipo; } break;
    switch(n-1) {
        TabelaBlocoAtlas
    }
    #undef X
    return x;
}


// -------------------------------------------------------------
// Inicialização e loop da cena de jogo
// -------------------------------------------------------------
void InitCenaJogo(VariveisGerais *geral, VariveisJogo *jogo)
{
    geral->jogador.vida = 100;
    geral->jogador.coracoes = 3;

    // Inicializa jogador
    VMM_Retangulo img = {0, 0, geral->tamanhos.jogador[0], geral->tamanhos.jogador[1]};
    VMM_Retangulo coli = {geral->tamanhos.bloco1[0]*2,62*geral->tamanhos.bloco1[1], geral->tamanhos.jogador_coli[0], geral->tamanhos.jogador_coli[1]};
    jogo->jogador = InitPlayer(img, coli, "../../sprites/entities/player/Guerreiro.png", geral->jogador.vida, geral->jogador.coracoes);

    // Inicializa inimigo (exemplo)
    VMM_Retangulo area = {geral->tamanhos.bloco1[0]*4, 65*geral->tamanhos.bloco1[1] - geral->tamanhos.inimigo1[1], geral->tamanhos.inimigo1[0]*5, geral->tamanhos.inimigo1[1]};
    VMM_Retangulo coli_inim = {geral->tamanhos.bloco1[0]*4, 65*geral->tamanhos.bloco1[1] - geral->tamanhos.inimigo1[1], geral->tamanhos.inimigo1[0], geral->tamanhos.inimigo1[1]};
    VMM_Retangulo img_inim = {geral->tamanhos.bloco1[0]*4, 65*geral->tamanhos.bloco1[1] - geral->tamanhos.inimigo1[1], geral->tamanhos.inimigo1[0], geral->tamanhos.inimigo1[1]};
    jogo->inimigos[0] = InitInimigo(img_inim, area, coli_inim, 50, 10, PORCO_NORMAL);

    // Carrega sprite dos inimigos
    jogo->sprite_atlas_inimigos[0] = al_load_bitmap("../../sprites/entities/mobs/porco marron.png");

    // Inicializa mapa
    jogo->mapa.n = 0;
    jogo->mapa.textura = al_load_bitmap("../../sprites/world/tiles/Tiles.png");
    CarregarMapa(&jogo->mapa, 0);
    jogo->mapa.area_vitoria = (VMM_Retangulo){20*geral->tamanhos.bloco1[0], 55*geral->tamanhos.bloco1[1], 10*geral->tamanhos.bloco1[0], 10*geral->tamanhos.bloco1[0]};

    jogo->camera.x = 0;
    jogo->camera.y = 0;
    jogo->tamanho_bloco[0] = geral->tamanhos.bloco1[0];
    jogo->tamanho_bloco[1] = geral->tamanhos.bloco1[1];
    jogo->tempo = 0;
}

void CalcularCenaJogo(VariveisGerais *geral, VariveisJogo *jogo)
{

    // Atualiza dimensões do jogador com base na resolução atual
    VMM_Retangulo img = {0, 0, geral->tamanhos.jogador[0], geral->tamanhos.jogador[1]};
    VMM_Retangulo coli = {0, 0, geral->tamanhos.jogador_coli[0], geral->tamanhos.jogador_coli[1]};
    jogo->jogador.retangulo_img = img;
    jogo->jogador.retangulo_coli = coli;
    jogo->jogador.acelera = coli.w / 100.0;
    jogo->jogador.vel_max_x = coli.w / 20.0;
    jogo->jogador.vel_max_y = coli.h / 20.0;
    jogo->tamanho_bloco[0] = geral->tamanhos.bloco1[0];
    jogo->tamanho_bloco[1] = geral->tamanhos.bloco1[1];
}

void LoopCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, double  delta_t)
{
    // Teclado
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state);

    // Pausa
    if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) {
        geral->cena_continuar = geral->cena;
        geral->cena_passada = geral->cena;
        geral->cena = CENA_PAUSE;
    }

    // Game over
    if (jogo->jogador.coracoes <= 0) {
        geral->cena_continuar = geral->cena;
        geral->cena_passada = geral->cena;
        geral->cena = CENA_MENU;
        jogo->jogador.coracoes = 3;
        jogo->jogador.vida = 100;
    }

    // Vitória
    if (jogo->jogador.retangulo_coli.x < jogo->mapa.area_vitoria.x + jogo->mapa.area_vitoria.w &&
        jogo->jogador.retangulo_coli.x + jogo->jogador.retangulo_coli.w > jogo->mapa.area_vitoria.x &&
        jogo->jogador.retangulo_coli.y < jogo->mapa.area_vitoria.y + jogo->mapa.area_vitoria.h &&
        jogo->jogador.retangulo_coli.y + jogo->jogador.retangulo_coli.h > jogo->mapa.area_vitoria.y) {
        // Vitória
        printf("VITORIA!\n");
    }

    // Câmera segue o jogador
    if(-jogo->camera.x+jogo->jogador.retangulo_coli.x + jogo->jogador.retangulo_coli.w > geral->resolucao_atual[0]*0.6) 
        jogo->camera.x = jogo->jogador.retangulo_coli.x  + jogo->jogador.retangulo_coli.w - geral->resolucao_atual[0]*0.6;
    
    else if(-jogo->camera.x+jogo->jogador.retangulo_coli.x  < geral->resolucao_atual[0]*0.4)
        jogo->camera.x = jogo->jogador.retangulo_coli.x - geral->resolucao_atual[0]*0.4;
    
    if(-jogo->camera.y+jogo->jogador.retangulo_coli.y + jogo->jogador.retangulo_coli.h > geral->resolucao_atual[1]*0.7) 
        jogo->camera.y = jogo->jogador.retangulo_coli.y  + jogo->jogador.retangulo_coli.h - geral->resolucao_atual[1]*0.7;
    
    else if(-jogo->camera.y+jogo->jogador.retangulo_coli.y  < geral->resolucao_atual[1]*0.3)
        jogo->camera.y = jogo->jogador.retangulo_coli.y - geral->resolucao_atual[1]*0.3;
    
    if(jogo->camera.x<0)
        jogo->camera.x=0;
        
    if(jogo->camera.y<0)
        jogo->camera.y=0;
    

    //jogo->camera.x = jogo->jogador.retangulo_coli.x -geral->tamanhos.tela[0]/2; 
    //jogo->camera.y = jogo->jogador.retangulo_coli.y -geral->tamanhos.tela[1]/2; 
    

    // Atualiza jogador
    CalcularPlayer(&key_state, &jogo->jogador, delta_t, &jogo->camera, jogo->mapa, jogo->tamanho_bloco, geral->resolucao_atual);
    // Atualiza inimigos (apenas os que estão na tela)
    VMM_Retangulo camera_rect = {
        jogo->camera.x - geral->resolucao_atual[0]/2,
        jogo->camera.y - geral->resolucao_atual[1]/2,
        geral->resolucao_atual[0]*2,
        geral->resolucao_atual[1]*2
    };
    for (int i = 0; i < 10; i++) {
        if (jogo->inimigos[i].vida > 0) {
            if (jogo->inimigos[i].retangulo_coli.x < camera_rect.x + camera_rect.w &&
                jogo->inimigos[i].retangulo_coli.x + jogo->inimigos[i].retangulo_coli.w > camera_rect.x &&
                jogo->inimigos[i].retangulo_coli.y < camera_rect.y + camera_rect.h &&
                jogo->inimigos[i].retangulo_coli.y + jogo->inimigos[i].retangulo_coli.h > camera_rect.y) {
                CalcularInimigo(&jogo->inimigos[i], delta_t, &jogo->camera, jogo->mapa, jogo->tamanho_bloco, geral->resolucao_atual);
                ColisaoPlayerInimigo(&jogo->jogador, &jogo->inimigos[i]);
            }
        }
    }
}

void DesenharCenaJogo(VariveisGerais geral, VariveisJogo jogo)
{
    // Limpeza
    al_clear_to_color(al_map_rgb(jogo.cor_fundo.r, jogo.cor_fundo.g, jogo.cor_fundo.b));

    // Mapa
    DesenharMapa(geral, jogo.mapa, jogo.camera, jogo.tamanho_bloco);

    // Inimigos
    for (int i = 0; i < 10; i++) {
        if (jogo.inimigos[i].vida > 0) {
            DesenharInimigo(jogo.inimigos[i], jogo.sprite_atlas_inimigos[jogo.inimigos[i].index], jogo.camera);
        }
    }

    // Jogador
    DesenharPlayer(jogo.jogador, jogo.camera);

    // HUD
    DesenharHud(geral, jogo);

    // Área de vitória (debug)
    al_draw_filled_rectangle(
        jogo.mapa.area_vitoria.x - jogo.camera.x,
        jogo.mapa.area_vitoria.y - jogo.camera.y,
        jogo.mapa.area_vitoria.x - jogo.camera.x + jogo.mapa.area_vitoria.w,
        jogo.mapa.area_vitoria.y - jogo.camera.y + jogo.mapa.area_vitoria.h,
        al_map_rgba(0, 255, 0, 100));
}