#include "jogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// -------------------------------------------------------------
// HUD
// -------------------------------------------------------------
void DesenharHud(VariveisGerais geral, VariveisJogo jogo)
{
    // Usa a textura hud já carregada em geral.textura_hud
    // Desenha a barra de vida, corações, etc.
    // Exemplo simplificado:
    VMM_Retangulo barra = {CantoFixo/3, CantoFixo/3, geral.tamanhos.barra_vida[0], geral.tamanhos.barra_vida[1]};
    // Desenha fundo da barra (usando a textura hud)
    al_draw_scaled_bitmap(geral.textura_hud,
        0, 0, 64, 16,  // origem (exemplo)
        barra.x, barra.y, barra.w, barra.h, 0);

    // Vida atual
    float percent = jogo.jogador.vida / 100.0f;
    VMM_Retangulo vida_barra = barra;
    vida_barra.w *= percent;
    al_draw_scaled_bitmap(geral.textura_hud,
        0, 16, 64, 16,  // outra parte da textura
        vida_barra.x, vida_barra.y, vida_barra.w, vida_barra.h, 0);
}

// -------------------------------------------------------------
// Mapa (desenho)
// -------------------------------------------------------------
VMM_Retangulo MapaTiles(int n)
{
    VMM_Retangulo rect = {0, 0, MedidaImgBloco, MedidaImgBloco};
    #define X(index, x_loc, y_loc, tipo) \
        case index: { rect.x = x_loc * MedidaImgBloco; rect.y = y_loc * MedidaImgBloco; } break;
    switch(n-1) {
        TabelaBlocoAtlas
    }
    #undef X
    return rect;
}

void DesenharMapa(VariveisGerais geral, Mapa mapa, Camera camera, int tamanho_bloco[2])
{
    int tela_w = geral.tamanhos.tela[0];    
    int tela_h = geral.tamanhos.tela[1];

    
    int i = camera.y / tamanho_bloco[1];
    if (i < 0) i = 0;
    for (; i * tamanho_bloco[1] < tela_h + camera.y && i < TamanhosMapaY; i++) {
        int j = camera.x / tamanho_bloco[0];
        if (j < 0) j = 0;
        for (; j * tamanho_bloco[0] < tela_w + camera.x && j < TamanhosMapaX; j++) {
            if (mapa.tiles[i][j]) {
                VMM_Retangulo src = MapaTiles(mapa.tiles[i][j]);
                VMM_Retangulo dst = {
                    j * tamanho_bloco[0] - camera.x,
                    i * tamanho_bloco[1] - camera.y,
                    tamanho_bloco[0],
                    tamanho_bloco[1]
                };

                al_draw_scaled_bitmap(mapa.textura,
                    src.x, src.y, src.w, src.h,
                    dst.x, dst.y, dst.w, dst.h, 0);
                
            }
        }
    }
}

double VMM_Clamp(double x,  double minimo, double maximo){
    if(x > maximo) return maximo;
    if(x < minimo) return minimo;
    return x;
}

// -------------------------------------------------------------
// Player
// -------------------------------------------------------------
PlayerInJogo InitPlayer(VMM_Retangulo retangulo_img, VMM_Retangulo retangulo_coli, char *img, float vida, int coracoes)
{
    PlayerInJogo jogador = {
        .coracoes = coracoes,
        .estado_atual = 0,
        .estado_passado = 0,
        .pulo = 0,
        .ataque = 0,
        .costas = 0,
        .coli_h = 0,
        .coli_v = 0,
        .coli_v_m = 0,
        .vida = vida,
        .dano = 10,
        .dano_sofrido = 0,
        .frame = 0,
        .tempo_safe = 0,
        .tempo_hit = 0,
        .acelera = retangulo_coli.w / 100.0,
        .vel_max_x = retangulo_coli.w / 20.0,
        .vel_max_y = retangulo_coli.h / 20.0,
        .velocidade_x = 0,
        .velocidade_y = 0,
        .posicao_x = retangulo_coli.x,
        .posicao_y = retangulo_coli.y,
        .posicao_x_back = retangulo_coli.x,
        .posicao_y_back = retangulo_coli.y,
        .retangulo_img = retangulo_img,
        .sprite_atlas = al_load_bitmap(img)
    };
    return jogador;
}

