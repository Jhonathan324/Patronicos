#include "../hdr/geral.h"
#include "../hdr/ui_cenas.h"

#define QuantBotao 4

void InitCenaMenu(VariveisGerais *geral, VariveisMenu *menu)
{
    // Carrega a fonte
    menu->fonte = al_load_ttf_font("../../fontes/arial.ttf", geral->tamanhos.botao1[1], 0);
    if (!menu->fonte) {
        ErroFatal("Falha ao carregar fonte: ../../fontes/arial.ttf");
    }

    // Carrega imagem de fundo
    //if (menu->imagem) al_destroy_bitmap(menu->imagem);
    menu->imagem = al_load_bitmap("../../sprites/ui/background/menu Inicial.png");
    if (!menu->imagem) {
        ErroFatal("Falha ao carregar imagem de fundo: ../../sprites/ui/background/menu Inicial.png");
    }

    // Cria a moldura do menu
    VMM_Retangulo rect_moldura = {
        geral->tamanhos.menu[1] * 0.1,
        geral->resolucao_atual[1] - geral->tamanhos.menu[1] * 0.9,
        geral->tamanhos.menu[0],
        geral->tamanhos.menu[1]
    };
    DestruirMoldura(&menu->moldura);
    menu->moldura = InitMoldura(&rect_moldura, "../../sprites/ui/panels/moldura de madeira.png");

    // Cria os botões (destroi os antigos se existirem)
    DestruirBotao(&menu->botao_conf);
    DestruirBotao(&menu->botao_criacao);
    DestruirBotao(&menu->botao_iniciar);
    DestruirBotao(&menu->botao_sair);

    menu->botao_iniciar = InitBotao(
        &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        "Iniciar Jogo",
        al_map_rgb(70, 70, 70),
        al_map_rgb SEMI_PRETO,
        CENA_JOGO,
        menu->fonte,
        al_map_rgb PRETO
    );

    menu->botao_criacao = InitBotao(
        &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        "Criar Mapas",
        al_map_rgb(70, 70, 70),
        al_map_rgb SEMI_PRETO,
        CENA_CRIACAO,
        menu->fonte,
        al_map_rgb PRETO
    );

    menu->botao_conf = InitBotao(
        &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        "Configuracoes",
        al_map_rgb(70, 70, 70),
        al_map_rgb SEMI_PRETO,
        CENA_CONF,
        menu->fonte,
        al_map_rgb PRETO
    );

    menu->botao_sair = InitBotao(
        &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]},
        "../../sprites/ui/buttons/botão.png",
        "Sair do Jogo",
        al_map_rgb(70, 70, 70),
        al_map_rgb SEMI_PRETO,
        CENA_SAIR,
        menu->fonte,
        al_map_rgb PRETO
    );

    // Centraliza os botões verticalmente dentro da moldura
    VMM_Retangulo *retangulos[] = {
        &menu->botao_iniciar.retangulo,
        &menu->botao_criacao.retangulo,
        &menu->botao_conf.retangulo,
        &menu->botao_sair.retangulo
    };
    CentralizarRectsInRectV(&menu->moldura.retangulo, retangulos, QuantBotao, 0.1, 0.2);
}

void LoopCenaMenu(VariveisGerais *geral, VariveisMenu *menu)
{
    // Atualiza posição do mouse
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    geral->ponto_mouse.x = mouse_state.x;
    geral->ponto_mouse.y = mouse_state.y;

    // Lista de botões
    Botao *botoes[] = {
        &menu->botao_iniciar,
        &menu->botao_criacao,
        &menu->botao_conf,
        &menu->botao_sair
    };

    // Verifica clique em cada botão
    for (int i = 0; i < QuantBotao; i++) {
        if (VerificarBotao(botoes[i], geral->ponto_mouse, geral->botao_mouse_esquerdo)) {
            geral->cena_passada = geral->cena;
            geral->cena = botoes[i]->indice;
        }
    }
}

void DesenharCenaMenu(VariveisGerais geral, VariveisMenu menu)
{
    Botao *botoes[] = {
        &menu.botao_iniciar,
        &menu.botao_criacao,
        &menu.botao_conf,
        &menu.botao_sair
    };

    // Limpa a tela e desenha o fundo
    //al_clear_to_color(al_map_rgb(menu.cor_fundo.r, menu.cor_fundo.g, menu.cor_fundo.b));
    al_draw_scaled_bitmap(menu.imagem, 0, 0, 640, 360, 0, 0,
                          geral.tamanhos.tela[0], geral.tamanhos.tela[1], 0);

    // Desenha todos os botões
    for (int i = 0; i < QuantBotao; i++) {
        DesenharBotao(menu.fonte, *botoes[i], NULL);
    }
}