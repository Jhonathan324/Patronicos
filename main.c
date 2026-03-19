#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

int main() {

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    int player_x = 400;
    int player_y = 550;

    int enemy_x = rand() % 800;
    int enemy_y = 0;

    int running = 1;

    al_start_timer(timer);

    while(running) {

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = 0;
        }

        if(event.type == ALLEGRO_EVENT_TIMER) {

            ALLEGRO_KEYBOARD_STATE state;
            al_get_keyboard_state(&state);

            if(al_key_down(&state, ALLEGRO_KEY_LEFT)) player_x -= 5;
            if(al_key_down(&state, ALLEGRO_KEY_RIGHT)) player_x += 5;
            if(al_key_down(&state, ALLEGRO_KEY_UP)) player_y -= 5;
            if(al_key_down(&state, ALLEGRO_KEY_DOWN)) player_y += 5;

            enemy_y += 4;

            if(enemy_y > 600) {
                enemy_y = 0;
                enemy_x = rand() % 800;
            }

            if(abs(player_x - enemy_x) < 30 && abs(player_y - enemy_y) < 30) {
                printf("GAME OVER\n");
                running = 0;
            }

            al_clear_to_color(al_map_rgb(0,0,0));

            al_draw_filled_rectangle(player_x-20, player_y-20, player_x+20, player_y+20,
                                     al_map_rgb(0,0,255));

            al_draw_filled_circle(enemy_x, enemy_y, 15,
                                  al_map_rgb(255,0,0));

            al_flip_display();
        }
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
