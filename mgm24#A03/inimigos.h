#ifndef INIMIGOS_H
#define INIMIGOS_H
#define MAX_INIMIGOS 10

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
    float x_inicial;
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


void inicializa_inimigos(Inimigo inimigos[MAX_INIMIGOS]);
void atualizar_inimigos(Inimigo inimigos[MAX_INIMIGOS], float fundo_x, int frames_duration, int frames_inimigo);

#endif