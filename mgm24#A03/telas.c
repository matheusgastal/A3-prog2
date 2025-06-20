#include <stdio.h>
#include "Joystick.h"	
#include "personagem.h"
#include "functions.h"
#include "tiros.h"
#include <math.h>
#include "inimigos.h"
#include "telas.h"

#define LIMITE_MAPA X_SCREEN * 12
EstadoJogo jogar(ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *background, EstadoJogoDados *dados, Inimigo inimigos[MAX_INIMIGOS]) {
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 45.0);
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();

    const int FRAMES_ATAQUE_AGACHANDO = 3;
    const int FRAMES_BOSS = 9;
    const int FRAMES_INIMIGO = 8;
    const int FRAMES_ANDANDO = 12;
    const int FRAMES_PULANDO = 4;
    const int FRAMES_AGACHANDO = 2;
    const int FRAMES_ATAQUE_NORMAL = 2;
    const int FRAMES_ATAQUE_BOSS = 6;

    const int FRAME_DURATION_ATAQUE_AGACHANDO = 15; 
    const int FRAME_DURATION_BOSS = 9;
    const int FRAME_DURATION_INIMIGO = 10;
    const int FRAME_DURATION_ANDANDO = 7;
    const int FRAME_DURATION_PULO = 21;
    const int FRAME_DURATION_AGACHANDO = 15;
    const int FRAME_DURATION_ATAQUE = 7;

    // Carregamento dos sprites
    ALLEGRO_BITMAP *sprites_ataque_agachando[FRAME_DURATION_ATAQUE_AGACHANDO];
    ALLEGRO_BITMAP *sprites_ataque_boss[FRAMES_ATAQUE_BOSS];
    ALLEGRO_BITMAP *sprites_boss[FRAMES_BOSS];
    ALLEGRO_BITMAP *sprites_ataque_normal[FRAMES_ATAQUE_NORMAL];
    ALLEGRO_BITMAP *sprites_andando[FRAMES_ANDANDO];
    ALLEGRO_BITMAP *sprites_pulando[FRAMES_PULANDO];
    ALLEGRO_BITMAP *sprites_agachando[FRAMES_AGACHANDO];
    ALLEGRO_BITMAP *sprites_ataque_pulando[FRAMES_PULANDO];
    ALLEGRO_BITMAP *sprites_inimigo[FRAMES_INIMIGO];
    ALLEGRO_BITMAP *projetil = al_load_bitmap("assets/projetil/hit1.png");

    ALLEGRO_BITMAP *projetil_inimigo[2];
    

    // Carrega sprites das animações — repetir o padrão para todas as arrays
    for(int i = 0; i<2; i++){
        char nome[64];
        sprintf(nome, "assets/projetil_inimigo/frame%d.png", i+1);
        projetil_inimigo[i] = al_load_bitmap(nome);
        if (!projetil_inimigo[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }
    for (int i = 0; i < FRAMES_ANDANDO; i++) {
        char nome[32];
        sprintf(nome, "assets/andando/frame%d.png", i + 1);
        sprites_andando[i] = al_load_bitmap(nome);
        if (!sprites_andando[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }
    for (int i = 0; i < FRAMES_PULANDO; i++) {
        char nome[32];
        sprintf(nome, "assets/pulando/Jump%d.png", i + 1);
        sprites_pulando[i] = al_load_bitmap(nome);
        if (!sprites_pulando[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }
    for (int i = 0; i < FRAMES_AGACHANDO; i++) {
        char nome[32];
        sprintf(nome, "assets/agachando/frame%d.png", i + 1);
        sprites_agachando[i] = al_load_bitmap(nome);
        if (!sprites_agachando[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }
    for (int i = 0; i < FRAMES_ATAQUE_NORMAL; i++) {
        char nome[32];
        sprintf(nome, "assets/ataque_normal/frame%d.png", i + 1);
        sprites_ataque_normal[i] = al_load_bitmap(nome);
        if (!sprites_ataque_normal[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }
    for (int i = 0; i < FRAMES_PULANDO; i++) {
        char nome[64];
        sprintf(nome, "assets/ataque_pulando/Jump_attack%d.png", i + 1);
        sprites_ataque_pulando[i] = al_load_bitmap(nome);
        if (!sprites_ataque_pulando[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }
    for (int i = 0; i < FRAMES_INIMIGO; i++) {
        char nome[64];
        sprintf(nome, "assets/inimigos/flying-eye-demon%d.png", i + 1);
        sprites_inimigo[i] = al_load_bitmap(nome);
        if (!sprites_inimigo[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }
    for (int i = 0; i < FRAMES_BOSS; i++) {
        char nome[64];
        sprintf(nome, "assets/boss/dragao%d.png", i + 1);
        sprites_boss[i] = al_load_bitmap(nome);
        if (!sprites_boss[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }

    for (int i = 0; i < FRAMES_ATAQUE_BOSS; i++) {
        char nome[64];
        sprintf(nome, "assets/projetil_boss/charged%d.png", i + 1);
        sprites_ataque_boss[i] = al_load_bitmap(nome);
        if (!sprites_ataque_boss[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }

     for (int i = 0; i < FRAMES_ATAQUE_AGACHANDO; i++) {
        char nome[64];
        sprintf(nome, "assets/ataque_agachando/attackcrouch%d.png", i + 1);
        sprites_ataque_agachando[i] = al_load_bitmap(nome);
        if (!sprites_ataque_agachando[i]) { printf("Erro ao carregar %s\n", nome); return ESTADO_SAIR; }
    }

    float pixeis_andados = dados->pixeis_andados;
    int pos_x_personagem = dados->pos_x;
    int pos_y_personagem = dados->pos_y;
    int frame_atual = dados->frame_atual;
    int direcao = dados->direcao;
    bool andando = dados->andando;
    float fundo_x = dados->fundo_x;
    
    int contador_frame_ataque_agachando = 0;
    int contador_frame_andando = 0;
    int contador_frame_pulando = dados->contador_frame_pulando;
    int contador_frame_agachando = 0;
    int contador_frame_ataque = 0;

    int largura_fundo = al_get_bitmap_width(background);
    int altura_fundo = al_get_bitmap_height(background);

    const float VELOCIDADE_FUNDO = 40.0f;
    int vida = dados->vida;
    float vel_y = dados->vel_y;
    const float GRAVIDADE = 0.3f;
    const float FORCA_PULO = -6.0f;
    bool no_chao = dados->no_chao;
    bool pulando = dados->pulando;
    bool agachando = false;
    bool atacando = false;  
    bool batalha_boss = false;

    
    boss Boss;
    Boss.ativo = true;
    Boss.vida = 100;
    Boss.destruido = false;
    Boss.frame_atual = 0;
    Boss.contador_frame = 0;
    Boss.x = 400;  
    Boss.y = 100;  
    Boss.vel_x = 2.0f;  
    Boss.largura = al_get_bitmap_width(sprites_boss[0]);
    Boss.altura =  al_get_bitmap_height(sprites_boss[0]);
    Boss.tempo_recarga_tiro = 45;

    // Registro de eventos
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_mouse_event_source());

    al_start_timer(timer);

    bool sair = false;
    bool jogando = true;
    ALLEGRO_EVENT evento;
    Tiro tiros[10];
    int num_tiros = 0;
    Tiro tiros_inimigo[10];
    int num_tiros_inimigo = 0;
    Tiro tiros_boss[10];
    int num_tiros_boss = 0;


    Coracao coracoes[NUM_CORACOES];
    for (int i = 0; i < NUM_CORACOES; i++) {
    coracoes[i].x_mundo = (i + 1) * DISTANCIA_ENTRE_CORACOES; // começa no 1000, 2000, 3000...
    coracoes[i].coletado = false;
}

    while (jogando && !sair) {
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_TIMER) {

            if (andando) {
                float velocidade = agachando ? VELOCIDADE_FUNDO / 2.0f : VELOCIDADE_FUNDO;
                float posicao_total = pixeis_andados + fundo_x;

                if (!batalha_boss) {
                    if (direcao == 1) { // Direita
                        if (pos_x_personagem < 300) {
                            pos_x_personagem += velocidade;
                            if (pos_x_personagem > 300)
                                pos_x_personagem = 300;
                        } else {
                            if (posicao_total + velocidade <= LIMITE_MAPA) {
                                fundo_x += velocidade;
                                if (fundo_x >= largura_fundo) {
                                    pixeis_andados += largura_fundo;
                                    fundo_x -= largura_fundo;
                                }
                            } else {
                                batalha_boss = true;
                                Boss.ativo = true;
                                Boss.vida = 100;
                                Boss.destruido = false;
                            }
                        }
                    } else if (direcao == 0) { // Esquerda
                        if (pos_x_personagem > 300) {
                            pos_x_personagem -= velocidade;
                            if (pos_x_personagem < 300)
                                pos_x_personagem = 300;
                        } else {
                            if (posicao_total - velocidade >= 0) {
                                fundo_x -= velocidade;
                                if (fundo_x < 0) {
                                    pixeis_andados -= largura_fundo;
                                    fundo_x += largura_fundo;
                                }
                            } else {
                                if (pos_x_personagem > 20) {
                                    pos_x_personagem -= velocidade;
                                    if (pos_x_personagem < 0)
                                        pos_x_personagem = 0;
                                } 
                            }
                        }
                    }

                } else {
                    // Movimento na batalha do boss
                    if (direcao == 1) {
                        if (pos_x_personagem < X_SCREEN - 100)
                            pos_x_personagem += velocidade;
                    } else if (direcao == 0) {
                        if (pos_x_personagem > 100)
                            pos_x_personagem -= velocidade;
                    }

                    // Trava o fundo no boss
                    float posicao_total = pixeis_andados + pos_x_personagem;
                    const float POSICAO_BOSS = LIMITE_MAPA;
                    if (posicao_total >= POSICAO_BOSS) {
                        fundo_x = 0;
                        pixeis_andados = POSICAO_BOSS - pos_x_personagem;
                    }
                }

                // Atualiza frame andando só se não atacando
                if (!atacando) {
                    contador_frame_andando++;
                    if (contador_frame_andando >= FRAME_DURATION_ANDANDO) {
                        contador_frame_andando = 0;
                        frame_atual = (frame_atual + 1) % FRAMES_ANDANDO;
                    }
                }
            } 
            else if (!pulando && !agachando && !atacando) {
                frame_atual = 0;
            }

            // ANIMAÇÃO DE ATAQUE
            if (atacando) {
                contador_frame_ataque++;
                if (contador_frame_ataque >= FRAME_DURATION_ATAQUE) {
                    contador_frame_ataque = 0;
                    frame_atual++;

                    if (pulando) {
                        if (frame_atual >= FRAMES_PULANDO) {
                            atacando = false;
                            frame_atual = 0;
                        }
                    } else {
                        if (frame_atual >= FRAMES_ATAQUE_NORMAL) {
                            atacando = false;
                            frame_atual = 0;
                        }
                    }
                }
            }

            // PULO E GRAVIDADE
            if (pulando && !no_chao) {
                vel_y += GRAVIDADE;
                pos_y_personagem += vel_y;
                contador_frame_pulando++;
                if (contador_frame_pulando >= FRAME_DURATION_PULO) {
                    contador_frame_pulando = 0;
                    frame_atual = (frame_atual + 1) % FRAMES_PULANDO;
                }

                if (pos_y_personagem >= dados->pos_y) {
                    pos_y_personagem = dados->pos_y;
                    vel_y = 0;
                    no_chao = true;
                    pulando = false;
                    frame_atual = 0;
                }
            }
            
            // AGACHAR ANIMAÇÃO
            if (agachando && !atacando){
                contador_frame_agachando++;
                if (contador_frame_agachando >= FRAME_DURATION_AGACHANDO) {
                    contador_frame_agachando = 0;
                    frame_atual = (frame_atual + 1) % FRAMES_AGACHANDO;
                }
            }

            if (agachando && atacando){
                contador_frame_ataque_agachando++;
                if (contador_frame_ataque_agachando >= FRAME_DURATION_ATAQUE_AGACHANDO) {
                    contador_frame_ataque_agachando = 0;
                    frame_atual = (frame_atual + 1) % FRAMES_ATAQUE_AGACHANDO;
                }
            }

            // DESENHO DO FUNDO (scrolling)
            int int_fundo_x = (int)fundo_x;
            al_draw_scaled_bitmap(background,
                int_fundo_x, 0,
                largura_fundo - int_fundo_x, altura_fundo,
                0, 0,
                largura_fundo - int_fundo_x, Y_SCREEN,
                0);

            al_draw_scaled_bitmap(background,
                0, 0,
                int_fundo_x, altura_fundo,
                largura_fundo - int_fundo_x, 0,
                int_fundo_x, Y_SCREEN,
                0);

            // Escolhe o sprite correto para o personagem
            ALLEGRO_BITMAP *sprite_atual;

            if (pulando && atacando) {
                sprite_atual = sprites_ataque_pulando[frame_atual % FRAMES_PULANDO];
            } else if (pulando) {
                sprite_atual = sprites_pulando[frame_atual % FRAMES_PULANDO];
            } else if (agachando && !atacando) {
                sprite_atual = sprites_agachando[frame_atual % FRAMES_AGACHANDO];
            } else if (atacando && !agachando) {
                sprite_atual = sprites_ataque_normal[frame_atual % FRAMES_ATAQUE_NORMAL];
            } 
            else if(atacando && agachando){
                sprite_atual = sprites_ataque_agachando[frame_atual % FRAMES_ATAQUE_NORMAL];
            }
            else {
                sprite_atual = sprites_andando[frame_atual % FRAMES_ANDANDO];
            }

            int sprite_w = al_get_bitmap_width(sprite_atual);
            int sprite_h = al_get_bitmap_height(sprite_atual);
            int pos_x = pos_x_personagem - PERSONAGEM_LADO / 2;
            int pos_y = pos_y_personagem - PERSONAGEM_LADO / 2;

            // Desenha personagem
            if (direcao == 0) {
                al_draw_scaled_bitmap(sprite_atual,
                    0, 0, sprite_w, sprite_h,
                    pos_x + PERSONAGEM_LADO, pos_y,
                    -PERSONAGEM_LADO, PERSONAGEM_LADO, 0);
            } else {
                al_draw_scaled_bitmap(sprite_atual,
                    0, 0, sprite_w, sprite_h,
                    pos_x, pos_y,
                    PERSONAGEM_LADO, PERSONAGEM_LADO, 0);
            }

            for (int i = 0; i < num_tiros; ) {
                tiros[i].x += tiros[i].vel_x;
                tiros[i].y += tiros[i].vel_y;
                if (tiros[i].x < 0 || tiros[i].x > X_SCREEN || tiros[i].y < 0 || tiros[i].y > Y_SCREEN || (direcao == 1 && tiros[i].x < pos_x_personagem) || (direcao == 0 && tiros[i].x > pos_x_personagem)) {
                    for (int j = i; j < num_tiros - 1; j++) tiros[j] = tiros[j + 1];
                    num_tiros--;
                } else {
                    al_draw_scaled_bitmap(projetil, 0, 0,
                        al_get_bitmap_width(projetil),
                        al_get_bitmap_height(projetil),
                        tiros[i].x, tiros[i].y, 32, 32, 0);
                    i++;
                }
            }

ALLEGRO_BITMAP *coracao = al_load_bitmap("assets/vida/coracao.png");
int vida_atual = vida;   

float escala_hud = 0.2;   
float escala_mapa = 0.13;  

// Tamanho para HUD
int largura_coracao_hud = (int)(al_get_bitmap_width(coracao) * escala_hud);
int altura_coracao_hud = (int)(al_get_bitmap_height(coracao) * escala_hud);

// Tamanho para mapa
int largura_coracao_mapa = (int)(al_get_bitmap_width(coracao) * escala_mapa);
int altura_coracao_mapa = (int)(al_get_bitmap_height(coracao) * escala_mapa);

int x_desenho = 20;
int y_desenho = 20;
int espacamento = 10;  // Espaço entre os corações no HUD

// Desenhar corações no HUD
for (int i = 0; i < vida_atual; i++) {
    int x = x_desenho + i * (largura_coracao_hud + espacamento);
    al_draw_scaled_bitmap(
        coracao,
        0, 0,
        al_get_bitmap_width(coracao),
        al_get_bitmap_height(coracao),
        x, y_desenho,
        largura_coracao_hud,
        altura_coracao_hud,
        0
    );
}

const int OFFSET_Y_MAPA = 90; // Ajuste para altura do chão

// Desenhar corações no mapa (menor e mais pra baixo)
for (int i = 0; i < NUM_CORACOES; i++) {
    if (!coracoes[i].coletado) {
        int pos_x_tela = coracoes[i].x_mundo - (int)(pixeis_andados + fundo_x) + pos_x_personagem;

        int y_coracao = CHAO - altura_coracao_mapa + OFFSET_Y_MAPA;

        if (pos_x_tela >= -largura_coracao_mapa && pos_x_tela <= X_SCREEN) {
            al_draw_scaled_bitmap(
                coracao,
                0, 0,
                al_get_bitmap_width(coracao),
                al_get_bitmap_height(coracao),
                pos_x_tela, y_coracao,
                largura_coracao_mapa,
                altura_coracao_mapa,
                0
            );
        }
    }
}

// Colisão com corações no mapa (usando tamanho menor)
for (int i = 0; i < NUM_CORACOES; i++) {
    if (!coracoes[i].coletado) {
        int pos_x_tela = coracoes[i].x_mundo - (int)(pixeis_andados + fundo_x) + pos_x_personagem;
        int y_coracao = CHAO - altura_coracao_mapa + OFFSET_Y_MAPA;

        int coracao_esq = pos_x_tela;
        int coracao_dir = pos_x_tela + largura_coracao_mapa;
        int coracao_top = y_coracao;
        int coracao_bot = y_coracao + altura_coracao_mapa;

        int personagem_esq = pos_x_personagem;
        int personagem_dir = pos_x_personagem + 64;
        int personagem_top = pos_y_personagem;
        int personagem_bot = pos_y_personagem + 64;

        if (personagem_dir > coracao_esq &&
            personagem_esq < coracao_dir &&
            personagem_bot > coracao_top &&
            personagem_top < coracao_bot) {

            coracoes[i].coletado = true;

            if (vida < 5) {
                vida++;
            }
        }
    }
}










        float delta_time = 1.0 / 45.0; // Sua taxa de FPS
    
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            float pos_tela = inimigos[i].x - (fundo_x + pixeis_andados) + pos_x_personagem;

            const float MARGEM_ATIVACAO = pos_x_personagem + 500;
            const float MARGEM_DESATIVACAO = pos_x_personagem - 500;

            if (!inimigos[i].destruido) {
                if (!inimigos[i].ativo && pos_tela > MARGEM_ATIVACAO) {
                    inimigos[i].ativo = true;
                    inimigos[i].tempo_recarga_tiro = 40.0; 
                }

                if (inimigos[i].ativo && pos_tela < MARGEM_DESATIVACAO) {
                    inimigos[i].ativo = false;
                }

                if (inimigos[i].ativo) {
                   al_draw_scaled_bitmap(
                    sprites_inimigo[inimigos[i].frame_atual],
                    0, 0,
                    al_get_bitmap_width(sprites_inimigo[0]),
                    al_get_bitmap_height(sprites_inimigo[0]),
                    pos_tela + inimigos[i].largura,  // Corrige a posição para o flip
                    inimigos[i].y,
                    -inimigos[i].largura,  // Largura negativa para flip horizontal
                    inimigos[i].altura,
                    0);

                    // Atualiza temporizador
                    inimigos[i].tempo_recarga_tiro -= delta_time;

                if (inimigos[i].tempo_recarga_tiro <= 0) {
                    disparar_tiro_inimigos(tiros_inimigo, &num_tiros_inimigo, 10, pos_x_personagem, pos_y_personagem, inimigos[i]);
                    inimigos[i].tempo_recarga_tiro = 40.0; // 2 segundos entre tiros
                }
                }
            }
        }


        
     for (int i = 0; i < num_tiros_inimigo; ) {
            tiros_inimigo[i].x += tiros_inimigo[i].vel_x;
            tiros_inimigo[i].y += tiros_inimigo[i].vel_y;

            float pos_tela = tiros_inimigo[i].x - pixeis_andados - fundo_x + pos_x_personagem;

            if (pos_tela < -50 || pos_tela > X_SCREEN + 50 || 
                tiros_inimigo[i].y < -50 || tiros_inimigo[i].y > CHAO + 100) 
            {
                // Remove tiro inativo
                for (int j = i; j < num_tiros_inimigo - 1; j++) 
                    tiros_inimigo[j] = tiros_inimigo[j + 1];
                num_tiros_inimigo--;
            } else {
                // Atualiza animação do tiro
                tiros_inimigo[i].contador_frame++;
                if (tiros_inimigo[i].contador_frame >= 5) { 
                    tiros_inimigo[i].contador_frame = 0;
                    tiros_inimigo[i].frame_atual++;
                    if (tiros_inimigo[i].frame_atual >= 2) {
                        tiros_inimigo[i].frame_atual = 0;
                    }
                }

                // Desenha o frame atual
                al_draw_scaled_bitmap(projetil_inimigo[tiros_inimigo[i].frame_atual], 
                    0, 0,
                    al_get_bitmap_width(projetil_inimigo[0]),
                    al_get_bitmap_height(projetil_inimigo[0]),
                    pos_tela, tiros_inimigo[i].y,
                    32, 32, 0);

                i++;
    }
}

        atualizar_inimigos(inimigos, fundo_x, FRAME_DURATION_INIMIGO, FRAMES_INIMIGO);

            for (int i = 0; i < num_tiros; i++) {
                bool atingiu = false;
                
                for (int j = 0; j < MAX_INIMIGOS; j++) {
                    if (!inimigos[j].ativo || inimigos[j].destruido) continue;
                    
                    if (verificar_colisao_tiro_personagem(&tiros[i], &inimigos[j], fundo_x, pixeis_andados, pos_x_personagem)) {
                        inimigos[j].ativo = false;
                        inimigos[j].destruido = true;
                        tiros[i].ativo = false;
                        atingiu = true;
                        break;
                    }
                }
                
                if (atingiu || !tiros[i].ativo) {
                    for (int k = i; k < num_tiros - 1; k++) {
                        tiros[k] = tiros[k + 1];
                    }
                    num_tiros--;
                } else {
                    i++;
                }
            }

            int largura_personagem = al_get_bitmap_width(sprite_atual);
            int altura_personagem = al_get_bitmap_height(sprite_atual);
            for (int i = 0; i < num_tiros_inimigo; i++ ) {
                if (verificar_colisao_tiro_inimigo(&tiros_inimigo[i], pos_x_personagem, pos_y_personagem, largura_personagem, altura_personagem, fundo_x, pixeis_andados, agachando)) {
                    tiros_inimigo[i].ativo = false;
                    vida -= 1;
                    if(vida <= 0){
                        return ESTADO_GAME_OVER;
                    }
                }

                if (!tiros_inimigo[i].ativo) {
                    for (int j = i; j < num_tiros_inimigo - 1; j++) {
                        tiros_inimigo[j] = tiros_inimigo[j + 1];
                    }
                    num_tiros_inimigo--;
                } else {
                    i++;
                }
            }
        float limite_esquerdo = 50;
        float limite_direito = X_SCREEN - 50;
        if (batalha_boss && Boss.ativo && !Boss.destruido) {
            
            Boss.x -= Boss.vel_x;

            if (Boss.x <= limite_esquerdo || Boss.x + Boss.largura >= limite_direito) {
                Boss.vel_x *= -1;
            }

            Boss.contador_frame++;
            if (Boss.contador_frame >= FRAME_DURATION_BOSS) {
                Boss.frame_atual++;
                Boss.contador_frame = 0;
            }
            if (Boss.frame_atual >= FRAMES_BOSS) {
                Boss.frame_atual = 0;
            }

            Boss.tempo_recarga_tiro -= 1;
            if (Boss.tempo_recarga_tiro <= 0) {
                disparar_tiro_boss(tiros_boss, &num_tiros_boss, MAX_TIROS, pos_x_personagem, pos_y_personagem, Boss);
                Boss.tempo_recarga_tiro = 45; 
            }

            if (Boss.vel_x < 0) {
                al_draw_scaled_bitmap(
                    sprites_boss[Boss.frame_atual],
                    0, 0,
                    al_get_bitmap_width(sprites_boss[0]),
                    al_get_bitmap_height(sprites_boss[0]),
                    Boss.x + Boss.largura, Boss.y,
                    -Boss.largura, Boss.altura,
                    0
                );
            } else {
                al_draw_scaled_bitmap(
                    sprites_boss[Boss.frame_atual],
                    0, 0,
                    al_get_bitmap_width(sprites_boss[0]),
                    al_get_bitmap_height(sprites_boss[0]),
                    Boss.x, Boss.y,
                    Boss.largura, Boss.altura,
                    0
                );
            }
        if(num_tiros_boss > 0)
        for (int i = 0; i < num_tiros_boss; i++) {
            tiros_boss[i].x += tiros_boss[i].vel_x;
            tiros_boss[i].y += tiros_boss[i].vel_y;
            if (tiros_boss[i].x < -50 || tiros_boss[i].x > X_SCREEN + 50 || 
                tiros_boss[i].y < -50 || tiros_boss[i].y > CHAO + 100)  {
                tiros_boss[i] = tiros_boss[num_tiros_boss - 1];
                num_tiros_boss--;
                i--; 
            } else {
                tiros_boss[i].contador_frame++;
                if (tiros_boss[i].contador_frame >= 5) {
                    tiros_boss[i].contador_frame = 0;
                    tiros_boss[i].frame_atual = (tiros_boss[i].frame_atual + 1) % 6;
                }
                float angulo = atan2f(tiros_boss[i].vel_y, tiros_boss[i].vel_x);
                ALLEGRO_BITMAP* sprite_tiro = sprites_ataque_boss[tiros_boss[i].frame_atual];
                
                al_draw_rotated_bitmap(
                    sprite_tiro,
                    al_get_bitmap_width(sprite_tiro) / 2.0,
                    al_get_bitmap_height(sprite_tiro) / 2.0,
                    tiros_boss[i].x, tiros_boss[i].y,
                    angulo,
                    0
                );
            }
        }
        for (int i = 0; i < num_tiros_boss; i++ ) {
                if (verificar_colisao_tiro_boss(&tiros_boss[i], pos_x_personagem, pos_y_personagem, largura_personagem, altura_personagem, agachando)) {
                    tiros_boss[i].ativo = false;
                    vida -= 1;
                    if(vida <= 0){
                        return ESTADO_GAME_OVER;
                    }
                }

                if (!tiros_boss[i].ativo) {
                    for (int j = i; j < num_tiros_boss - 1; j++) {
                        tiros_boss[j] = tiros_boss[j + 1];
                    }
                    num_tiros_boss--;
                } 
            }
          for (int i = 0; i < num_tiros; i++ ) {
                if (verificar_colisao_tiro_personagem_boss(tiros, Boss)) {
                    tiros[i].ativo = false;
                    Boss.vida -= 10;
                    if(Boss.vida <= 0){
                        return ESTADO_WIN;
                    }
                }

                if (!tiros[i].ativo) {
                    for (int j = i; j < num_tiros - 1; j++) {
                        tiros[j] = tiros[j + 1];
                    }
                    num_tiros--;
                } 
            }
            int total_blocos = 10;
            int blocos_preenchidos = Boss.vida / 10;

            float largura_bloco = 20;
            float altura_bloco = 10;
            float espacamento = 5;
            float x_inicial = X_SCREEN - (total_blocos * (largura_bloco + espacamento)) + espacamento - 20;
            float y_inicial = 20;

            for (int i = 0; i < total_blocos; i++) {
                if (i < blocos_preenchidos) {
                    al_draw_filled_rectangle(
                        x_inicial + i * (largura_bloco + espacamento),
                        y_inicial,
                        x_inicial + i * (largura_bloco + espacamento) + largura_bloco,
                        y_inicial + altura_bloco,
                        al_map_rgb(255, 0, 0) 
                    );
                } else {
                    al_draw_rectangle(
                        x_inicial + i * (largura_bloco + espacamento),
                        y_inicial,
                        x_inicial + i * (largura_bloco + espacamento) + largura_bloco,
                        y_inicial + altura_bloco,
                        al_map_rgb(100, 100, 100), 
                        2
                    );
                }
            }
            al_draw_rectangle(
                x_inicial - espacamento / 2,
                y_inicial - espacamento / 2,
                x_inicial + total_blocos * (largura_bloco + espacamento) - espacamento / 2,
                y_inicial + altura_bloco + espacamento / 2,
                al_map_rgb(255, 255, 255), 
                3 // Espessura da borda
            );

        }

            al_flip_display();
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ((direcao == 1 && evento.mouse.x >= pos_x_personagem) || (direcao == 0 && evento.mouse.x <= pos_x_personagem))) {
            if (evento.mouse.button == 1) { 
                atacando = true;
                frame_atual = 0;
                contador_frame_ataque = 0;
                disparar_tiro_personagem(tiros, &num_tiros, pos_x_personagem, pos_y_personagem, direcao, evento.mouse.x, evento.mouse.y);
                }
            }   
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_D) {
                andando = true;
                direcao = 1;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_A) {
                andando = true;
                direcao = 0;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                jogando = false;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_W && no_chao) {
                vel_y = FORCA_PULO;
                no_chao = false;
                pulando = true;
                contador_frame_pulando = 0;
                frame_atual = 0;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_S && no_chao) {
                agachando = true;
                frame_atual = 0;
                contador_frame_agachando = 0;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_H) {
                    dados->vida -= 10;
                    if (dados->vida < 0) dados->vida = 0;
                    if(dados->vida == 0){
                        return ESTADO_GAME_OVER;
                    }
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            if ((evento.keyboard.keycode == ALLEGRO_KEY_D && direcao == 1) ||
                (evento.keyboard.keycode == ALLEGRO_KEY_A && direcao == 0)) {
                andando = false;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_S) {
                agachando = false;
                frame_atual = 0;
            }
        }

        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true;
        }
    }

    // Libera recursos
    for (int i = 0; i < FRAMES_ANDANDO; i++) al_destroy_bitmap(sprites_andando[i]);
    for (int i = 0; i < FRAMES_PULANDO; i++) al_destroy_bitmap(sprites_pulando[i]);
    for (int i = 0; i < FRAMES_AGACHANDO; i++) al_destroy_bitmap(sprites_agachando[i]);
    for (int i = 0; i < FRAMES_ATAQUE_NORMAL; i++) al_destroy_bitmap(sprites_ataque_normal[i]);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);

    if (sair) {
        return ESTADO_SAIR;
    } else {
        dados->andando = andando;
        dados->direcao = direcao;
        dados->frame_atual = frame_atual;
        dados->fundo_x = fundo_x;
        dados->pos_x = pos_x_personagem;
        if (no_chao) dados->pos_y = pos_y_personagem;
        dados->vel_y = vel_y;
        dados->no_chao = no_chao;
        dados->pulando = pulando;
        dados->contador_frame_pulando = contador_frame_pulando;
        dados->agachando = agachando;
        dados->atacando = atacando;
        dados->contador_frame_ataque = contador_frame_ataque;
        return ESTADO_PAUSE;
    }
}




EstadoJogo menu_principal(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background) {
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_display_event_source(janela));
    ALLEGRO_EVENT evento;

    int opcao_selecionada = 0; 
    const int total_opcoes = 2;

    while (1) {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        al_draw_scaled_bitmap(
			background,
			0, 0,
			al_get_bitmap_width(background),
			al_get_bitmap_height(background),
			0, 0,
			X_SCREEN,
			Y_SCREEN,
				0
			);


        desenha_texto_com_borda(fonte_titulo, 400,220, ALLEGRO_ALIGN_CENTER, "Castle of Shadows", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));

        if (opcao_selecionada == 0) {
        desenha_texto_com_borda(fonte_opcao, 400, 300, ALLEGRO_ALIGN_CENTER, "Play", al_map_rgb(255, 255, 0), al_map_rgb(0, 0, 0));
        desenha_texto_com_borda(fonte_opcao, 400, 360, ALLEGRO_ALIGN_CENTER, "Quit", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));
        } 
        else {
        desenha_texto_com_borda(fonte_opcao, 400, 300, ALLEGRO_ALIGN_CENTER, "Play", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));
        desenha_texto_com_borda(fonte_opcao, 400, 360, ALLEGRO_ALIGN_CENTER, "Quit", al_map_rgb(255, 255, 0), al_map_rgb(0, 0, 0));
        }

        al_flip_display();

        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_W) {
                opcao_selecionada--;
                if (opcao_selecionada < 0) opcao_selecionada = total_opcoes - 1;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_S) {
                opcao_selecionada++;
                if (opcao_selecionada >= total_opcoes) opcao_selecionada = 0;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                al_destroy_event_queue(fila);
                return (opcao_selecionada == 0) ? ESTADO_JOGO : ESTADO_SAIR;
            }
        } else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_event_queue(fila);
            return ESTADO_SAIR;
        }
    }
}


EstadoJogo PAUSE(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background){
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_display_event_source(janela));
    ALLEGRO_EVENT evento;

    int opcao_selecionada = 0; 
    const int total_opcoes = 2;

    while (1) {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        al_draw_scaled_bitmap(
			background,
			0, 0,
			al_get_bitmap_width(background),
			al_get_bitmap_height(background),
			0, 0,
			X_SCREEN,
			Y_SCREEN,
				0
			);


        desenha_texto_com_borda(fonte_titulo, 400,220, ALLEGRO_ALIGN_CENTER, "Castle of Shadows", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));

        if (opcao_selecionada == 0) {
        desenha_texto_com_borda(fonte_opcao, 400, 300, ALLEGRO_ALIGN_CENTER, "Return to game", al_map_rgb(255, 255, 0), al_map_rgb(0, 0, 0));
        desenha_texto_com_borda(fonte_opcao, 400, 360, ALLEGRO_ALIGN_CENTER, "Back to menu", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));
        } 
        else {
        desenha_texto_com_borda(fonte_opcao, 400, 300, ALLEGRO_ALIGN_CENTER, "Return to game", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));
        desenha_texto_com_borda(fonte_opcao, 400, 360, ALLEGRO_ALIGN_CENTER, "Back to menu", al_map_rgb(255, 255, 0), al_map_rgb(0, 0, 0));
        }

        al_flip_display();

        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_W) {
                opcao_selecionada--;
                if (opcao_selecionada < 0) opcao_selecionada = total_opcoes - 1;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_S) {
                opcao_selecionada++;
                if (opcao_selecionada >= total_opcoes) opcao_selecionada = 0;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                al_destroy_event_queue(fila);
                return (opcao_selecionada == 0) ? ESTADO_JOGO : ESTADO_MENU;
            }
        } else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_event_queue(fila);
            return ESTADO_SAIR;
        }
    }
}

