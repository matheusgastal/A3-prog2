#ifndef TELAS_H
#define TELAS_H
#include "personagem.h"
#include "inimigos.h"

// === DEFINIÇÕES DE TELA ===
#define X_SCREEN 800
#define Y_SCREEN 600
#define PERSONAGEM_LADO (Y_SCREEN / 3)
#define CHAO (Y_SCREEN - (PERSONAGEM_LADO / 2) - 60)
#define VELOCIDADE 50
#define DISTANCIA_ENTRE_CORACOES 2000
#define NUM_CORACOES (LIMITE_MAPA / DISTANCIA_ENTRE_CORACOES)

// === ENUM DE ESTADO DO JOGO ===
typedef enum {
    ESTADO_MENU,
    ESTADO_JOGO,
    ESTADO_SAIR,
    ESTADO_PAUSE,
    ESTADO_GAME_OVER,
    ESTADO_WIN
} EstadoJogo;


EstadoJogo PAUSE(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background);
EstadoJogo menu_principal(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background);
EstadoJogo jogar(ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *background, EstadoJogoDados *dados, Inimigo inimigos[MAX_INIMIGOS]);
EstadoJogo game_over(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background, EstadoJogoDados *dados);
EstadoJogo win(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background);
#endif	