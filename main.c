//Compilacao: gcc AggresiveSquares.c -o AggresiveSquares `pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_font-5`

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

int main(){
    al_init();
    al_install_keyboard();
    al_init_font_addon(); // Necessário para usar fontes

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    char flag = 0;
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    while(1){
        al_wait_for_event(queue, &event);

        if (event.type == 30) {
            al_clear_to_color(al_map_rgb(0, 0, 0)); // Fundo preto

            // Escreve na tela
            al_draw_text(font, 
                         al_map_rgb(255, 255, 255), // Cor branca
                         320,200,                  // Posição (centro da tela 320x320)
                         ALLEGRO_ALIGN_CENTRE, 
                         "JOGAR");
            al_draw_text(font, 
                         al_map_rgb(255, 255, 255), // Cor branca
                         320,240,                  // Posição (centro da tela 320x320)
                         ALLEGRO_ALIGN_CENTRE, 
                         "SAIR");
            al_flip_display();
        }
        else if (event.type == 42) break;
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
