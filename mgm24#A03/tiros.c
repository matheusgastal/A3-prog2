#include "tiros.h"
#include <stdlib.h>
#include "personagem.h"
#include <math.h>
#include "inimigos.h"
#include "telas.h"
void disparar_tiro_personagem(Tiro tiros[], int *num_tiros, int x, int y, int direcao, float x_mouse, float y_mouse) {
    if (*num_tiros >= 10) return;
    float dx = x_mouse - x;
    float dy = y_mouse - y;
    float dist = sqrt(dx*dx + dy*dy);
    if (dist == 0) dist = 1; 

    tiros[*num_tiros].x = x;
    tiros[*num_tiros].y = y;
    
    // Velocidade constante independente da direção do personagem
    float velocidade = 10.0f;
    tiros[*num_tiros].vel_x = (dx/dist) * velocidade;
    tiros[*num_tiros].vel_y = (dy/dist) * velocidade;
    
    tiros[*num_tiros].direcao = direcao; // Mantém para possível uso futuro
    tiros[*num_tiros].ativo = true;

    (*num_tiros)++;
}

void disparar_tiro_inimigos(Tiro tiros_inimigo[], int *num_tiros, int max_tiros, float x_personagem, float y_personagem, Inimigo inimigo) {
    if (*num_tiros >= max_tiros) return;

    float dx = x_personagem - inimigo.x;
    float dy = y_personagem + 700 - inimigo.y;
    float dist = sqrt(dx * dx + dy * dy);
    if (dist == 0) dist = 1;

    tiros_inimigo[*num_tiros].x = inimigo.x;
    tiros_inimigo[*num_tiros].y = inimigo.y;

    float velocidade = 9.0f;
    tiros_inimigo[*num_tiros].vel_x = (dx / dist) * velocidade;
    tiros_inimigo[*num_tiros].vel_y = (dy / dist) * velocidade;

    tiros_inimigo[*num_tiros].ativo = true;
    tiros_inimigo[*num_tiros].frame_atual = 0;
    tiros_inimigo[*num_tiros].contador_frame = 0;
    (*num_tiros)++;
}



void atualizar_tiros_array(Tiro tiros[], int *num_tiros) {
    for (int i = 0; i < *num_tiros; ) {
        if (!tiros[i].ativo) {
            tiros[i] = tiros[*num_tiros - 1];
            (*num_tiros)--;
            continue; 
        }
        tiros[i].x += tiros[i].vel_x;
        tiros[i].y += tiros[i].vel_y;

        if (tiros[i].x < 0 || tiros[i].x > X_SCREEN || tiros[i].y < 0 || tiros[i].y > Y_SCREEN) {
            tiros[i].ativo = false;
            continue; 
        }

        i++; 
    }
}


bool verificar_colisao_tiro_personagem(Tiro* tiro, Inimigo* inimigo, float fundo_x, float pixeis_andados, float pos_x_personagem) {
    // Verificação básica de estado
    if (!tiro->ativo || !inimigo->ativo) return false;

    // Calcula posição real do inimigo na tela
    float inimigo_x = inimigo->x - (fundo_x + pixeis_andados) + pos_x_personagem;
    float inimigo_y = inimigo->y;


    float tiro_esq = tiro->x - 16;  
    float tiro_dir = tiro->x + 16;
    float tiro_top = tiro->y - 16;
    float tiro_btm = tiro->y + 16;

    float inimigo_esq = inimigo_x;
    float inimigo_dir = inimigo_x + inimigo->largura;
    float inimigo_top = inimigo_y;
    float inimigo_btm = inimigo_y + inimigo->altura;

    // Detecção AABB (Axis-Aligned Bounding Box)
    return (tiro_dir > inimigo_esq) &&
           (tiro_esq < inimigo_dir) &&
           (tiro_btm > inimigo_top) &&
           (tiro_top < inimigo_btm);
}



