#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "types.h"

// Verificações
void ErroFatal(const char *mensagem);
int file_exists(const char *path);

double VMM_Clamp(double x,  double minimo, double maximo);

// Retangulos
void DesenharRetangulo(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor);
void DesenharImagemEscala(ALLEGRO_BITMAP *textura, VMM_Retangulo *retangulo_in,
    float left_width, float right_width,
    float top_height, float bottom_height,
    float escala, VMM_Retangulo *retangulo_destino);
    
void CentralizarRectInRect(VMM_Retangulo *rect_pai, VMM_Retangulo *rect_filho);
void CentralizarRectsInRectV(VMM_Retangulo *pai, VMM_Retangulo *filho[], int n, float borda_x, float borda_y);
bool ColisaoRetangulo(VMM_Retangulo *r1, VMM_Retangulo *r2);

// Efeitos (fogo)
ALLEGRO_COLOR CorFogo(char cor);
void DesenharFogo(float tamanho_tela[2], char matriz[][64]);

#endif // UTILS_H_INCLUDED
