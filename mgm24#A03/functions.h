#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "personagem.h"

void desenha_texto_com_borda(ALLEGRO_FONT *fonte, float x, float y, int alinhamento, const char *texto, ALLEGRO_COLOR cor_texto, ALLEGRO_COLOR cor_borda);
unsigned char collision_2D(personagem *a, personagem *b);
void update_position(personagem *p1, personagem *p2);


#endif