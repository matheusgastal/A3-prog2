#ifndef TELAS_H
#define TELAS_H
#include "personagem.h"




EstadoJogo PAUSE(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background);
EstadoJogo menu_principal(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background);
EstadoJogo jogar(ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *background, EstadoJogoDados *dados, Inimigo inimigos[MAX_INIMIGOS]);
EstadoJogo game_over(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background, EstadoJogoDados *dados);
#endif	