void redimencionar_jogador(PlayerInJogo *jogador, VMM_Ponto local)
{

    jogador->posicao_x = local.x;
    jogador->posicao_y = local.y - jogador->retangulo_coli.h;
    jogador->retangulo_coli.x = jogador->posicao_x;
    jogador->retangulo_coli.y = jogador->posicao_y;
}

void CalcularPlayer(ALLEGRO_KEYBOARD_STATE *teclado, PlayerInJogo *jogador, double  delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2])
{
    double movi_v = 1.0, movi_h = 0.0;
    // Se vida <= 0, perde coração e volta ao spawn
    if (jogador->vida <= 0) {
        jogador->vida = 100;
        jogador->coracoes--;
        jogador->dano_sofrido = 0;
        redimencionar_jogador(jogador, (VMM_Ponto){tamanho_bloco[0], 62 * tamanho_bloco[1]});
    }

    jogador->frame += delta_frame;

    // Movimento horizontal
    if(al_key_down(teclado, ALLEGRO_KEY_D))
        movi_h += 1.0;
    if(al_key_down(teclado, ALLEGRO_KEY_A))
        movi_h -= 1.0;
    

    if (movi_h < 0) jogador->costas = true;
    if (movi_h > 0) jogador->costas = false;
    if (movi_h == 0) jogador->velocidade_x = 0.0;

    // Pulo
    jogador->pulo -= delta_frame;
    if (jogador->pulo < 0) jogador->pulo = 0.0;
    //printf("delta frame %lf\n",delta_frame);
    //printf("pulo %lf\n\n",jogador->pulo);

    // Gravidade e pulo
    
    jogador->velocidade_y -= jogador->acelera  * (jogador->pulo / 6);
    jogador->velocidade_y += jogador->acelera  * movi_v;
    jogador->velocidade_y = VMM_Clamp(jogador->velocidade_y, -jogador->vel_max_y, jogador->vel_max_y);
    double velocidade = VMM_Clamp(jogador->velocidade_y , -jogador->vel_max_y, jogador->vel_max_y);

    // Colisão vertical
    jogador->posicao_y_back = jogador->posicao_y;
    jogador->posicao_y += velocidade;
    jogador->retangulo_coli.y = jogador->posicao_y;
    jogador->coli_v = ColisaoComMapa(&jogador->retangulo_coli, mapa, tamanho_bloco, tamanhos_tela, *camera);

    if (jogador->coli_v) {
        if (jogador->pulo) {
            jogador->pulo = 0;
            jogador->coli_v = false;
        } else {
            jogador->coyote_time = 0;
            if (al_key_down(teclado, ALLEGRO_KEY_SPACE) && !jogador->pulo) {
                jogador->pulo = 50;
            }
            movi_v = 0;
            jogador->velocidade_y = 0;
            jogador->posicao_y = jogador->posicao_y_back;
            jogador->retangulo_coli.y = jogador->posicao_y_back;
        }
    }
    if (jogador->estado_atual != VMM_PLAYER_CAIR && jogador->posicao_y != jogador->posicao_y_back)
        jogador->coyote_time++;

    // Colisão horizontal
    jogador->velocidade_x += jogador->acelera * movi_h;
    jogador->velocidade_x = VMM_Clamp(jogador->velocidade_x, -jogador->vel_max_x, jogador->vel_max_x);
    velocidade = VMM_Clamp(jogador->velocidade_x , -jogador->vel_max_x, jogador->vel_max_x);

    jogador->posicao_x_back = jogador->posicao_x;
    jogador->posicao_x += velocidade;
    jogador->retangulo_coli.x = jogador->posicao_x;
    jogador->coli_h = ColisaoComMapa(&jogador->retangulo_coli, mapa, tamanho_bloco, tamanhos_tela, *camera);
    if (jogador->coli_h) {
        printf("coli h\n");
        jogador->velocidade_x = 0;
        jogador->posicao_x = jogador->posicao_x_back;
        jogador->retangulo_coli.x = jogador->posicao_x_back;
    }


    
    // Lógica de ataque (simplificada)
    if (jogador->ataque) {
        jogador->ataque -= delta_frame;
        if (jogador->ataque < 0) jogador->ataque = 0;
        jogador->retangulo_dano.w = 0;
    }
    if (al_key_down(teclado, ALLEGRO_KEY_F) && !jogador->ataque) {
        jogador->ataque = 40;
        jogador->retangulo_dano.w = tamanho_bloco[0] * 2;
        jogador->retangulo_dano.h = tamanho_bloco[1] * 2;
        jogador->retangulo_dano.y = jogador->retangulo_coli.y;
        if (jogador->costas)
            jogador->retangulo_dano.x = jogador->retangulo_coli.x - jogador->retangulo_dano.w * 1.2;
        else
            jogador->retangulo_dano.x = jogador->retangulo_coli.x + jogador->retangulo_coli.w * 1.2;
    }

    // Dano recebido
    if (jogador->dano_sofrido) {
        if (jogador->dano_sofrido > delta_frame && jogador->tempo_safe) {
            jogador->dano_sofrido -= delta_frame;
            jogador->vida -= delta_frame;
        } else {
            jogador->vida -= jogador->dano_sofrido;
            jogador->dano_sofrido = 0;
        }
    }
    if (jogador->tempo_hit) {
        movi_h = 0.0;
        jogador->velocidade_x = 0;
        jogador->velocidade_y = 0;
        if (jogador->tempo_hit > delta_frame)
            jogador->tempo_hit -= delta_frame;
        else
            jogador->tempo_hit = 0;
        jogador->estado_atual = VMM_PLAYER_HIT;
    } else if (jogador->estado_atual == VMM_PLAYER_HIT) {
        jogador->estado_atual = jogador->estado_passado;
    }
    if (jogador->tempo_safe) {
        jogador->tempo_safe -= delta_frame;
        if (jogador->tempo_safe < 0) jogador->tempo_safe = 0;
    }

    // Atualiza estado da animação (simplificado)
    // (implementação completa similar ao SDL3)
    if (!jogador->tempo_hit) {
        if (!jogador->coli_v) {
            if (jogador->pulo) {
                if (jogador->ataque) {
                    if (jogador->estado_passado != VMM_PLAYER_ATAQUE2_MOVIMENTO && jogador->estado_passado != VMM_PLAYER_ATAQUE2)
                        jogador->frame = 0;
                    jogador->estado_passado = jogador->estado_atual;
                    jogador->estado_atual = VMM_PLAYER_ATAQUE2_MOVIMENTO;
                } else {
                    if (jogador->estado_atual != VMM_PLAYER_PULAR && jogador->estado_atual != VMM_PLAYER_PULO_TRANSICAO && jogador->estado_atual != VMM_PLAYER_CAIR) {
                        jogador->frame = 0;
                        jogador->estado_passado = jogador->estado_atual;
                        jogador->estado_atual = VMM_PLAYER_PULAR;
                    } else if (jogador->estado_atual == VMM_PLAYER_PULAR && jogador->pulo < 4) {
                        jogador->frame = 0;
                        jogador->estado_passado = jogador->estado_atual;
                        jogador->estado_atual = VMM_PLAYER_PULO_TRANSICAO;
                    }
                }
            } else if (jogador->coyote_time > 3) {
                if (jogador->ataque) {
                    if (jogador->estado_passado != VMM_PLAYER_ATAQUE2_MOVIMENTO && jogador->estado_passado != VMM_PLAYER_ATAQUE2)
                        jogador->frame = 0;
                    jogador->estado_passado = jogador->estado_atual;
                    jogador->estado_atual = VMM_PLAYER_ATAQUE2_MOVIMENTO;
                } else {
                    jogador->frame = 0;
                    jogador->estado_passado = jogador->estado_atual;
                    jogador->estado_atual = VMM_PLAYER_CAIR;
                }
            }
        } else {
            if (movi_h) {
                if (jogador->ataque) {
                    if (jogador->estado_passado != VMM_PLAYER_ATAQUE2_MOVIMENTO && jogador->estado_passado != VMM_PLAYER_ATAQUE2)
                        jogador->frame = 0;
                    jogador->estado_passado = jogador->estado_atual;
                    jogador->estado_atual = VMM_PLAYER_ATAQUE2_MOVIMENTO;
                } else {
                    if (jogador->estado_passado != VMM_PLAYER_CORRER)
                        jogador->frame = 0;
                    jogador->estado_passado = jogador->estado_atual;
                    jogador->estado_atual = VMM_PLAYER_CORRER;
                }
            } else {
                if (jogador->ataque) {
                    if (jogador->estado_passado != VMM_PLAYER_ATAQUE2_MOVIMENTO && jogador->estado_passado != VMM_PLAYER_ATAQUE2)
                        jogador->frame = 0;
                    jogador->estado_passado = jogador->estado_atual;
                    jogador->estado_atual = VMM_PLAYER_ATAQUE2;
                } else {
                    if (jogador->estado_passado != VMM_PLAYER_IDLE)
                        jogador->frame = 0;
                    jogador->estado_passado = jogador->estado_atual;
                    jogador->estado_atual = VMM_PLAYER_IDLE;
                }
            }
        }
    }

    // Posição da imagem
    jogador->retangulo_img.y = jogador->retangulo_coli.y + jogador->retangulo_coli.h - jogador->retangulo_img.h - camera->y;
    jogador->retangulo_img.x = jogador->retangulo_coli.x - (jogador->retangulo_img.w * (44.0/MedidaImgPlayerX)) - (jogador->costas ? (jogador->retangulo_img.w * (11.0/MedidaImgPlayerX)) : 0) - camera->x;
}