EstadoJogo game_over(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background, EstadoJogoDados *dados) {
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_display_event_source(janela));
    ALLEGRO_EVENT evento;

    int opcao_selecionada = 0; 
    const int total_opcoes = 2;

    while (1) {
        al_clear_to_color(al_map_rgb(0, 0, 0));


        al_draw_scaled_bitmap(
			background,
			0, 0,
			al_get_bitmap_width(background),
			al_get_bitmap_height(background),
			0, 0,
			X_SCREEN,
			Y_SCREEN,
				0
			);

        desenha_texto_com_borda(fonte_titulo, 400,220, ALLEGRO_ALIGN_CENTER, "GAME OVER", al_map_rgb(255,0,0), al_map_rgb(0, 0, 0));

        if (opcao_selecionada == 0) {
        desenha_texto_com_borda(fonte_opcao, 400, 300, ALLEGRO_ALIGN_CENTER, "Play again", al_map_rgb(255, 255, 0), al_map_rgb(0, 0, 0));
        desenha_texto_com_borda(fonte_opcao, 400, 360, ALLEGRO_ALIGN_CENTER, "Quit", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));
        } 
        else {
        desenha_texto_com_borda(fonte_opcao, 400, 300, ALLEGRO_ALIGN_CENTER, "Play again", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));
        desenha_texto_com_borda(fonte_opcao, 400, 360, ALLEGRO_ALIGN_CENTER, "Quit", al_map_rgb(255, 255, 0), al_map_rgb(0, 0, 0));
        }

        al_flip_display();

        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_W) {
                opcao_selecionada--;
                if (opcao_selecionada < 0) opcao_selecionada = total_opcoes - 1;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_S) {
                opcao_selecionada++;
                if (opcao_selecionada >= total_opcoes) opcao_selecionada = 0;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                al_destroy_event_queue(fila);
                if (opcao_selecionada == 0) {
                    resetar_dados(dados);
                    return ESTADO_JOGO;
            }
                else{
                    return ESTADO_SAIR;
                }
        } else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_event_queue(fila);
            return ESTADO_SAIR;
        }
    }
}
}

