#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#define MAX_INIMIGOS 10

// === INCLUDES ===
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "Joystick.h"

// === DEFINIÇÕES DE TELA ===
#define X_SCREEN 800
#define Y_SCREEN 600
#define PERSONAGEM_LADO (Y_SCREEN / 3)
#define CHAO (Y_SCREEN - (PERSONAGEM_LADO / 2) - 60)
#define VELOCIDADE 50

// === ENUM DE ESTADO DO JOGO ===
typedef enum {
    ESTADO_MENU,
    ESTADO_JOGO,
    ESTADO_SAIR,
    ESTADO_PAUSE,
    ESTADO_GAME_OVER
} EstadoJogo;


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
    int vida;
    bool ativo; 
    int frame_atual;
    int contador_frame;
    bool destruido;
    float x, y;
    float largura, altura;
    float vel_x;
    float tempo_recarga_tiro;
} boss;

typedef struct {
    float x, y;
    int direcao_vertical;
    int direcao_horizontal;
    bool ativo;
    int frame_atual;
    int contador_frame;
    bool destruido;
    float largura;  
    float altura;  
    int tempo_recarga_tiro;
} Inimigo;

// === FUNÇÕES DO PERSONAGEM ===
personagem* cria_personagem(int lado, int x, int y, int max_x, int max_y);
void destroi_personagem(personagem *p);
void inicializa_inimigos(Inimigo inimigos[MAX_INIMIGOS]);
void cria_inimigo(Inimigo inimigos[MAX_INIMIGOS]);
EstadoJogoDados* inicia_dados();
void resetar_dados(EstadoJogoDados *dados);
void atualizar_inimigos(Inimigo inimigos[MAX_INIMIGOS], float fundo_x, int frames_duration, int frames_inimigo, int x_inicial[MAX_INIMIGOS]);


#endif