void DesenharPlayer(PlayerInJogo jogador, Camera camera)
{
    // Desenha o retângulo de colisão (debug)
    // al_draw_rectangle(jogador.retangulo_coli.x - camera.x, ...);

    int frame_atual = (int)(jogador.frame / 10);
    VMM_Retangulo dst = {
        jogador.retangulo_img.x,
        jogador.retangulo_img.y,
        jogador.retangulo_img.w,
        jogador.retangulo_img.h
    };

    #define X(index, quant) \
        case index: { \
            al_draw_scaled_bitmap(jogador.sprite_atlas, \
                MedidaImgPlayerX * (frame_atual % quant), MedidaImgPlayerY * index, \
                MedidaImgPlayerX, MedidaImgPlayerY, \
                dst.x, dst.y, dst.w, dst.h, \
                jogador.costas ? ALLEGRO_FLIP_HORIZONTAL : 0); \
        } break;

    switch (jogador.estado_atual) {
        TabelaPlayerAnim
    }
    #undef X
}

// -------------------------------------------------------------
// Inimigo
// -------------------------------------------------------------
Inimigo InitInimigo(VMM_Retangulo retangulo_img, VMM_Retangulo retangulo_area, VMM_Retangulo retangulo_coli, float vida, float dano, int index)
{
    Inimigo inimigo = {
        .index = index,
        .estado_atual = 0,
        .estado_passado = 0,
        .costas = 0,
        .coli_h = 0,
        .coli_v = 0,
        .vida = vida,
        .dano = dano,
        .frame = 0,
        .tempo_safe = 0,
        .acelera = retangulo_coli.w / 100.0,
        .vel_max_x = retangulo_coli.w / 48.0,
        .vel_max_y = retangulo_coli.h / 48.0,
        .velocidade_x = 0,
        .velocidade_y = 0,
        .retangulo_area = retangulo_area,
        .posicao_x = retangulo_coli.x,
        .posicao_y = retangulo_coli.y,
        .retangulo_coli = retangulo_coli,
        .retangulo_coli_h = retangulo_coli,
        .retangulo_coli_v = retangulo_coli,
        .retangulo_img = retangulo_img,
    };
    return inimigo;
}

