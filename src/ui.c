#include "../hdr/ui.h"



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

void DesenharBotao(ALLEGRO_FONT *font, Botao botao, VMM_Retangulo *dentro) {
    if (!botao.imagem) {
        al_draw_filled_rectangle(botao.retangulo.x, botao.retangulo.y,
                                botao.retangulo.x + botao.retangulo.w,
                                botao.retangulo.y + botao.retangulo.h,
                                botao.sobre ? botao.cor2 : botao.cor1);
    } 
    else {
        if (dentro){
            DesenharImagemEscala(botao.imagem, dentro,
                            EscalaBotao, EscalaBotao,
                            EscalaBotao, EscalaBotao,
                            3.0f, &botao.retangulo);
        }
        else{
            DesenharImagemEscala(botao.imagem, NULL,
                            EscalaBotao, EscalaBotao,
                            EscalaBotao, EscalaBotao,
                            3.0f, &botao.retangulo);
        }
        
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

void DesenharBotaoExpansivo(ALLEGRO_FONT *font, BotaoExpansivo botao, VMM_Retangulo *dentro){
    if(dentro){
        DesenharBotao(font, botao.botao_pai, dentro);
        if (botao.expandido) {
            for (int i = 0; i < botao.n; i++) {
                DesenharBotao(font, botao.botao_filho[i], NULL);
            }
        }
    }   
    else{
        DesenharBotao(font, botao.botao_pai, NULL);
        if (botao.expandido) {
            for (int i = 0; i < botao.n; i++) {
                DesenharBotao(font, botao.botao_filho[i], NULL);
            }
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
