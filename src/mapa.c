#include "../hdr/mapa.h"

// Funções auxiliares
bool VerificarMarcadorBloco(MarcadorBloco *marcador, VMM_Ponto mouse, int rolada) {
    VMM_Retangulo r = marcador->retangulo;
    r.y += rolada;
    if (mouse.x >= r.x && mouse.x <= r.x + r.w &&
        mouse.y >= r.y && mouse.y <= r.y + r.h) {
        marcador->sobre = true;
        return true;
    }
    marcador->sobre = false;
    return false;
}

void DesenharMarcadorBloco(MarcadorBloco marcador, int index, ALLEGRO_BITMAP *textura, int rolada) {
    VMM_Retangulo src = MapaTiles(index);
    VMM_Retangulo dst = marcador.retangulo;
    dst.y += rolada;
    al_draw_scaled_bitmap(textura, src.x, src.y, src.w, src.h,
                          dst.x, dst.y, dst.w, dst.h, 0);
}

void InitCenaMapa(VariveisGerais *geral, VariaveisMapa *mapa) {
    geral->fonte = al_load_ttf_font("../../fontes/arial.ttf", geral->tamanhos.tela[1] / 10, 0);

    mapa->mapa.n = 0;
    CarregarMapa(&mapa->mapa, 0);
    if (mapa->mapa.textura) al_destroy_bitmap(mapa->mapa.textura);
    mapa->mapa.textura = al_load_bitmap("../../sprites/world/tiles/Tiles.png");

    mapa->camera.x = geral->tamanhos.bloco1[0];
    mapa->camera.y = (TamanhosMapaY/2) * geral->tamanhos.bloco1[1];
    mapa->rolada = 0;
    mapa->velocidade = 1;
    mapa->tamanho_bloco[0] = geral->tamanhos.bloco1[0];
    mapa->tamanho_bloco[1] = geral->tamanhos.bloco1[1];
    mapa->selecao.w = geral->tamanhos.bloco1[0];
    mapa->selecao.h = geral->tamanhos.bloco1[1];
    mapa->bloco_uso = 1;

    VMM_Retangulo rect_moldura = {
        geral->tamanhos.tela[0] - geral->tamanhos.bloco1[0]*2 - 30,
        0,
        geral->tamanhos.bloco1[0]*2 + 30,
        geral->tamanhos.tela[1]
    };
    DestruirMoldura(&mapa->moldura_bloco);
    mapa->moldura_bloco = InitMoldura(&rect_moldura, "../../sprites/ui/panels/moldura de madeira.png");

    DestruirBotao(&mapa->botao_salvar);
    mapa->botao_salvar = InitBotao(
        &(VMM_Retangulo){ geral->tamanhos.tela[0] - CantoFixo*4, geral->tamanhos.tela[1] - geral->tamanhos.botao1[1] - CantoFixo, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1] },
        "../../sprites/ui/buttons/botão.png",
        "Salvar",
        al_map_rgb(70,70,70),
        al_map_rgb(30,30,30),
        CENA_MENU,
        geral->fonte,
        al_map_rgb(0,0,0)
    );
    mapa->botao_salvar.retangulo.x -= mapa->botao_salvar.retangulo.w;

    DestruirBotao(&mapa->botao_carregar);
    mapa->botao_carregar = InitBotao(
        &(VMM_Retangulo){ geral->tamanhos.tela[0] - CantoFixo*4, geral->tamanhos.tela[1] - geral->tamanhos.botao1[1] - CantoFixo, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1] },
        "../../sprites/ui/buttons/botão.png",
        "Carregar",
        al_map_rgb(70,70,70),
        al_map_rgb(30,30,30),
        CENA_MENU,
        geral->fonte,
        al_map_rgb(0,0,0)
    );
    mapa->botao_carregar.retangulo.x -= mapa->botao_carregar.retangulo.w * 2;

    mapa->marcador_preencher = InitMarcador(
        &(VMM_Retangulo){ mapa->botao_carregar.retangulo.x - CantoFixo - geral->tamanhos.botao1[0], mapa->botao_carregar.retangulo.y, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1] },
        false,
        "../../sprites/ui/buttons/marcador.png",
        al_map_rgba(0,0,0,128),
        al_map_rgba(0,0,0,128)
    );

    #define X(index, x_loc, y_loc, tipo) \
        mapa->marcador[index] = (MarcadorBloco){ \
            .ativo = false, \
            .cor1 = al_map_rgb(0,0,0), \
            .cor2 = al_map_rgb(255,255,255), \
            .retangulo = { \
                geral->tamanhos.tela[0] - geral->tamanhos.bloco1[0] - 13, \
                geral->tamanhos.bloco1[1] * (index/2), \
                geral->tamanhos.bloco1[0], \
                geral->tamanhos.bloco1[1] \
            } \
        }; \
        mapa->marcador[index].retangulo.x += (!(index%2)) ? -geral->tamanhos.bloco1[0]-2 : 0;
    TabelaBlocoAtlas
    #undef X
}

