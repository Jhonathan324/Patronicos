#include "gerais.h"

#define QuantBotao 4

void InitCenaMenu(VariveisGerais *geral, VariveisMenu *menu)
{
    // fonte
    menu->fonte = al_load_ttf_font("../../fontes/arial.ttf", geral->tamanhos.botao1[1], 0);

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
    menu->moldura = InitMoldura(&rect_moldura, "../../sprites/ui/panels/moldura de madeira.png");

    // Criação dos botões
    DestruirBotao(&menu->botao_conf);
    DestruirBotao(&menu->botao_criacao);
    DestruirBotao(&menu->botao_iniciar);
    DestruirBotao(&menu->botao_sair);
    menu->botao_iniciar =
        InitBotao(
                  &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]}, // retangulo base
                  "../../sprites/ui/buttons/botão.png",
                  "Iniciar Jogo",
                  al_map_rgb(70, 70, 70),
                  al_map_rgb SEMI_PRETO,
                  CENA_JOGO,
                  menu->fonte,
                  al_map_rgb PRETO);

    menu->botao_criacao =
        InitBotao(
                  &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]}, // retangulo base
                  "../../sprites/ui/buttons/botão.png",
                  "Criar Mapas",
                  al_map_rgb(70, 70, 70),
                  al_map_rgb SEMI_PRETO,
                  CENA_CRIACAO,
                  menu->fonte,
                  al_map_rgb PRETO);

    menu->botao_conf =
        InitBotao(
                  &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]}, // retangulo base
                  "../../sprites/ui/buttons/botão.png",
                  "Configuracoes",
                  al_map_rgb(70, 70, 70),
                  al_map_rgb SEMI_PRETO,
                  CENA_CONF,
                  menu->fonte,
                  al_map_rgb PRETO);

    menu->botao_sair =
        InitBotao(
                  &(VMM_Retangulo){0, 0, geral->tamanhos.botao1[0], geral->tamanhos.botao1[1]}, // retangulo base
                  "../../sprites/ui/buttons/botão.png",
                  "Sair do Jogo",
                  al_map_rgb(70, 70, 70),
                  al_map_rgb SEMI_PRETO,
                  CENA_SAIR,
                  menu->fonte,
                  al_map_rgb PRETO);

    // Necessario para alinhar os botões de forma mais pratica
    VMM_Retangulo *retangulos[] = {
        &menu->botao_iniciar.retangulo,
        &menu->botao_criacao.retangulo,
        &menu->botao_conf.retangulo,
        &menu->botao_sair.retangulo};
    CentralizarRectsInRectV(&menu->moldura.retangulo, retangulos, QuantBotao, 0.1, 0.2);

    // Calculo das partes dos botões para as imagens

}

void DesenharCenaMenu(VariveisGerais geral, VariveisMenu menu){
    Botao *botoes[] = {
        &menu.botao_iniciar,
        &menu.botao_criacao,
        &menu.botao_conf,
        &menu.botao_sair};

    // limpeza de tela
    al_clear_to_color(al_map_rgb(menu.cor_fundo.r, menu.cor_fundo.g, menu.cor_fundo.b));
    al_draw_scaled_bitmap(menu.imagem, 0, 0, 640, 360, 0, 0, geral.tamanhos.tela[0], geral.tamanhos.tela[1], 0);

    
    // botões
    // DesenharMoldura(geral.renderizador, menu.moldura);
    for (int i = 0; i < QuantBotao; i++){
        DesenharBotao(menu.fonte, *botoes[i]);
  
    }

   
}
