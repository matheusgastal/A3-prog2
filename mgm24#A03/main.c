#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "personagem.h"
#include "telas.h"
#include "functions.h"
#include "tiros.h"
#define X_SCREEN 800
#define Y_SCREEN 600
#define PERSONAGEM_LADO (Y_SCREEN / 3) 



int main() {
    al_init();
    al_install_mouse();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();

    ALLEGRO_DISPLAY *janela = al_create_display(X_SCREEN, Y_SCREEN);

    ALLEGRO_FONT *fonte_titulo = al_load_font("fonts/Cave-Stone.ttf", 64, 0);
    ALLEGRO_FONT *fonte_opcao = al_load_font("fonts/Cave-Stone.ttf", 40, 0);
    ALLEGRO_BITMAP *background = al_load_bitmap("assets/background.png");

    if (!janela || !fonte_titulo || !fonte_opcao || !background) {
        printf("Erro ao iniciar Allegro ou carregar recursos.\n");
        return -1;
    }

    EstadoJogo estado = ESTADO_MENU;
    EstadoJogoDados *dados_do_jogo = inicia_dados();
    while (estado != ESTADO_SAIR) {
        if (estado == ESTADO_MENU){
            estado = menu_principal(janela, fonte_titulo, fonte_opcao, background);
            resetar_dados(dados_do_jogo);
        }
        else if (estado == ESTADO_JOGO){
            Inimigo inimigos[MAX_INIMIGOS];
            inicializa_inimigos(inimigos);
            estado = jogar(janela,background, dados_do_jogo, inimigos);
        }
        else if(estado == ESTADO_PAUSE){
            estado = PAUSE(janela, fonte_titulo, fonte_opcao, background);
        }
        else if(estado == ESTADO_GAME_OVER)
            estado = game_over(janela,fonte_titulo, fonte_opcao, background, dados_do_jogo);
        else if(estado == ESTADO_WIN){
            estado = win(janela, fonte_titulo, fonte_opcao,background);
        }
    }

    al_destroy_font(fonte_titulo);
    al_destroy_font(fonte_opcao);
    al_destroy_bitmap(background);
    al_destroy_display(janela);
    return 0;
}

