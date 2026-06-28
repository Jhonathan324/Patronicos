#include "../hdr/geral.h"
#include "../hdr/ui_cenas.h"

void InitCenaPause(VariveisGerais *geral, VariveisPause *pause) {
    pause->fonte = al_load_ttf_font("../../fontes/arial.ttf", geral->tamanhos.tela[1] / 10, 0);

    if (pause->imagem) al_destroy_bitmap(pause->imagem);
    pause->imagem = al_load_bitmap("../../sprites/ui/backgrounds/pause.png");

    VMM_Retangulo rect_moldura = {0, 0, geral->tamanhos.menu[0], geral->tamanhos.menu[1]};
    VMM_Retangulo rect_janela = {0, 0, geral->tamanhos.tela[0], geral->tamanhos.tela[1]};
    CentralizarRectInRect(&rect_janela, &rect_moldura);

    DestruirMoldura(&pause->moldura);
    pause->moldura = InitMoldura(&rect_moldura, "../../sprites/ui/panels/moldura de madeira.png");

    DestruirBotao(&pause->botao_iniciar);
    DestruirBotao(&pause->botao_conf);
    DestruirBotao(&pause->botao_sair);

    pause->botao_iniciar = InitBotao(
        &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        "Continuar",
        al_map_rgb(70,70,70),
        al_map_rgba(0,0,0,128),
        geral->cena_continuar,
        pause->fonte,
        al_map_rgb(0,0,0)
    );

    pause->botao_conf = InitBotao(
        &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        "Configurações",
        al_map_rgb(70,70,70),
        al_map_rgba(0,0,0,128),
        CENA_CONF,
        pause->fonte,
        al_map_rgb(0,0,0)
    );

    pause->botao_sair = InitBotao(
        &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        "Menu Inicial",
        al_map_rgb(70,70,70),
        al_map_rgba(0,0,0,128),
        CENA_MENU,
        pause->fonte,
        al_map_rgb(0,0,0)
    );

    VMM_Retangulo *botoes[] = {
        &pause->botao_iniciar.retangulo,
        &pause->botao_conf.retangulo,
        &pause->botao_sair.retangulo
    };
    CentralizarRectsInRectV(&pause->moldura.retangulo, botoes, 3, 0.1, 0.2);
}

void LoopCenaPause(VariveisGerais *geral, VariveisPause *pause) {
    ALLEGRO_MOUSE_STATE mouse_state;      // <-- variável local
    al_get_mouse_state(&mouse_state);
    geral->ponto_mouse.x = mouse_state.x;
    geral->ponto_mouse.y = mouse_state.y;

    Botao *botoes[] = {
        &pause->botao_iniciar,
        &pause->botao_conf,
        &pause->botao_sair
    };

    if (geral->cena_passada != CENA_CONF && geral->cena_passada != CENA_MENU)
        botoes[0]->indice = geral->cena_passada;

    for (int i = 0; i < 3; i++) {
        if (VerificarBotao(botoes[i], geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
            geral->cena_passada = geral->cena;
            geral->cena = botoes[i]->indice;
        }
    }
}

void DesenharCenaPause(VariveisGerais geral, VariveisPause pause) {
    DesenharMoldura(pause.moldura);
    DesenharBotao(pause.fonte, pause.botao_iniciar, NULL);  // <-- fonte como primeiro argumento
    DesenharBotao(pause.fonte, pause.botao_conf, NULL);
    DesenharBotao(pause.fonte, pause.botao_sair, NULL);
}