bool verificar_colisao_tiro_inimigo(Tiro* tiro, float x_personagem, float y_personagem, float largura_personagem, float altura_personagem, float fundo_x, float pixeis_andados, bool agachando) {
    if (!tiro->ativo) return false;
    if(agachando){
        y_personagem += 30;
        altura_personagem -= 30;
    }
    float tiro_x = tiro->x - (fundo_x + pixeis_andados) + x_personagem;
    float tiro_y = tiro->y;

    float tiro_esq = tiro_x - 16;
    float tiro_dir = tiro_x + 16;
    float tiro_top = tiro_y - 16;
    float tiro_btm = tiro_y + 16;

    float personagem_esq = x_personagem - (largura_personagem / 4) - 5;
    float personagem_dir = x_personagem + (largura_personagem / 4) - 5;
    float personagem_top = y_personagem + 10;
    float personagem_btm = y_personagem + altura_personagem;
    return (tiro_dir > personagem_esq) &&
       (tiro_esq < personagem_dir) &&
       (tiro_btm > personagem_top) &&
       (tiro_top < personagem_btm);
}



void disparar_tiro_boss(Tiro tiros_boss[], int *num_tiros, int max_tiros, float x_personagem, float y_personagem, boss Boss) {
    if (*num_tiros >= max_tiros) return;
    float origem_x = Boss.x + Boss.largura / 2;
    float origem_y = Boss.y + Boss.altura;

    float dx = x_personagem - origem_x;

    float velocidade_vertical = 9;
    float fator_inclinacao = 90.0;
    float velocidade_horizontal = dx / fator_inclinacao;
    

    tiros_boss[*num_tiros].x = origem_x;
    tiros_boss[*num_tiros].y = origem_y;
    tiros_boss[*num_tiros].vel_x = velocidade_horizontal;
    tiros_boss[*num_tiros].vel_y = velocidade_vertical;
    tiros_boss[*num_tiros].ativo = true;

    tiros_boss[*num_tiros].frame_atual = 0;
    tiros_boss[*num_tiros].contador_frame = 0;

    (*num_tiros)++;
}

bool verificar_colisao_tiro_boss(Tiro* tiro, float x_personagem, float y_personagem, float largura_personagem, float altura_personagem, bool agachando) {
    if (!tiro->ativo) return false;

    float personagem_top = y_personagem + 10;
    float personagem_altura = altura_personagem;

    if (agachando) {
        personagem_top += 30;
        personagem_altura -= 30;
    }

    float personagem_esq = x_personagem - (largura_personagem / 4) - 5;
    float personagem_dir = x_personagem + (largura_personagem / 4) - 5;
    float personagem_btm = y_personagem + altura_personagem;
    // Limites do tiro (supondo tiro com hitbox 32x32)
    float tiro_esq = tiro->x - 16;
    float tiro_dir = tiro->x + 16;
    float tiro_top = tiro->y - 16;
    float tiro_btm = tiro->y + 16;

    // Verifica colisão AABB
    bool colisao = (tiro_dir > personagem_esq) &&
                   (tiro_esq < personagem_dir) &&
                   (tiro_btm > personagem_top) &&
                   (tiro_top < personagem_btm);
    return colisao;
}

bool verificar_colisao_tiro_personagem_boss(Tiro* tiro, boss Boss) {
    // Verificação básica de estado
    if (!tiro->ativo || !Boss.ativo) return false;


    float tiro_esq = tiro->x - 12;  
    float tiro_dir = tiro->x + 12;
    float tiro_top = tiro->y - 12;
    float tiro_btm = tiro->y + 12;

    float Boss_esq = Boss.x + 10;
    float Boss_dir = Boss.x + Boss.largura - 10;
    float Boss_top = Boss.y + 40;
    float Boss_btm = Boss.y + Boss.altura - 40;

    // Detecção AABB (Axis-Aligned Bounding Box)
    return (tiro_dir > Boss_esq) &&
           (tiro_esq < Boss_dir) &&
           (tiro_btm > Boss_top) &&
           (tiro_top < Boss_btm);
}
