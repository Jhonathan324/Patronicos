#include "../hdr/geral.h"
#include <string.h>



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

// -------------------- Moldura --------------------
Moldura InitMoldura( VMM_Retangulo *retangulo, char *file){
    Moldura moldura = {*retangulo, file ? al_load_bitmap(file) : NULL};
    if (file && !moldura.textura) {
        ErroFatal("Falha ao carregar moldura: ");
    }
    return moldura;
}

void DesenharMoldura(Moldura moldura) {
    if (moldura.textura) {
        DesenharImagemEscala(moldura.textura, NULL,
                             EscalaMoldura, EscalaMoldura,
                             EscalaMoldura, EscalaMoldura,
                             2.0f, &moldura.retangulo);
    } else {
        // fallback: desenha um retângulo simples
        al_draw_rectangle(moldura.retangulo.x, moldura.retangulo.y,
                          moldura.retangulo.x + moldura.retangulo.w,
                          moldura.retangulo.y + moldura.retangulo.h,
                          al_map_rgb(255,255,255), 1);
    }
}

void DestruirMoldura(Moldura *moldura){
    //al_destroy_bitmap(moldura->textura);
    moldura->textura = NULL;
}

// -------------------- Efeito Fogo --------------------
ALLEGRO_COLOR CorFogo(char cor){
    switch(cor){
        case 0:  return al_map_rgba(7,7,7,0);break;
        case 1:  return al_map_rgba(31,7,7,8);break;
        case 2:  return al_map_rgba(47,15,7,16);break;
        case 3:  return al_map_rgba(71,15,7,24);break;
        case 4:  return al_map_rgba(87,23,7,32);break;
        case 5:  return al_map_rgba(103,31,7,40);break;
        case 6:  return al_map_rgba(119,31,7,48);break;
        case 7:  return al_map_rgba(143,39,7,56);break;
        case 8:  return al_map_rgba(159,47,7,64);break;
        case 9:  return al_map_rgba(175,63,7,72);break;
        case 10: return al_map_rgba(191,71,7,80);break;
        case 11: return al_map_rgba(199,71,7,88);break;
        case 12: return al_map_rgba(223,79,7,96);break;
        case 13: return al_map_rgba(223,87,7,104);break;
        case 14: return al_map_rgba(223,87,7,112);break;
        case 15: return al_map_rgba(215,95,7,120);break;
        case 16: return al_map_rgba(215,95,7,128);break;
        case 17: return al_map_rgba(215,103,15,136);break;
        case 18: return al_map_rgba(207,111,15,144);break;
        case 19: return al_map_rgba(207,119,15,152);break;
        case 20: return al_map_rgba(207,127,15,160);break;
        case 21: return al_map_rgba(207,135,23,168);break;
        case 22: return al_map_rgba(199,135,23,176);break;
        case 23: return al_map_rgba(199,143,23,184);break;
        case 24: return al_map_rgba(199,151,31,192);break;
        case 25: return al_map_rgba(191,159,31,200);break;
        case 26: return al_map_rgba(191,159,31,208);break;
        case 27: return al_map_rgba(191,167,39,216);break;
        case 28: return al_map_rgba(191,167,39,224);break;
        case 29: return al_map_rgba(191,175,47,232);break;
        case 30: return al_map_rgba(183,175,47,240);break;
        case 31: return al_map_rgba(183,183,47,248);break;
        case 32: return al_map_rgba(183,183,55,255);break;
        case 33: return al_map_rgba(207,207,111,255);break;
        case 34: return al_map_rgba(223,223,159,255);break;
        case 35: return al_map_rgba(239,239,199,255);break;
        case 36: return al_map_rgba(255,255,255,255);break;
        default: return al_map_rgba(0,0,0,0);break;
    }
}

void DesenharFogo(float tamanho_tela[2],  char matriz[][64]){

    for(int i = 39; i>=0; i--){
        for(int j = 0; j<64; j++){
            if(i > 0){
                matriz[i][j] = matriz[i-1][j] - 1 - rand()%2;
                if (matriz[i][j] < 0) matriz[i][j] = 0;
            }
            DesenharRetangulo(&(VMM_Retangulo){j*10,35*10-i*10,10,10},CorFogo(matriz[i][j]));
        }
    }
}

