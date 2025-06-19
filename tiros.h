#ifndef TIROS_H
#define TIROS_H
#include "personagem.h"
#define MAX_TIROS 10

typedef struct{
    int x, y;
    int frame_atual;
    int contador_frame;
    int vel_x, vel_y;
    int direcao; // 0 = esquerda, 1 = direita, 2 = cima
    struct Tiro *proximo;
    int ativo;
} Tiro;

void disparar_tiro_personagem(Tiro tiros[], int *num_tiros, int x, int y, int direcao, float x_mouse, float y_mouse);
bool verificar_colisao_tiro_personagem(Tiro* tiro, Inimigo* inimigo, float fundo_x, float pixeis_andados, float pos_x_personagem);

void disparar_tiro_inimigos(Tiro tiros_inimigo[], int *num_tiros, int max_tiros, float x_personagem, float y_personagem, Inimigo inimigo);
bool verificar_colisao_tiro_inimigo(Tiro* tiro, float x_personagem, float y_personagem, float largura_personagem, float altura_personagem, float fundo_x, float pixeis_andados, bool agachando);
bool verificar_colisao_tiro_boss(Tiro* tiro, float x_personagem, float y_personagem, float largura_personagem, float altura_personagem, bool agachando);
void disparar_tiro_boss(Tiro tiros_boss[], int *num_tiros, int max_tiros, float x_personagem, float y_personagem, boss Boss);
bool verificar_colisao_tiro_personagem_boss(Tiro* tiro, boss Boss);
#endif
