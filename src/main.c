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
#include "jogo.h"
#include "mapa.h"



int main() {
    // Variaveis De cada cena
    VariveisGerais gerais;
    VariveisMenu menu;
    VariveisPause pause;
    VariveisConf conf;
    VariveisJogo jogo;
    VariaveisMapa mapa;
    gerais.tamanhos.escala = 2;  

    //Inits das cenas
    InitCenaGeral(&gerais);
    InitCenaMenu(&gerais, &menu);
    InitCenaPause(&gerais, &pause);
    InitCenaConf(&gerais, &conf);
    InitCenaJogo(&gerais, &jogo);
    InitCenaMapa(&gerais, &mapa);

    al_start_timer(gerais.timer);
    double tempo_inicial;
    double tempo_final;
    while (gerais.rodando) {
        tempo_inicial = al_get_timer_count(gerais.timer);
        ModuloEvento(&gerais);
        if (gerais.evento.type == ALLEGRO_EVENT_TIMER) {
            double delta_t = 10.0 / 60.0; // fixo, pois o timer é 60 FPS
            switch (gerais.cena) {
                case CENA_MENU:
                    LoopCenaMenu(&gerais, &menu);
                    DesenharCenaMenu(gerais, menu);
                    break;
                case CENA_PAUSE:
                    LoopCenaPause(&gerais, &pause);
                    DesenharCenaPause(gerais, pause);
                    break;
                case CENA_CONF:
                    LoopCenaConf(&gerais, &conf);
                    DesenharCenaConf(gerais, conf);
                    break;
                case CENA_JOGO:
                    LoopCenaJogo(&gerais, &jogo,(tempo_inicial - tempo_final) );
                    tempo_final =  al_get_timer_count(gerais.timer);
                    DesenharCenaJogo(gerais, jogo);
                    break;
                case CENA_CRIACAO:
                    LoopCenaMapa(&gerais, &mapa);
                    DesenharCenaMapa(gerais, mapa);
                    break;
                case CENA_SAIR:
                    gerais.rodando = false;
                    break;
                default:
                    gerais.cena = CENA_MENU;
                    break;
            }
            
            

            // Troca de resolução
            if (gerais.troca_reso) {
                gerais.troca_reso = false;
                InitCenaMenu(&gerais, &menu);
                InitCenaPause(&gerais, &pause);
                InitCenaConf(&gerais, &conf);
                InitCenaJogo(&gerais, &jogo);
                InitCenaMapa(&gerais, &mapa);
                // Atualiza tamanhos nos componentes que dependem
                CalcularCenaJogo(&gerais, &jogo);
                gerais.resolucao_antiga[0] = gerais.resolucao_atual[0];
                gerais.resolucao_antiga[1] = gerais.resolucao_atual[1];
            }

            al_flip_display();
        }
    }

    // Limpeza
    al_destroy_timer(gerais.timer);
    al_destroy_event_queue(gerais.fila_evento);
    al_destroy_display(gerais.janela);
    return 0;
}
