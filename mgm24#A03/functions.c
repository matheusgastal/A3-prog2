#include <stdio.h>
#include "Joystick.h"	
#include "personagem.h"



void desenha_texto_com_borda(ALLEGRO_FONT *fonte, float x, float y, int alinhamento, const char *texto, ALLEGRO_COLOR cor_texto, ALLEGRO_COLOR cor_borda) {
    int desloc = 2; 

    al_draw_text(fonte, cor_borda, x - desloc, y, alinhamento, texto);
    al_draw_text(fonte, cor_borda, x + desloc, y, alinhamento, texto);
    al_draw_text(fonte, cor_borda, x, y - desloc, alinhamento, texto);
    al_draw_text(fonte, cor_borda, x, y + desloc, alinhamento, texto);

    al_draw_text(fonte, cor_texto, x, y, alinhamento, texto);
}


