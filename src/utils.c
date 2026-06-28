#include "../hdr/geral.h"

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

// -------------------- Verificações --------------------
double VMM_Clamp(double x,  double minimo, double maximo){
    if(x > maximo) return maximo;
    if(x < minimo) return minimo;
    return x;
}

// -------------------- Desenho de retângulo --------------------
void DesenharRetangulo(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor){
    al_draw_filled_rectangle(retangulo->x,retangulo->y,retangulo->x+retangulo->w,retangulo->y+retangulo->h, cor);
}

// -------------------- 9-slice com escala --------------------
void DesenharImagemEscala(
    ALLEGRO_BITMAP *textura,
    VMM_Retangulo *retangulo_interno,
    float largura_esquerda,
    float largura_direita,
    float altura_cima,
    float altura_baixo,
    float escala,
    VMM_Retangulo *retangulo_destino){

    if (!textura || !retangulo_destino) return;

    float tex_w = al_get_bitmap_width(textura);
    float tex_h = al_get_bitmap_height(textura);

    // limites da textura (9-slice source)
    float x0 = 0;
    float x1 = largura_esquerda;
    float x2 = tex_w - largura_direita;
    float x3 = tex_w;

    float y0 = 0;
    float y1 = altura_cima;
    float y2 = tex_h - altura_baixo;
    float y3 = tex_h;

    // destino fixo
    float dx = retangulo_destino->x;
    float dy = retangulo_destino->y;
    float dw = retangulo_destino->w;
    float dh = retangulo_destino->h;

    // bordas no destino (fixas)
    float dl = largura_esquerda;
    float dr = largura_direita;
    float dt = altura_cima;
    float db = altura_baixo;

    // centro no destino
    float dcw = dw - dl - dr;
    float dch = dh - dt - db;

    if (dcw < 0) dcw = 0;
    if (dch < 0) dch = 0;

    // aplica escala só no "conteúdo interno" (centro)
    float sw = (x2 - x1);
    float sh = (y2 - y1);

    float scaled_sw = sw * escala;
    float scaled_sh = sh * escala;

    // evita distorção de layout do 9-slice: usamos escala só no desenho do centro
    float center_src_x = x1;
    float center_src_y = y1;

    float center_src_w = sw;
    float center_src_h = sh;

    float center_dst_x = dx + dl;
    float center_dst_y = dy + dt;
    float center_dst_w = dcw;
    float center_dst_h = dch;

    // ---- CANTOS ----
    al_draw_scaled_bitmap(textura,
        x0, y0, x1 - x0, y1 - y0,
        dx, dy, dl, dt, 0);

    al_draw_scaled_bitmap(textura,
        x2, y0, x3 - x2, y1 - y0,
        dx + dl + dcw, dy, dr, dt, 0);

    al_draw_scaled_bitmap(textura,
        x0, y2, x1 - x0, y3 - y2,
        dx, dy + dt + dch, dl, db, 0);

    al_draw_scaled_bitmap(textura,
        x2, y2, x3 - x2, y3 - y2,
        dx + dl + dcw, dy + dt + dch, dr, db, 0);

    // ---- BORDAS ----
    al_draw_scaled_bitmap(textura,
        x1, y0, sw, y1 - y0,
        dx + dl, dy, dcw, dt, 0);

    al_draw_scaled_bitmap(textura,
        x1, y2, sw, y3 - y2,
        dx + dl, dy + dt + dch, dcw, db, 0);

    al_draw_scaled_bitmap(textura,
        x0, y1, x1 - x0, sh,
        dx, dy + dt, dl, dch, 0);

    al_draw_scaled_bitmap(textura,
        x2, y1, x3 - x2, sh,
        dx + dl + dcw, dy + dt, dr, dch, 0);

    // ---- CENTRO (com escala) ----
    al_draw_scaled_bitmap(textura,
        center_src_x, center_src_y,
        center_src_w, center_src_h,
        center_dst_x, center_dst_y,
        center_dst_w, center_dst_h,
        0);
}

// -------------------- Centralização --------------------
void CentralizarRectInRect(VMM_Retangulo *pai, VMM_Retangulo *filho) {
    filho->x = pai->x + (pai->w - filho->w) / 2;
    filho->y = pai->y + (pai->h - filho->h) / 2;
}

