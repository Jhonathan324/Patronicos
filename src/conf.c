#include "../hdr/geral.h"
#include "../hdr/ui_cenas.h"
#include <stdio.h>

void InitCenaConf(VariveisGerais *geral, VariveisConf *conf) {
    conf->fonte = al_load_ttf_font("../../fontes/arial.ttf", geral->tamanhos.tela[1] / 10, 0);

    //if (conf->imagem) al_destroy_bitmap(conf->imagem);
    conf->imagem = al_load_bitmap("../../sprites/ui/backgrounds/conf.png");

    VMM_Retangulo rect_moldura = {0, 0, geral->tamanhos.menu[0], geral->tamanhos.menu[1]};
    VMM_Retangulo rect_janela = {0, 0, geral->tamanhos.tela[0], geral->tamanhos.tela[1]};
    CentralizarRectInRect(&rect_janela, &rect_moldura);

    DestruirMoldura(&conf->moldura);
    conf->moldura = InitMoldura(&rect_moldura, "../../sprites/ui/panels/moldura de madeira.png");

    DestruirTexto(&conf->texto_reso);
    DestruirTexto(&conf->texto_full);

    conf->texto_reso = InitTexto(
        &(VMM_Retangulo){conf->moldura.retangulo.x + CantoFixo - 10,
                         conf->moldura.retangulo.y + CantoFixo,
                         geral->tamanhos.botao1[0],
                         geral->tamanhos.botao1[1] / 2},
        al_map_rgba(0,0,0,0),
        "Resolução",
        NULL,
        conf->fonte,
        al_map_rgb(0,0,0),
        false
    );

    conf->texto_full = InitTexto(
        &(VMM_Retangulo){0,
                         conf->moldura.retangulo.y + CantoFixo,
                         geral->tamanhos.botao1[0],
                         geral->tamanhos.botao1[1] / 2},
        al_map_rgba(0,0,0,0),
        "Tela Cheia",
        NULL,
        conf->fonte,
        al_map_rgb(0,0,0),
        false
    );
    conf->texto_full.retangulo.x = conf->moldura.retangulo.x + conf->moldura.retangulo.w - conf->texto_full.retangulo.w - CantoFixo / 2;

    DestruirBotao(&conf->botao_sair);
    conf->botao_sair = InitBotao(
        &(VMM_Retangulo){conf->moldura.retangulo.x + conf->moldura.retangulo.w - CantoFixo / 2,
                         conf->moldura.retangulo.y + conf->moldura.retangulo.h - geral->tamanhos.botao1[1] - CantoFixo / 2,
                         geral->tamanhos.botao1[0],
                         geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        "Voltar",
        al_map_rgb(70,70,70),
        al_map_rgb(30,30,30),
        CENA_MENU,
        conf->fonte,
        al_map_rgb(0,0,0)
    );
    conf->botao_sair.retangulo.x -= conf->botao_sair.retangulo.w;

    DestruirMarcador(&conf->troca_fullscreen);
    conf->troca_fullscreen = InitMarcador(
        &(VMM_Retangulo){conf->texto_full.retangulo.x + CantoFixo * (2.0f/3.0f),
                         conf->texto_full.retangulo.y + conf->texto_full.retangulo.h,
                         geral->tamanhos.botao1[1],
                         geral->tamanhos.botao1[1]},
        geral->fullscrean,
        "../../sprites/ui/buttons/marcador.png",
        al_map_rgb(0,0,0),
        al_map_rgb(255,255,255)
    );

    char *resolucao_atual = (char*) calloc(32,sizeof(char));
    sprintf(resolucao_atual, "(%d, %d)", geral->resolucao_atual[0], geral->resolucao_atual[1]);
    char *textos[] = {
        "640, 360",
        "960, 540",
        "1280, 720",
        "1600, 900",
        "1920, 1080",
        "2240, 1260",
        "2560, 1440"
    };

    DestruirBotaoExpansivo(&conf->botao_reso);
    conf->botao_reso = InitBotaoExpansivo(
        &(VMM_Retangulo){conf->moldura.retangulo.x + CantoFixo,
                         conf->texto_reso.retangulo.y + conf->texto_reso.retangulo.h,
                         geral->tamanhos.botao1[0],
                         geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        resolucao_atual,
        textos,
        al_map_rgb(255,255,255),
        al_map_rgba(0,0,0,128),
        CENA_CONF,
        conf->fonte,
        al_map_rgb(0,0,0),
        7
    );
}

void LoopCenaConf(VariveisGerais *geral, VariveisConf *conf) {
    ALLEGRO_MOUSE_STATE mouse_state;      // <-- local
    al_get_mouse_state(&mouse_state);
    geral->ponto_mouse.x = mouse_state.x;
    geral->ponto_mouse.y = mouse_state.y;

    if (VerificarBotao(&conf->botao_sair, geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
        int temp = geral->cena;
        geral->cena = geral->cena_passada;
        geral->cena_passada = temp;
        if (conf->reso_inicial != geral->tamanhos.escala || conf->valida_fullscrean != geral->fullscrean) {
            conf->valida_fullscrean = geral->fullscrean;
            conf->reso_inicial = geral->tamanhos.escala;
            geral->troca_reso = true;
        }
    }

    if (VerificarMarcador(&conf->troca_fullscreen, geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
        geral->fullscrean = !geral->fullscrean;
        CalcularGeral(geral);
        InitCenaConf(geral, conf);
    }

    if (VerificarBotao(&conf->botao_reso.botao_pai, geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
        conf->botao_reso.expandido = !conf->botao_reso.expandido;
    }

    if (conf->botao_reso.expandido) {
        for (int i = 0; i < 7; i++) {
            if (VerificarBotao(&conf->botao_reso.botao_filho[i], geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
                if (i == geral->tamanhos.escala) {
                    conf->botao_reso.expandido = false;
                } else {
                    geral->tamanhos.escala = i;
                    CalcularGeral(geral);
                    InitCenaConf(geral, conf);
                }
            }
        }
    }
}

void DesenharCenaConf(VariveisGerais geral, VariveisConf conf) {
    DesenharMoldura(conf.moldura);
    DesenharTexto(conf.texto_reso);
    DesenharTexto(conf.texto_full);
    DesenharBotao(conf.fonte, conf.botao_sair, NULL);      // <-- fonte
    DesenharBotaoExpansivo(conf.fonte, conf.botao_reso, NULL);
    DesenharMarcador(conf.troca_fullscreen);
}