// -------------------- Texto --------------------
CampoTexto InitTexto(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor_fundo,
                     char *texto, char *imagem, ALLEGRO_FONT *fonte,
                     ALLEGRO_COLOR cor_fonte, bool alinhado) {
    CampoTexto ct = {0};
    ct.retangulo = *retangulo;
    ct.texto = texto;
    ct.cor_fundo = cor_fundo;
    if (imagem) {
        ct.imagem = al_load_bitmap(imagem);
        if (!ct.imagem) {
            ErroFatal("Falha ao carregar imagem de texto: ");
        }
    }
    if (texto && fonte) {
        int x, y, w, h;
        al_get_text_dimensions(fonte, texto, &x, &y, &w, &h);
        ct.proporcao = (float)w / h;
        if (!alinhado) {
            retangulo->w = retangulo->h * ct.proporcao;
            ct.retangulo.w = retangulo->w;
        }
        ALLEGRO_BITMAP *surface = al_create_bitmap(x, y);
        if (!surface) {
            ErroFatal("Falha ao criar bitmap de texto.");
        }
        al_set_target_bitmap(surface);
        al_clear_to_color(al_map_rgba(0,0,0,0));
        al_draw_text(fonte, cor_fonte, 0, 0, 0, texto);
        al_set_target_backbuffer(al_get_current_display());
        ct.textura_texto = surface;
    }
    return ct;
}

void DesenharTexto(CampoTexto texto) {
    if (texto.cor_fundo.a) {
        al_draw_filled_rectangle(texto.retangulo.x, texto.retangulo.y,
                                  texto.retangulo.x + texto.retangulo.w,
                                  texto.retangulo.y + texto.retangulo.h,
                                  texto.cor_fundo);
    }
    if (texto.imagem) {
        al_draw_scaled_bitmap(texto.imagem, 0,0,
                              al_get_bitmap_width(texto.imagem),
                              al_get_bitmap_height(texto.imagem),
                              texto.retangulo.x, texto.retangulo.y,
                              texto.retangulo.w, texto.retangulo.h, 0);
    }
    if (texto.textura_texto) {
        VMM_Retangulo r = texto.retangulo;
        r.w *= 0.8;
        r.h *= 0.8;
        r.x += (texto.retangulo.w - r.w) / 2;
        r.y += (texto.retangulo.h - r.h) / 2;
        al_draw_scaled_bitmap(texto.textura_texto, 0,0,
                              al_get_bitmap_width(texto.textura_texto),
                              al_get_bitmap_height(texto.textura_texto),
                              r.x, r.y, r.w, r.h, 0);
    }
}

void DestruirTexto(CampoTexto *texto) {
    if (texto->textura_texto) al_destroy_bitmap(texto->textura_texto);
    if (texto->imagem) al_destroy_bitmap(texto->imagem);
    texto->textura_texto = NULL;
    texto->imagem = NULL;
}

// -------------------- Marcador --------------------
Marcador InitMarcador(VMM_Retangulo *retangulo, bool ativo, char *imagem1,
                      ALLEGRO_COLOR cor1, ALLEGRO_COLOR cor2) {
    Marcador m = {0};
    m.retangulo = *retangulo;
    m.ativo = ativo;
    m.cor1 = cor1;
    m.cor2 = cor2;
    if (imagem1) {
        m.imagem1 = al_load_bitmap(imagem1);
        if (!m.imagem1) {
            ErroFatal("Falha ao carregar imagem de marcador: ");
        }
    }
    return m;
}

void DesenharMarcador(Marcador marcador) {
    if (marcador.imagem1) {
        int w = al_get_bitmap_width(marcador.imagem1);
        int h = al_get_bitmap_height(marcador.imagem1);
        // desenha metade da imagem (ativado/desativado)
        float sx = marcador.ativo ? w/2 : 0;
        al_draw_scaled_bitmap(marcador.imagem1, sx, 0, w/2, h,
                              marcador.retangulo.x, marcador.retangulo.y,
                              marcador.retangulo.w, marcador.retangulo.h, 0);
    }
    if (marcador.sobre) {
        al_draw_filled_rectangle(marcador.retangulo.x, marcador.retangulo.y,
                                  marcador.retangulo.x + marcador.retangulo.w,
                                  marcador.retangulo.y + marcador.retangulo.h,
                                  marcador.cor2);
    }
}

bool VerificarMarcador(Marcador *marcador, VMM_Ponto mouse, bool click) {
    if (marcador->timer) {
        if (marcador->timer > 1) marcador->timer--;
        else {
            marcador->timer = 0;
            marcador->ativo = !marcador->ativo;
            return true;
        }
    }
    VMM_Retangulo r = marcador->retangulo;
    if (mouse.x >= r.x && mouse.x <= r.x + r.w &&
        mouse.y >= r.y && mouse.y <= r.y + r.h) {
        marcador->sobre = true;
        if (click) marcador->timer = 15;
    } else {
        marcador->sobre = false;
    }
    return false;
}

void DestruirMarcador(Marcador *marcador) {
    //if (marcador->imagem1) al_destroy_bitmap(marcador->imagem1);
    marcador->imagem1 = NULL;
}

