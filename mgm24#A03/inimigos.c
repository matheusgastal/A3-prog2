#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "Joystick.h"
#include "inimigos.h"


void inicializa_inimigos(Inimigo inimigos[MAX_INIMIGOS]) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        inimigos[i].x = 1500 + (i * 800);  // Aumente a distância inicial
        inimigos[i].x_inicial = inimigos[i].x;
        inimigos[i].y = 300 + rand() % 100;
        inimigos[i].largura = 80;
        inimigos[i].altura = 80;
        inimigos[i].ativo = false;
        inimigos[i].destruido = false;
        inimigos[i].direcao_vertical = (rand() % 2) ? 1 : -1; // Direção aleatória
        inimigos[i].frame_atual = 0;
        inimigos[i].contador_frame = 0;
        inimigos[i].tempo_recarga_tiro = 2;
        inimigos[i].direcao_horizontal = (rand() % 2) ? 1 : -1;
    }
}




void atualizar_inimigos(Inimigo inimigos[MAX_INIMIGOS], float fundo_x, int frames_duration, int frames_inimigo) {

    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (!inimigos[i].ativo)
            continue;

        // Limites fixos no mundo
        float limite_superior = 300;
        float limite_inferior = 400;
        float limite_esquerdo = inimigos[i].x_inicial - 100;
        float limite_direito = inimigos[i].x_inicial + 100;

        // Movimento vertical
        if (inimigos[i].direcao_vertical == 1) {
            inimigos[i].y += 1;
            if (inimigos[i].y >= limite_inferior) {
                inimigos[i].direcao_vertical = -1;
            }
        } else {
            inimigos[i].y -= 1;
            if (inimigos[i].y <= limite_superior) {
                inimigos[i].direcao_vertical = 1;
            }
        }

        // Movimento horizontal
        if (inimigos[i].direcao_horizontal == 1) {
            inimigos[i].x += 1;
            if (inimigos[i].x >= limite_direito) {
                inimigos[i].direcao_horizontal = -1;
            }
        } else {
            inimigos[i].x -= 1;
            if (inimigos[i].x <= limite_esquerdo) {
                inimigos[i].direcao_horizontal = 1;
            }
        }

        // Animação
        inimigos[i].contador_frame++;
        if (inimigos[i].contador_frame >= frames_duration) {
            inimigos[i].contador_frame = 0;
            inimigos[i].frame_atual = (inimigos[i].frame_atual + 1) % frames_inimigo;
        }
    }
}