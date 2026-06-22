#include "gerais.h"

#define QuantBotao 4

void InitCenaMenu(VariveisGerais *geral, VariveisMenu *menu)
{
    // fonte
    ALLEGRO_FONT *fonte = al_load_ttf_font("fontes/arial.ttf", geral->tamanhos.botao1[1], NULL);

    // fundo
    if(menu->imagem) al_destroy_bitmap(menu->imagem);
    menu->imagem = al_load_bitmap("../../sprites/ui/background/menu Inicial.png");

    // Criação do menu para os botões
    VMM_Retangulo rect_moldura = {geral->tamanhos.menu[1]*0.1, geral->resolucao_atual[1]-geral->tamanhos.menu[1]*0.9, geral->tamanhos.menu[0], geral->tamanhos.menu[1]};

    // obtenção do rect da janela
    //int janela_w, janela_h;
    //SDL_GetWindowSize(geral->janela, &janela_w, &janela_h);
    //SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    //CentralizarRectInRect(&rect_janela, &rect_moldura); // centralização do menu com base na tela
    DestruirMoldura(&menu->moldura);
    menu->moldura = InitMoldura(&rect_moldura, "assets/imagens/ui/panels/moldura de madeira.png");

    // Criação dos botões
    DestruirBotao(&menu->botao_conf);
    DestruirBotao(&menu->botao_criacao);
    DestruirBotao(&menu->botao_iniciar);
    DestruirBotao(&menu->botao_sair);
    menu->botao_iniciar =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.botao1[0], tamanhos.botao1[1]}, // retangulo base
                  "assets/imagens/ui/buttons/botão.png",
                  "Iniciar Jogo",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color)SEMI_PRETO,
                  CENA_JOGO,
                  fonte,
                  (SDL_Color)PRETO);

    menu->botao_criacao =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.botao1[0], tamanhos.botao1[1]}, // retangulo base
                  "assets/imagens/ui/buttons/botão.png",
                  "Criar Mapas",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color)SEMI_PRETO,
                  CENA_CRIACAO,
                  fonte,
                  (SDL_Color)PRETO);

    menu->botao_conf =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.botao1[0], tamanhos.botao1[1]}, // retangulo base
                  "assets/imagens/ui/buttons/botão.png",
                  "Configuracoes",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color)SEMI_PRETO,
                  CENA_CONF,
                  fonte,
                  (SDL_Color)PRETO);

    menu->botao_sair =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.botao1[0], tamanhos.botao1[1]}, // retangulo base
                  "assets/imagens/ui/buttons/botão.png",
                  "Sair do Jogo",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color)SEMI_PRETO,
                  CENA_SAIR, fonte,
                  (SDL_Color)PRETO);

    // Necessario para alinhar os botões de forma mais pratica
    SDL_FRect *retangulos[] = {
        &menu->botao_iniciar.retangulo,
        &menu->botao_criacao.retangulo,
        &menu->botao_conf.retangulo,
        &menu->botao_sair.retangulo};
    CentralizarRectsInRectV(&menu->moldura.retangulo, retangulos, QuantBotao, 0.1, 0.2);

    // Calculo das partes dos botões para as imagens

}