#include "gerais.h"

void GetTamanhos(Tamanhos *tamanhos)
{
    if(tamanhos->escala >= 0 && tamanhos->escala <=6){
        tamanhos->tela[0] = 320 * (tamanhos->escala + 2);
        tamanhos->tela[1] = 180 * (tamanhos->escala + 2);
    }
    else{
        tamanhos->escala = 0;
        tamanhos->tela[0] = 640;
        tamanhos->tela[1] = 360;
    }

    tamanhos->menu[0]         = tamanhos->tela[0] * (float)CantoFixo           / 640 * 6;
    tamanhos->menu[1]         = tamanhos->tela[1] * (float)CantoFixo           / 360 * 6;

    tamanhos->bloco1[0]       = tamanhos->tela[0] * (float)MedidaImgBloco      / 640 * 2;
    tamanhos->bloco1[1]       = tamanhos->tela[1] * (float)MedidaImgBloco      / 360 * 2;

    tamanhos->botao1[0]       = tamanhos->tela[0] * (float)MedidaImgBloco      / 640 * 2;
    tamanhos->botao1[1]       = tamanhos->tela[1] * (float)MedidaImgBloco      / 360 * 2;

    tamanhos->botao2[0]       = tamanhos->tela[0] * (float)MedidaImgBloco      / 640 * 2;
    tamanhos->botao2[1]       = tamanhos->tela[1] * (float)MedidaImgBloco      / 360 * 2;

    tamanhos->barra_vida[0]   = tamanhos->tela[0] * (float)(EscalaHud*4)       / 640 * 2;
    tamanhos->barra_vida[1]   = tamanhos->tela[1] * (float)(EscalaHud*1)       / 360 * 2;

    tamanhos->jogador[0]      = tamanhos->tela[0] * (float)MedidaImgPlayerX    / 640 * 2;
    tamanhos->jogador[1]      = tamanhos->tela[1] * (float)MedidaImgPlayerY    / 360 * 2;

    tamanhos->inimigo1[0]     = tamanhos->tela[0] * (float)MedidaImgInimigo1X  / 640 * 2;
    tamanhos->inimigo1[1]     = tamanhos->tela[1] * (float)MedidaImgInimigo1Y  / 360 * 2;

    tamanhos->inimigo2[0]     = tamanhos->tela[0] * (float)MedidaImgInimigo2X  / 640 * 2;
    tamanhos->inimigo2[1]     = tamanhos->tela[1] * (float)MedidaImgInimigo2Y  / 360 * 2;

    tamanhos->jogador_coli[0] = tamanhos->tela[0] * (float)MedidaImgPlayerColiX / 640 * 2;
    tamanhos->jogador_coli[1] = tamanhos->tela[1] * (float)MedidaImgPlayerColiY / 360 * 2;

}

void InitCenaGeral(VariveisGerais *geral){
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_mouse();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();


    GetTamanhos(&geral->tamanhos);
    geral->janela               = al_create_display(geral->tamanhos.tela[0], geral->tamanhos.tela[1]);
    geral->textura_hud          = al_load_bitmap("../../sprites/ui/panels/hud geral.png");
    geral->fila_evento          = al_create_event_queue();
    geral->timer                = al_create_timer(1.0 / 60.0); // 60 FPS
    geral->rodando              = true;
    geral->fullscrean           = false;
    geral->troca_reso           = false;
    geral->debug                = false;
    geral->botao_mouse_direito  = false;
    geral->botao_mouse_esquerdo = false;
    geral->botao_mouse_meio     = false;
    geral->botao_mouse_gira     = false;
    geral->cena                 = CENA_MENU;
    geral->cena_continuar       = CENA_JOGO;
    geral->resolucao_atual[0]   = geral->tamanhos.tela[0];
    geral->resolucao_atual[1]   = geral->tamanhos.tela[1];
    geral->resolucao_antiga[0]  = geral->tamanhos.tela[0];
    geral->resolucao_antiga[1]  = geral->tamanhos.tela[1];


    al_register_event_source(geral->fila_evento, al_get_display_event_source(geral->janela));
    al_register_event_source(geral->fila_evento, al_get_timer_event_source(geral->timer));
    al_register_event_source(geral->fila_evento, al_get_mouse_event_source());
    al_register_event_source(geral->fila_evento, al_get_keyboard_event_source());
}