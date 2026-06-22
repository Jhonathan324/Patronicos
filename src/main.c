/*
Este programa utiliza a biblioteca Allegro 5 para criar uma interface gráfica interativa.
Ele introduz conceitos fundamentais de desenvolvimento de jogos:
1. Agrupamento de dados de entidades usando Structs.
2. Controle de movimento contínuo usando um vetor de estados de teclas.
3. Atualização física atrelada ao temporizador (Frame Rate Independency).
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/mouse.h>
#include <allegro5/keyboard.h>
#include "gerais.h"

VariveisGerais gerais;
Tamanhos tamanhos;


int main() {
    tamanhos.escala = 0;
    InitCenaGeral(&gerais);

    VariveisMenu menu = (VariveisMenu){AZUL};
    InitCenaMenu(&gerais, &menu);
    
    
    
    
    al_start_timer(gerais.timer);
    while(gerais.rodando){
        al_wait_for_event(gerais.fila_evento, &gerais.evento);
        if(gerais.evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            gerais.rodando = false;
        }
        else if (gerais.evento.type == ALLEGRO_EVENT_TIMER){
            switch(gerais.cena){
                case(CENA_MENU):{
                    DesenharCenaMenu(gerais, menu);
                    //DesenharFogo(gerais.tamanhos.tela, gerais.matriz_fogo);
                }break;

                default:{
                    gerais.cena = CENA_MENU;
                }break;
            }


            al_flip_display();
        }
    }

    // 6. Limpeza de Memória

    //al_destroy_sample_instance(sample_instance);
    //al_destroy_sample(sample);
    //al_destroy_bitmap(sprite_sheet);
    //al_destroy_font(font);
    al_destroy_timer(gerais.timer);
    al_destroy_event_queue(gerais.fila_evento);
    al_destroy_display(gerais.janela);

    return 0;
}


