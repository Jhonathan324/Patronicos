#include "gerais.h"

void DesenharRetangulo(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor){
    al_draw_filled_rectangle(retangulo->x,retangulo->y,retangulo->x+retangulo->w,retangulo->y+retangulo->h, cor);
}

void DesenharImagemEscala(
    ALLEGRO_BITMAP *textura,
    VMM_Retangulo *retangulo_interno,
    float largura_esquerda,
    float largura_direita,
    float altura_cima,
    float altura_baixo,
    float escala,
    VMM_Retangulo *retangulo_destino)
{
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


Moldura InitMoldura( VMM_Retangulo *retangulo, char *file){
    Moldura moldura = {*retangulo, file ? al_load_bitmap(file) : NULL};
    return moldura;
}

void DestruirMoldura(Moldura *moldura){
    al_destroy_bitmap(moldura->textura);
    moldura->textura = NULL;
}




Botao InitBotao( VMM_Retangulo *retangulo, char *imagem, char *texto, ALLEGRO_COLOR cor1, ALLEGRO_COLOR cor2, int indice, ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor_fonte)
{
    // texto
    int x, y;
    float proporcao = 1;
    ALLEGRO_BITMAP *textura_texto = NULL;
    if (texto)
    {
        x = al_get_text_width(fonte, texto);
        y = al_get_font_line_height(fonte);
        proporcao = (float)x / y;
        retangulo->w = retangulo->h * proporcao;
    }
    // Botão

    Botao botao = {
        .retangulo = *retangulo, // retangulo
        .texto = texto,      // texto
        .proporcao = proporcao,  // proporcao (porporção entre a largura sobre altura)
        .timer = 0,          // timer
        .sobre = false,      // sobre (verificar se o mouse esta sobre)
        .indice = indice,     // indice
        .cor1 = cor1,       // cor1 e cor2
        .cor2 = cor2,
        .textura = textura_texto,                                              // textura texto
        .imagem = imagem ? al_load_bitmap(imagem) : NULL};    // textura da imagem

    float tamanho_canto;
    if (retangulo->h >= retangulo->w)
        tamanho_canto = retangulo->w / 2;
    else
        tamanho_canto = retangulo->h / 2;

    return botao;
}

void DesenharBotao(ALLEGRO_FONT *font, Botao botao){
    if (!botao.imagem)
    {
        DesenharRetangulo(&botao.retangulo,  (!botao.sobre) ? al_map_rgba(botao.cor1.r, botao.cor1.g, botao.cor1.b, 126) : al_map_rgba(botao.cor2.r, botao.cor2.g, botao.cor2.b, 0));
    }
    else
    {
        DesenharImagemEscala(
            botao.imagem,
            NULL,
            EscalaBotao,
            EscalaBotao,
            EscalaBotao,
            EscalaBotao,
            3.0f,
            &botao.retangulo
        );
        DesenharRetangulo(&botao.retangulo,  (!botao.sobre) ? al_map_rgba(botao.cor1.r, botao.cor1.g, botao.cor1.b, 126) : al_map_rgba(botao.cor2.r, botao.cor2.g, botao.cor2.b, 0));
    }
    if (botao.textura)
    {
        VMM_Retangulo retangulo_texto = botao.retangulo;
        retangulo_texto.h *= 0.8;
        retangulo_texto.w = retangulo_texto.h * botao.proporcao;
        retangulo_texto.x = (botao.retangulo.x + (botao.retangulo.w - retangulo_texto.w) / 2);
        retangulo_texto.y = (botao.retangulo.y + (botao.retangulo.h - retangulo_texto.h) / 2);
        al_draw_text(font, botao.cor2, botao.retangulo.x, botao.retangulo.y, 0, botao.texto);
    }
}

void DestruirBotao(Botao *botao){
    botao->texto = NULL;

    if(botao->textura){
        al_destroy_bitmap(botao->textura);
        botao->textura = NULL;
    }

    if(botao->imagem){
        al_destroy_bitmap(botao->imagem);
        botao->imagem = NULL;
    }
}


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