void CalcularInimigo(Inimigo *inimigo, double  delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2])
{
    // Se estiver fora da tela, não calcula
    if (inimigo->retangulo_coli.x < camera->x - tamanhos_tela[0] ||
        inimigo->retangulo_coli.x > camera->x + tamanhos_tela[0] * 2 ||
        inimigo->retangulo_coli.y < camera->y - tamanhos_tela[1] ||
        inimigo->retangulo_coli.y > camera->y + tamanhos_tela[1] * 2)
        return;

    double movi_v = 1, movi_h = 0;
    inimigo->frame += delta_frame;

    // Movimento horizontal (patrulha)
    if (!inimigo->tempo_safe)
        movi_h = (inimigo->costas ? -1 : 1);

    if (inimigo->retangulo_coli.x > inimigo->retangulo_area.x + inimigo->retangulo_area.w)
        inimigo->costas = true;
    if (inimigo->retangulo_coli.x < inimigo->retangulo_area.x)
        inimigo->costas = false;

    // Aplica gravidade e colisão vertical (usando a mesma função genérica)
    inimigo->velocidade_y += inimigo->acelera * delta_frame * movi_v;
    inimigo->velocidade_y = VMM_Clamp(inimigo->velocidade_y, -inimigo->vel_max_y, inimigo->vel_max_y);
    double velocidade = inimigo->velocidade_y * delta_frame;

    inimigo->posicao_y += velocidade;
    inimigo->retangulo_coli.y = inimigo->posicao_y;
    inimigo->coli_v = ColisaoComMapa(&inimigo->retangulo_coli, mapa, tamanho_bloco, tamanhos_tela, *camera);
    if (inimigo->coli_v) {
        inimigo->velocidade_y = 0;
        inimigo->posicao_y = inimigo->retangulo_coli.y; // ajuste
    }

    // Movimento horizontal com colisão
    inimigo->velocidade_x += inimigo->acelera * delta_frame * movi_h;
    inimigo->velocidade_x = VMM_Clamp(inimigo->velocidade_x, -inimigo->vel_max_x, inimigo->vel_max_x);
    velocidade = inimigo->velocidade_x * delta_frame;
    inimigo->posicao_x += velocidade;
    inimigo->retangulo_coli.x = inimigo->posicao_x;
    inimigo->coli_h = ColisaoComMapa(&inimigo->retangulo_coli, mapa, tamanho_bloco, tamanhos_tela, *camera);
    if (inimigo->coli_h) {
        inimigo->costas = !inimigo->costas;
        inimigo->velocidade_x = 0;
        inimigo->posicao_x = inimigo->retangulo_coli.x;
    }

    // Tempo de invulnerabilidade
    if (inimigo->tempo_safe) {
        if (inimigo->tempo_safe > delta_frame)
            inimigo->tempo_safe -= delta_frame;
        else
            inimigo->tempo_safe = 0;
    }

    // Animação (simples)
    if (movi_h)
        inimigo->estado_atual = VMM_PORCO_ANDAR;
    else
        inimigo->estado_atual = VMM_PORCO_IDLE;

    // Posição da imagem
    inimigo->retangulo_img.x = inimigo->posicao_x;
    inimigo->retangulo_img.y = inimigo->posicao_y;
}