void CentralizarRectsInRectV(VMM_Retangulo *pai, VMM_Retangulo *filho[], int n, float borda_x, float borda_y)
{
    borda_x = borda_x * pai->w;
    borda_y = borda_y * pai->h;
    float soma = 0;
    for (int i = 0; i < n; i++)
        soma += filho[i]->h;
    float espaco_interno_remanecente = (pai->h - borda_y * 2) - soma;
    float espacamento = espaco_interno_remanecente / (n - 1);
    for (int i = 0; i < n; i++)
    {
        filho[i]->x = pai->x + (pai->w - filho[i]->w) / 2;
        if (i != 0)
        {
            filho[i]->y = filho[i - 1]->y + filho[i - 1]->h + espacamento;
        }
        else
        {
            filho[i]->y = pai->y + borda_y;
        }
    }
}

bool ColisaoRetangulo(VMM_Retangulo *r1, VMM_Retangulo *r2){
    float Amin, Amax, Bmin, Bmax;
    // Horizontal intersection
    Amin = r1->x;
    Amax = Amin + r1->w;
    Bmin = r2->x;
    Bmax = Bmin + r2->w;
    if (Bmin > Amin) {
        Amin = Bmin;
    }
    if (Bmax < Amax) {
        Amax = Bmax;
    }
    if (Amax < Amin)
        return false;
    // Vertical intersection
    Amin = r1->y;
    Amax = Amin + r1->h;
    Bmin = r2->y;
    Bmax = Bmin + r2->h;
    if (Bmin > Amin) {
        Amin = Bmin;
    }
    if (Bmax < Amax) {
        Amax = Bmax;
    }
    if (Amax < Amin)
        return false;
    return true;
}



// -------------------- Efeito Fogo --------------------
ALLEGRO_COLOR CorFogo(char cor){
    switch(cor){
        case 0:  return al_map_rgb(7,7,7);break;
        case 1:  return al_map_rgb(31,7,7);break;
        case 2:  return al_map_rgb(47,15,7);break;
        case 3:  return al_map_rgb(71,15,7);break;
        case 4:  return al_map_rgb(87,23,7);break;
        case 5:  return al_map_rgb(103,31,7);break;
        case 6:  return al_map_rgb(119,31,7);break;
        case 7:  return al_map_rgb(143,39,7);break;
        case 8:  return al_map_rgb(159,47,7);break;
        case 9:  return al_map_rgb(175,63,7);break;
        case 10: return al_map_rgb(191,71,7);break;
        case 11: return al_map_rgb(199,71,7);break;
        case 12: return al_map_rgb(223,79,7);break;
        case 13: return al_map_rgb(223,87,7);break;
        case 14: return al_map_rgb(223,87,7);break;
        case 15: return al_map_rgb(215,95,7);break;
        case 16: return al_map_rgb(215,95,7);break;
        case 17: return al_map_rgb(215,103,15);break;
        case 18: return al_map_rgb(207,111,15);break;
        case 19: return al_map_rgb(207,119,15);break;
        case 20: return al_map_rgb(207,127,15);break;
        case 21: return al_map_rgb(207,135,23);break;
        case 22: return al_map_rgb(199,135,23);break;
        case 23: return al_map_rgb(199,143,23);break;
        case 24: return al_map_rgb(199,151,31);break;
        case 25: return al_map_rgb(191,159,31);break;
        case 26: return al_map_rgb(191,159,31);break;
        case 27: return al_map_rgb(191,167,39);break;
        case 28: return al_map_rgb(191,167,39);break;
        case 29: return al_map_rgb(191,175,47);break;
        case 30: return al_map_rgb(183,175,47);break;
        case 31: return al_map_rgb(183,183,47);break;
        case 32: return al_map_rgb(183,183,55);break;
        case 33: return al_map_rgb(207,207,111);break;
        case 34: return al_map_rgb(223,223,159);break;
        case 35: return al_map_rgb(239,239,199);break;
        case 36: return al_map_rgb(255,255,255);break;
        default: return al_map_rgb(0,0,0);break;
    }
}

void DesenharFogo(float tamanho_tela[2],  char matriz[][64]){

    for(int i = 39; i>=0; i--){
        for(int j = 0; j<64; j++){
            if(i > 0){
                //matriz[i][j] = matriz[i-1][j] - 1 - rand()%2;
                matriz[i][j] = matriz[i-1][j+1] - 1 - rand()%2;
                if (matriz[i][j] < 0) matriz[i][j] = 0;
            }
            DesenharRetangulo(&(VMM_Retangulo){j*(tamanho_tela[0]/64),39*(tamanho_tela[1]/40)-i*(tamanho_tela[1]/40),tamanho_tela[0]/64,tamanho_tela[1]/40},CorFogo(matriz[i][j]));
        }
    }
}