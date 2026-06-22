#include "gerais.h"




void DesenharRetangulo(VMM_Retangulo *retangulo, ALLEGRO_COLOR cor){
    al_draw_filled_rectangle(retangulo->x,retangulo->y,retangulo->x+retangulo->w,retangulo->y+retangulo->h, cor);
}





ALLEGRO_COLOR CorFogo(short cor){
    switch(cor){
        case 0:
            return al_map_rgb(7,7,7);
            break;

        case 1:
            return al_map_rgb(31,7,7);
            break;

        case 2:
            return al_map_rgb(47,15,7);
            break;

        case 3:
            return al_map_rgb(71,15,7);
            break;

        case 4:
            return al_map_rgb(87,23,7);
            break;

        case 5:
            return al_map_rgb(103,31,7);
            break;

        case 6:
            return al_map_rgb(119,31,7);
            break;

        case 7:
            return al_map_rgb(143,39,7);
            break;

        case 8:
            return al_map_rgb(159,47,7);
            break;

        case 9:
            return al_map_rgb(175,63,7);
            break;

        case 10:
            return al_map_rgb(191,71,7);
            break;

        case 11:
            return al_map_rgb(199,71,7);
            break;

        case 12:
            return al_map_rgb(223,79,7);
            break;

        case 13:
            return al_map_rgb(223,87,7);
            break;

        case 14:
            return al_map_rgb(223,87,7);
            break;

        case 15:
            return al_map_rgb(215,95,7);
            break;

        case 16:
            return al_map_rgb(215,95,7);
            break;

        case 17:
            return al_map_rgb(215,103,15);
            break;

        case 18:
            return al_map_rgb(207,111,15);
            break;

        case 19:
            return al_map_rgb(207,119,15);
            break;

        case 20:
            return al_map_rgb(207,127,15);
            break;

        case 21:
            return al_map_rgb(207,135,23);
            break;

        case 22:
            return al_map_rgb(199,135,23);
            break;

        case 23:
            return al_map_rgb(199,143,23);
            break;

        case 24:
            return al_map_rgb(199,151,31);
            break;

        case 25:
            return al_map_rgb(191,159,31);
            break;

        case 26:
            return al_map_rgb(191,159,31);
            break;

        case 27:
            return al_map_rgb(191,167,39);
            break;

        case 28:
            return al_map_rgb(191,167,39);
            break;

        case 29:
            return al_map_rgb(191,175,47);
            break;

        case 30:
            return al_map_rgb(183,175,47);
            break;

        case 31:
            return al_map_rgb(183,183,47);
            break;

        case 32:
            return al_map_rgb(183,183,55);
            break;

        case 33:
            return al_map_rgb(207,207,111);
            break;

        case 34:
            return al_map_rgb(223,223,159);
            break;

        case 35:
            return al_map_rgb(239,239,199);
            break;

        case 36:
            return al_map_rgb(255,255,255);
            break;
    }
}

void DesenharFogo(float tamanho_tela[2], short matriz[][64]){
        
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

void DestruirMoldura(Moldura *moldura){
    al_destroy_bitmap(moldura->textura);
    moldura->textura = NULL;
}