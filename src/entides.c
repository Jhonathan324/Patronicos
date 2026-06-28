#include "../hdr/jogo.h"

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
        .retangulo_coli = retangulo_coli,
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
    delta_frame /= 10;
    // Se vida <= 0, perde coração e volta ao spawn
    if (jogador->vida <= 0) {
        jogador->vida = 100;
        jogador->coracoes--;
        jogador->dano_sofrido = 0;
        redimencionar_jogador(jogador, (VMM_Ponto){tamanho_bloco[0], 62 * tamanho_bloco[1]});
    }

    jogador->frame += delta_frame;

    // Movimento horizontal
    if(al_key_down(teclado, ALLEGRO_KEY_D) || al_key_down(teclado, ALLEGRO_KEY_RIGHT) )
        movi_h += 1.0;
    if(al_key_down(teclado, ALLEGRO_KEY_A) || al_key_down(teclado, ALLEGRO_KEY_LEFT) )
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
    jogador->velocidade_y -= jogador->acelera  * (jogador->pulo / 6) * delta_frame;
    jogador->velocidade_y += jogador->acelera  * movi_v * delta_frame;
    jogador->velocidade_y = VMM_Clamp(jogador->velocidade_y, -jogador->vel_max_y, jogador->vel_max_y);
    double velocidade     = VMM_Clamp(jogador->velocidade_y * delta_frame, -jogador->vel_max_y, jogador->vel_max_y);

    // Colisão vertical
    jogador->posicao_y_back = jogador->posicao_y;
    jogador->posicao_y += velocidade;
    jogador->retangulo_coli.y = jogador->posicao_y;
    jogador->coli_v = ColisaoComMapa(&jogador->retangulo_coli, mapa, tamanho_bloco, tamanhos_tela, *camera);

    if (jogador->coli_v) {
        if (jogador->pulo) {
        jogador->pulo = 0;
        jogador->coli_v = false;
        } 
        else{
            jogador->coyote_time = 0;
            if ((al_key_down(teclado, ALLEGRO_KEY_SPACE)||al_key_down(teclado, ALLEGRO_KEY_W)|| al_key_down(teclado, ALLEGRO_KEY_UP)) && !jogador->pulo) {
                jogador->pulo = 50;
            }
        }
        movi_v = 0;
        jogador->velocidade_y = 0;
        jogador->posicao_y = jogador->posicao_y_back;
        jogador->retangulo_coli.y = jogador->posicao_y_back;
        
    }
    if (jogador->estado_atual != VMM_PLAYER_CAIR && jogador->posicao_y != jogador->posicao_y_back)
        jogador->coyote_time++;

    // Colisão horizontal
    jogador->velocidade_x += jogador->acelera * movi_h* delta_frame;
    jogador->velocidade_x = VMM_Clamp(jogador->velocidade_x, -jogador->vel_max_x, jogador->vel_max_x);
    velocidade =            VMM_Clamp(jogador->velocidade_x * delta_frame, -jogador->vel_max_x, jogador->vel_max_x);

    jogador->posicao_x_back = jogador->posicao_x;
    jogador->posicao_x += velocidade;
    jogador->retangulo_coli.x = jogador->posicao_x;
    jogador->coli_h = ColisaoComMapa(&jogador->retangulo_coli, mapa, tamanho_bloco, tamanhos_tela, *camera);
    if (jogador->coli_h) {
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
    
    al_draw_filled_rectangle(
        jogador.retangulo_coli.x - camera.x,
        jogador.retangulo_coli.y - camera.y,
        jogador.retangulo_coli.x+jogador.retangulo_coli.w - camera.x,
        jogador.retangulo_coli.y+jogador.retangulo_coli.h - camera.y,
        al_map_rgb(50,50,50)
    );
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