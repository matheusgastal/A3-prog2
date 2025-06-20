#include <stdlib.h>
#include "personagem.h"
#include <math.h>
#include "telas.h"

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
    dados->vida = 5;
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
    dados->vida = 5;
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
