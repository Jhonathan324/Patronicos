#include "gerais.h"
#include <string.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include <stdlib.h>

// Função de erro robusta: imprime no console, tenta caixa de diálogo e espera Enter
void ErroFatal(const char *mensagem) {
    fprintf(stderr, "ERRO FATAL: %s\n", mensagem);
    // Tenta mostrar caixa de diálogo se o add-on estiver disponível
    if (al_is_native_dialog_addon_initialized()) {
        al_show_native_message_box(NULL, "Erro Fatal", "Falha ao carregar recurso", mensagem, NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
    printf("\nPressione Enter para sair...\n");
    getchar();
    exit(1);
}

// Função auxiliar para verificar se um arquivo existe
int file_exists(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}

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

// Inicialização geral com verificações de recursos
void InitCenaGeral(VariveisGerais *geral){
    // Inicializa Allegro e add-ons
    if (!al_init()) {
        ErroFatal("Falha ao inicializar Allegro.");
    }
    if (!al_init_font_addon()) {
        ErroFatal("Falha ao inicializar add-on de fontes.");
    }
    if (!al_init_ttf_addon()) {
        ErroFatal("Falha ao inicializar add-on de TTF.");
    }
    if (!al_init_primitives_addon()) {
        ErroFatal("Falha ao inicializar add-on de primitivas.");
    }
    if (!al_init_image_addon()) {
        ErroFatal("Falha ao inicializar add-on de imagens.");
    }
    if (!al_install_mouse()) {
        ErroFatal("Falha ao instalar mouse.");
    }
    if (!al_install_keyboard()) {
        ErroFatal("Falha ao instalar teclado.");
    }
    if (!al_install_audio()) {
        ErroFatal("Falha ao instalar áudio.");
    }
    if (!al_init_acodec_addon()) {
        ErroFatal("Falha ao inicializar add-on de codecs de áudio.");
    }
    if (!al_init_native_dialog_addon()) {
        // Apenas aviso, pois ainda podemos usar console
        fprintf(stderr, "Aviso: add-on de diálogo nativo não inicializado.\n");
    }

    GetTamanhos(&geral->tamanhos);
    printf("Criando display com resolução %dx%d\n", (int)geral->tamanhos.tela[0], (int)geral->tamanhos.tela[1]);
    geral->janela = al_create_display(geral->tamanhos.tela[0], geral->tamanhos.tela[1]);
    if (!geral->janela) {
        ErroFatal("Falha ao criar a janela do display.");
    }

    const char *hud_path = "../../sprites/ui/panels/hud geral.png";
    printf("Carregando HUD: %s\n", hud_path);
    if (!file_exists(hud_path)) {
        ErroFatal("Arquivo de textura HUD não encontrado!");
    }
    geral->textura_hud = al_load_bitmap(hud_path);
    if (!geral->textura_hud) {
        ErroFatal("Falha ao carregar textura HUD (arquivo existe, mas falha ao carregar).");
    }

    geral->fila_evento = al_create_event_queue();
    if (!geral->fila_evento) {
        ErroFatal("Falha ao criar fila de eventos.");
    }

    geral->timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!geral->timer) {
        ErroFatal("Falha ao criar timer.");
    }

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
    memset(geral->matriz_fogo,0,sizeof(geral->matriz_fogo));
    for(int j = 0; j<64;j++){
        geral->matriz_fogo[0][j] = 36;
    }

    al_register_event_source(geral->fila_evento, al_get_display_event_source(geral->janela));
    al_register_event_source(geral->fila_evento, al_get_timer_event_source(geral->timer));
    al_register_event_source(geral->fila_evento, al_get_mouse_event_source());
    al_register_event_source(geral->fila_evento, al_get_keyboard_event_source());

    printf("Inicialização geral concluída com sucesso.\n");
}

void CalcularGeral(VariveisGerais *geral) {
    GetTamanhos(&geral->tamanhos);
    al_resize_display(geral->janela, geral->tamanhos.tela[0], geral->tamanhos.tela[1]);
    if (geral->fullscrean)
        al_set_display_flag(geral->janela, ALLEGRO_FULLSCREEN_WINDOW, true);
    else
        al_set_display_flag(geral->janela, ALLEGRO_FULLSCREEN_WINDOW, false);
    geral->resolucao_atual[0] = geral->tamanhos.tela[0];
    geral->resolucao_atual[1] = geral->tamanhos.tela[1];
}

void ModuloEvento(VariveisGerais *geral){
    al_wait_for_event(geral->fila_evento, &geral->evento);
    switch(geral->evento.type){

    case ALLEGRO_EVENT_DISPLAY_CLOSE:{
        geral->rodando = false;
    }break;

    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:{
        if (geral->evento.mouse.button & 1) geral->botao_mouse_esquerdo = true;
        if (geral->evento.mouse.button & 2) geral->botao_mouse_direito = true;
        if (geral->evento.mouse.button & 4) geral->botao_mouse_meio = true;
    }break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:{
        if (geral->evento.mouse.button & 1) geral->botao_mouse_esquerdo = false;
        if (geral->evento.mouse.button & 2) geral->botao_mouse_direito = false;
        if (geral->evento.mouse.button & 4) geral->botao_mouse_meio = false;
        }break;
    case ALLEGRO_EVENT_MOUSE_AXES:{
        geral->ponto_mouse.x = geral->evento.mouse.x;
        geral->ponto_mouse.y = geral->evento.mouse.y;
        geral->botao_mouse_gira = geral->evento.mouse.dz; // scroll vertical
        }break;
    }
}
