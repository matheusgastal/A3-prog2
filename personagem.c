#include <stdlib.h>
#include "personagem.h"
#include <math.h>
#define VELOCIDADE_INIMIGO 1.5



void inicializa_inimigos(Inimigo inimigos[MAX_INIMIGOS]) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        inimigos[i].x = 1500 + (i * 800);  // Aumente a distância inicial
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




void atualizar_inimigos(Inimigo inimigos[MAX_INIMIGOS], float fundo_x, int frames_duration, int frames_inimigo, int x_inicial[MAX_INIMIGOS]) {

    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (!inimigos[i].ativo)
            continue;

        float limite_superior = 300;
        float limite_inferior = 400;
        float limite_esquerdo = x_inicial[i] - 100;
        float limite_direito = x_inicial[i] + 100;

        // Movimento vertical
        if (inimigos[i].direcao_vertical == 1) {
            inimigos[i].y += rand() % 2 + 1;
            if (inimigos[i].y >= limite_inferior) {
                inimigos[i].direcao_vertical = -1;
            }
        } else {
            inimigos[i].y -= rand() % 2 + 1;
            if (inimigos[i].y <= limite_superior) {
                inimigos[i].direcao_vertical = 1;
            }
        }

        // Movimento horizontal
        if (inimigos[i].direcao_horizontal == 1) {
            inimigos[i].x += rand() % 2 + 1;
            if (inimigos[i].x >= limite_direito) {
                inimigos[i].direcao_horizontal = -1;
            }
        } else {
            inimigos[i].x -= rand() % 2 + 1;
            if (inimigos[i].x <= limite_esquerdo) {
                inimigos[i].direcao_horizontal = 1;
            }
        }

        // Atualiza animação
        inimigos[i].contador_frame++;
        if (inimigos[i].contador_frame >= frames_duration) {
            inimigos[i].contador_frame = 0;
            inimigos[i].frame_atual = (inimigos[i].frame_atual + 1) % frames_inimigo;
        }
    }
}





personagem* cria_personagem(int lado, int x, int y, int max_x, int max_y) {					
	if ((x - lado/2 < 0) || (x + lado/2 > max_x) || (y - lado/2 < 0) || (y + lado/2 > max_y)) return NULL;										//Verifica se a posição inicial é válida; caso não seja, retorna NULL

	personagem *novo_personagem = (personagem*) malloc(sizeof(personagem));																						//Aloca memória na heap para um novo quadrado
	novo_personagem->lado = lado;																													//Insere o tamanho do lado de um quadrado
	novo_personagem->x = x;																															//Insere a posição inicial central de X
	novo_personagem->y = y;																															//Insere a posição inicial central de Y
	novo_personagem->control = joystick_create();																									//Insere o elemento de controle do quadrado (!)
	return novo_personagem;																															//Retorna o novo quadrado
}

void resetar_dados(EstadoJogoDados *dados) {
    dados->pixeis_andados = 0;
    dados->agachando = false;
    dados->pos_x = 300;
    dados->pos_y = CHAO;
    dados->frame_atual = 0;
    dados->direcao = 1;
    dados->andando = false;
    dados->vida = 100;
    dados->fundo_x = 0.0f;
    dados->vel_y = 0.0f;
    dados->no_chao = true;
    dados->pulando = false;
    dados->contador_frame_pulando = 0;
    dados->contador_frame_ataque = 0;
    dados->atacando = false;
}


EstadoJogoDados* inicia_dados(){
    EstadoJogoDados *dados = (EstadoJogoDados*)malloc(sizeof(EstadoJogoDados));
    dados->agachando = false;
    dados->pos_x = 300;
    dados->pos_y = CHAO;
    dados->frame_atual = 0;
    dados->pixeis_andados = 0;
    dados->direcao = 1;
    dados->vida = 100;
    dados->andando = false;
    dados->fundo_x = 0.0f;
    dados->vel_y = 0.0f;
    dados->no_chao = true;
    dados->pulando = false;
    dados->contador_frame_pulando = 0;
    dados->atacando = false;
    dados->contador_frame_ataque = 0;
    return dados;
}




void destroi_personagem(personagem *p) {
    if (p) free(p);
}