void LoopCenaMapa(VariveisGerais *geral, VariaveisMapa *mapa) {
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    geral->ponto_mouse.x = mouse_state.x;
    geral->ponto_mouse.y = mouse_state.y;

    // Teclado
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state);
    if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) {
        geral->cena_continuar = CENA_CRIACAO;
        geral->cena_passada = CENA_CRIACAO;
        geral->cena = CENA_PAUSE;
    }

    int aumentar = (al_key_down(&key_state, ALLEGRO_KEY_UP) ? 1 : 0) - (al_key_down(&key_state, ALLEGRO_KEY_DOWN) ? 1 : 0);
    mapa->velocidade += aumentar;
    int movimento_h = (al_key_down(&key_state, ALLEGRO_KEY_D) ? 1 : 0) - (al_key_down(&key_state, ALLEGRO_KEY_A) ? 1 : 0);
    int movimento_v = (al_key_down(&key_state, ALLEGRO_KEY_S) ? 1 : 0) - (al_key_down(&key_state, ALLEGRO_KEY_W) ? 1 : 0);
    mapa->camera.x += movimento_h * mapa->velocidade;
    mapa->camera.y += movimento_v * mapa->velocidade;

    mapa->selecao.x = ((int)(geral->ponto_mouse.x + mapa->camera.x) / mapa->tamanho_bloco[0]) * mapa->tamanho_bloco[0] - mapa->camera.x;
    mapa->selecao.y = ((int)(geral->ponto_mouse.y + mapa->camera.y) / mapa->tamanho_bloco[1]) * mapa->tamanho_bloco[1] - mapa->camera.y;
    mapa->selecao_coli.x = (geral->ponto_mouse.x + mapa->camera.x) / mapa->tamanho_bloco[0];
    mapa->selecao_coli.y = (geral->ponto_mouse.y + mapa->camera.y) / mapa->tamanho_bloco[1];

    if (VerificarBotao(&mapa->botao_salvar, geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
        SalvarMapa(&mapa->mapa);
        geral->carregar_mapa = true;
    }
    if (VerificarBotao(&mapa->botao_carregar, geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
        CarregarMapa(&mapa->mapa, mapa->mapa.n);
    }
    if (VerificarMarcador(&mapa->marcador_preencher, geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
        // ativa preenchimento
    }

    if (geral->ponto_mouse.x > mapa->moldura_bloco.retangulo.x) {
        mapa->rolada += geral->botao_mouse_gira * 64;
        geral->botao_mouse_gira = 0;
    }

    if (al_key_down(&key_state, ALLEGRO_KEY_LCTRL)) {
        mapa->camera.x = mapa->camera.x / mapa->tamanho_bloco[0];
        mapa->camera.y = mapa->camera.y / mapa->tamanho_bloco[1];
        mapa->tamanho_bloco[0] += geral->botao_mouse_gira * 2;
        mapa->tamanho_bloco[1] += geral->botao_mouse_gira * 2;
        mapa->selecao.w = mapa->tamanho_bloco[0];
        mapa->selecao.h = mapa->tamanho_bloco[1];
        mapa->camera.x = mapa->camera.x * mapa->tamanho_bloco[0];
        mapa->camera.y = mapa->camera.y * mapa->tamanho_bloco[1];
        geral->botao_mouse_gira = 0;
    } else if (al_key_down(&key_state, ALLEGRO_KEY_LSHIFT)) {
        mapa->camera.x -= geral->botao_mouse_gira * mapa->velocidade;
        geral->botao_mouse_gira = 0;
    } else {
        mapa->camera.y -= geral->botao_mouse_gira * mapa->velocidade;
        geral->botao_mouse_gira = 0;
    }

    if (geral->botao_mouse_esquerdo && !mapa->botao_salvar.sobre && geral->ponto_mouse.x > mapa->moldura_bloco.retangulo.x) {
        #define X(index, x_loc, y_loc, tipo) \
            if (VerificarMarcadorBloco(&mapa->marcador[index], geral->ponto_mouse, mapa->rolada)) { \
                mapa->bloco_uso = index + 1; \
            }
        TabelaBlocoAtlas
        #undef X
    } else if (geral->botao_mouse_esquerdo && !mapa->botao_salvar.sobre && !mapa->marcador_preencher.sobre) {
        if (mapa->marcador_preencher.ativo) {
            mapa->selecao_coli_preencimento.x = mapa->selecao_coli.x;
            mapa->selecao_coli_preencimento.y = mapa->selecao_coli.y;
            mapa->preencher = true;
        } else {
            mapa->mapa.tiles[(int)mapa->selecao_coli.y][(int)mapa->selecao_coli.x] = mapa->bloco_uso;
        }
    }
    if (geral->botao_mouse_direito) {
        mapa->mapa.tiles[(int)mapa->selecao_coli.y][(int)mapa->selecao_coli.x] = 0;
    }

    if (!geral->botao_mouse_esquerdo && mapa->preencher) {
        int mod_x = mapa->selecao_coli_preencimento.x - mapa->selecao_coli_back.x;
        int mod_y = mapa->selecao_coli_preencimento.y - mapa->selecao_coli_back.y;
        if (mod_y < 0) {
            for (int i = mapa->selecao_coli_back.y; i >= mapa->selecao_coli_back.y + mod_y; i--) {
                if (mod_x < 0) {
                    for (int j = mapa->selecao_coli_back.x; j >= mapa->selecao_coli_back.x + mod_x; j--)
                        mapa->mapa.tiles[i][j] = mapa->bloco_uso;
                } else {
                    for (int j = mapa->selecao_coli_back.x; j <= mapa->selecao_coli_back.x + mod_x; j++)
                        mapa->mapa.tiles[i][j] = mapa->bloco_uso;
                }
            }
        } else {
            for (int i = mapa->selecao_coli_back.y; i <= mapa->selecao_coli_back.y + mod_y; i++) {
                if (mod_x < 0) {
                    for (int j = mapa->selecao_coli_back.x; j >= mapa->selecao_coli_back.x + mod_x; j--)
                        mapa->mapa.tiles[i][j] = mapa->bloco_uso;
                } else {
                    for (int j = mapa->selecao_coli_back.x; j <= mapa->selecao_coli_back.x + mod_x; j++)
                        mapa->mapa.tiles[i][j] = mapa->bloco_uso;
                }
            }
        }
        mapa->preencher = false;
    }

    // CORREÇÃO AQUI: atribuir campos individualmente
    mapa->selecao_coli_back.x = mapa->selecao_coli.x;
    mapa->selecao_coli_back.y = mapa->selecao_coli.y;

    if (geral->botao_mouse_meio) {
        mapa->camera.x = mapa->camera_back.x + geral->ponto_mouse_back.x - geral->ponto_mouse.x;
        mapa->camera.y = mapa->camera_back.y + geral->ponto_mouse_back.y - geral->ponto_mouse.y;
    } else {
        mapa->camera_back = mapa->camera;
        geral->ponto_mouse_back = geral->ponto_mouse;
    }
}

void DesenharCenaMapa(VariveisGerais geral, VariaveisMapa mapa) {
    al_clear_to_color(al_map_rgb(mapa.cor_fundo.r, mapa.cor_fundo.g, mapa.cor_fundo.b));

    DesenharMapa(geral, mapa.mapa, mapa.camera, mapa.tamanho_bloco);

    al_draw_filled_rectangle(mapa.selecao.x, mapa.selecao.y,
                             mapa.selecao.x + mapa.selecao.w, mapa.selecao.y + mapa.selecao.h,
                             al_map_rgba(0,0,0,128));

    DesenharMoldura(mapa.moldura_bloco);
    #define X(index, x_loc, y_loc, tipo) \
        DesenharMarcadorBloco(mapa.marcador[index], index+1, mapa.mapa.textura, mapa.rolada);
    TabelaBlocoAtlas
    #undef X

    DesenharBotao(geral.fonte, mapa.botao_salvar);   // <-- fonte como primeiro argumento
    DesenharBotao(geral.fonte, mapa.botao_carregar);
    DesenharMarcador(mapa.marcador_preencher);
}