// -------------------- Botão --------------------
Botao InitBotao(VMM_Retangulo *retangulo, char *imagem, char *texto,
                ALLEGRO_COLOR cor1, ALLEGRO_COLOR cor2, int indice,
                ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor_fonte) {
    Botao b = {0};
    b.retangulo = *retangulo;
    b.texto = texto;
    b.cor1 = cor1;
    b.cor2 = cor2;
    b.indice = indice;
    if (texto && fonte) {
        int x, y, h, w;
        al_get_text_dimensions(fonte, texto, &x, &y, &w, &h);
        b.proporcao = (float)w / h;
        retangulo->w = retangulo->h * b.proporcao;
        b.retangulo.w = retangulo->w;
        // cria textura do texto (opcional, aqui usamos al_draw_text diretamente)
    }
    if (imagem) {
        b.imagem = al_load_bitmap(imagem);
        if (!b.imagem) {
            ErroFatal("Falha ao carregar imagem de botão: ");
        }
    }
    return b;
}

void DesenharBotao(ALLEGRO_FONT *font, Botao botao) {
    if (!botao.imagem) {
        al_draw_filled_rectangle(botao.retangulo.x, botao.retangulo.y,
                                 botao.retangulo.x + botao.retangulo.w,
                                 botao.retangulo.y + botao.retangulo.h,
                                 botao.sobre ? botao.cor2 : botao.cor1);
    } else {
        DesenharImagemEscala(botao.imagem, NULL,
                             EscalaBotao, EscalaBotao,
                             EscalaBotao, EscalaBotao,
                             3.0f, &botao.retangulo);
        if (botao.sobre) {
            al_draw_filled_rectangle(botao.retangulo.x, botao.retangulo.y,
                                     botao.retangulo.x + botao.retangulo.w,
                                     botao.retangulo.y + botao.retangulo.h,
                                     al_map_rgba(0,0,0,64));
        }
    }
    if (botao.texto && font) {
        VMM_Retangulo r = botao.retangulo;
        r.w *= 0.8;
        r.h *= 0.8;
        r.x += (botao.retangulo.w - r.w) / 2;
        r.y += (botao.retangulo.h - r.h) / 2;
        al_draw_text(font, botao.cor2, r.x, r.y, 0, botao.texto);
    }
}

bool VerificarBotao(Botao *botao, VMM_Ponto mouse, bool click) {
    if (botao->timer) {
        if (botao->timer > 1) botao->timer--;
        else {
            botao->timer = 0;
            return true;
        }
    }
    VMM_Retangulo r = botao->retangulo;
    if (mouse.x >= r.x && mouse.x <= r.x + r.w &&
        mouse.y >= r.y && mouse.y <= r.y + r.h) {
        botao->sobre = true;
        if (click) botao->timer = 15;
    } else {
        botao->sobre = false;
    }
    return false;
}

void DestruirBotao(Botao *botao) {
    //if (botao->imagem) al_destroy_bitmap(botao->imagem);
    //if (botao->textura) al_destroy_bitmap(botao->textura);
    botao->imagem = NULL;
    botao->textura = NULL;
}

// -------------------- Botão Expansivo --------------------
BotaoExpansivo InitBotaoExpansivo(VMM_Retangulo *retangulo, char *imagem, char *texto,
                                  char *textos[], ALLEGRO_COLOR cor, ALLEGRO_COLOR cor2,
                                  int indice, ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor_fonte,
                                  int n) {
    BotaoExpansivo be = {0};
    be.n = n;
    be.botao_pai = InitBotao(retangulo, imagem, texto, cor, cor2, indice, fonte, cor_fonte);
    be.botao_filho = malloc(n * sizeof(Botao));
    if (!be.botao_filho) {
        ErroFatal("Falha ao alocar memória para botões filhos.");
    }
    for (int i = 0; i < n; i++) {
        VMM_Retangulo r = *retangulo;
        r.y += retangulo->h * (i + 1);
        be.botao_filho[i] = InitBotao(&r, imagem, textos[i], cor, cor2, indice, fonte, cor_fonte);
    }
    be.expandido = false;
    return be;
}

void DesenharBotaoExpansivo(BotaoExpansivo botao) {
    DesenharBotao(NULL, botao.botao_pai);
    if (botao.expandido) {
        for (int i = 0; i < botao.n; i++) {
            DesenharBotao(NULL, botao.botao_filho[i]);
        }
    }
}

void DestruirBotaoExpansivo(BotaoExpansivo *botao) {
    DestruirBotao(&botao->botao_pai);
    if (botao->botao_filho) {
        for (int i = 0; i < botao->n; i++) {
            DestruirBotao(&botao->botao_filho[i]);
        }
        free(botao->botao_filho);
        botao->botao_filho = NULL;
    }
}