void DesenharInimigo(Inimigo inimigo, ALLEGRO_BITMAP *sprite_atlas, Camera camera)
{
    int frame_atual = (int)(inimigo.frame / 10);
    VMM_Retangulo dst = {
        inimigo.retangulo_img.x - camera.x,
        inimigo.retangulo_img.y - camera.y,
        inimigo.retangulo_img.w,
        inimigo.retangulo_img.h
    };

    #define X(index, quant) \
        case index: { \
            al_draw_scaled_bitmap(sprite_atlas, \
                MedidaImgInimigo1X * (frame_atual % quant), MedidaImgInimigo1Y * index, \
                MedidaImgInimigo1X, MedidaImgInimigo1Y, \
                dst.x, dst.y, dst.w, dst.h, \
                inimigo.costas ? 0 : ALLEGRO_FLIP_HORIZONTAL); \
        } break;

    if (inimigo.index == PORCO_NORMAL) {
        switch (inimigo.estado_atual) {
            TabelaPorcoAnim
        }
    }
    #undef X
}

// -------------------------------------------------------------
// Colisões unificadas
// -------------------------------------------------------------
bool ColisaoComMapa(VMM_Retangulo *retangulo, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2], Camera camera)
{
   for(int i = camera.y/tamanho_bloco[1]; i*tamanho_bloco[1] < tamanhos_tela[1] + camera.y && i < TamanhosMapaY; i++){
        for(int j = camera.x/tamanho_bloco[0]; j*tamanho_bloco[0] < tamanhos_tela[0] + camera.x && j < TamanhosMapaX; j++) {
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
                                    if(ColisaoRetangulo(bloco, *retangulo))
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
// Colisão Player-Inimigo
// -------------------------------------------------------------
void ColisaoPlayerInimigo(PlayerInJogo *jogador, Inimigo *inimigo)
{
    // Verifica se o ataque do jogador acertou o inimigo
    if (jogador->retangulo_dano.w > 0) {
        if (jogador->retangulo_dano.x < inimigo->retangulo_coli.x + inimigo->retangulo_coli.w &&
            jogador->retangulo_dano.x + jogador->retangulo_dano.w > inimigo->retangulo_coli.x &&
            jogador->retangulo_dano.y < inimigo->retangulo_coli.y + inimigo->retangulo_coli.h &&
            jogador->retangulo_dano.y + jogador->retangulo_dano.h > inimigo->retangulo_coli.y) {
            if (!inimigo->tempo_safe) {
                inimigo->tempo_safe = 40;
                inimigo->vida -= jogador->dano;
            }
        }
    }

    // Colisão do corpo do jogador com o inimigo (dano ao jogador)
    if (jogador->retangulo_coli.x < inimigo->retangulo_coli.x + inimigo->retangulo_coli.w &&
        jogador->retangulo_coli.x + jogador->retangulo_coli.w > inimigo->retangulo_coli.x &&
        jogador->retangulo_coli.y < inimigo->retangulo_coli.y + inimigo->retangulo_coli.h &&
        jogador->retangulo_coli.y + jogador->retangulo_coli.h > inimigo->retangulo_coli.y) {
        if (!jogador->tempo_safe) {
            jogador->tempo_hit = 50;
            jogador->tempo_safe = 100;
            jogador->dano_sofrido = inimigo->dano;
        }
    }
}

// -------------------------------------------------------------
// Persistência do mapa
// -------------------------------------------------------------
void SalvarMapa(Mapa *c)
{
    char nome[64];
    sprintf(nome, "../../map/map_%d.bin", c->n);
    FILE *f = fopen(nome, "wb");
    if (!f) {
        printf("Erro ao salvar mapa %d\n", c->n);
        return;
    }
    fwrite(c->tiles, sizeof(uint16_t), TamanhosMapaX * TamanhosMapaY, f);
    fclose(f);
    printf("Mapa %d salvo.\n", c->n);
}

void CarregarMapa(Mapa *c, int n)
{
    c->n = n;
    char nome[64];
    sprintf(nome, "../../map/map_%d.bin", c->n);
    FILE *f = fopen(nome, "rb");
    if (f) {
        if (fread(c->tiles, sizeof(uint16_t), TamanhosMapaX * TamanhosMapaY, f) != TamanhosMapaX * TamanhosMapaY) {
            printf("Erro ao ler mapa %d\n", c->n);
        }
        fclose(f);
        printf("Mapa %d carregado.\n", n);
    } else {
        printf("Mapa %d não encontrado, gerando novo.\n", n);
        memset(c->tiles, 0, sizeof(c->tiles));
        SalvarMapa(c);
    }
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
    VMM_Retangulo coli = {0, geral->tamanhos.bloco1[0]*10, geral->tamanhos.jogador_coli[0], geral->tamanhos.jogador_coli[1]};
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
    //DesenharHud(geral, jogo);

    // Área de vitória (debug)
    //al_draw_filled_rectangle(
    //    jogo.mapa.area_vitoria.x - jogo.camera.x,
    //    jogo.mapa.area_vitoria.y - jogo.camera.y,
    //    jogo.mapa.area_vitoria.x - jogo.camera.x + jogo.mapa.area_vitoria.w,
    //    jogo.mapa.area_vitoria.y - jogo.camera.y + jogo.mapa.area_vitoria.h,
    //    al_map_rgba(0, 255, 0, 100));
}