EstadoJogo win(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte_titulo, ALLEGRO_FONT *fonte_opcao, ALLEGRO_BITMAP *background) {
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_display_event_source(janela));
    ALLEGRO_EVENT evento;

    int opcao_selecionada = 0; 
    const int total_opcoes = 2;

    while (1) {
        al_clear_to_color(al_map_rgb(0, 0, 0));


        al_draw_scaled_bitmap(
			background,
			0, 0,
			al_get_bitmap_width(background),
			al_get_bitmap_height(background),
			0, 0,
			X_SCREEN,
			Y_SCREEN,
				0
			);


        desenha_texto_com_borda(fonte_titulo, 400,220, ALLEGRO_ALIGN_CENTER, "Congratulations, you won!", al_map_rgb(0,255,0), al_map_rgb(0, 0, 0));

        if (opcao_selecionada == 0) {
        desenha_texto_com_borda(fonte_opcao, 400, 300, ALLEGRO_ALIGN_CENTER, "Back to Menu", al_map_rgb(255, 255, 0), al_map_rgb(0, 0, 0));
        desenha_texto_com_borda(fonte_opcao, 400, 360, ALLEGRO_ALIGN_CENTER, "Quit", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));
        } 
        else {
        desenha_texto_com_borda(fonte_opcao, 400, 300, ALLEGRO_ALIGN_CENTER, "Back to Menu", al_map_rgb(255, 255, 255), al_map_rgb(0, 0, 0));
        desenha_texto_com_borda(fonte_opcao, 400, 360, ALLEGRO_ALIGN_CENTER, "Quit", al_map_rgb(255, 255, 0), al_map_rgb(0, 0, 0));
        }

        al_flip_display();

        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_W) {
                opcao_selecionada--;
                if (opcao_selecionada < 0) opcao_selecionada = total_opcoes - 1;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_S) {
                opcao_selecionada++;
                if (opcao_selecionada >= total_opcoes) opcao_selecionada = 0;
            } else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                al_destroy_event_queue(fila);
                if (opcao_selecionada == 0) {
                    return ESTADO_MENU;
            }
                else{
                    return ESTADO_SAIR;
                }
        } else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_event_queue(fila);
            return ESTADO_SAIR;
        }
    }
}
}