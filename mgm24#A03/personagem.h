#ifndef PERSONAGEM_H
#define PERSONAGEM_H

// === INCLUDES ===
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "Joystick.h"



// === DADOS GLOBAIS DO ESTADO DO JOGO ===
typedef struct {
    float pixeis_andados;
    int pos_x;
    int pos_y;
    int frame_atual;
    int direcao;
    int vida;
    bool andando;
    float fundo_x;
    float vel_y;
    bool no_chao;
    bool pulando;
    bool agachando;
    int contador_frame_pulando;
    bool atacando;
    int contador_frame_ataque;
} EstadoJogoDados;

// === ESTRUTURA DO PERSONAGEM ===
typedef struct {
    int x, y;
    int lado;
    joystick *control;
} personagem;



typedef struct {
    int x_mundo;
    bool coletado;
} Coracao;


// === FUNÇÕES DO PERSONAGEM ===
personagem* cria_personagem(int lado, int x, int y, int max_x, int max_y);
void destroi_personagem(personagem *p);
EstadoJogoDados* inicia_dados();
void resetar_dados(EstadoJogoDados *dados);



#endif