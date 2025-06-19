#include <stdio.h>
#include "Joystick.h"	
#include "personagem.h"
#include "functions.h"
#include "tiros.h"
#include <math.h>

#define LIMITE_MAPA X_SCREEN * 12
EstadoJogo jogar(ALLEGRO_DISPLAY *janela, ALLEGRO_BITMAP *background, EstadoJogoDados *dados, Inimigo inimigos[MAX_INIMIGOS]) {
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 45.0);
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();

    // Constantes frames e duração das animações
    const int FRAMES_BOSS = 9;
    const int FRAMES_INIMIGO = 8;
    const int FRAMES_ANDANDO = 12;
    const int FRAMES_PULANDO = 4;
    const int FRAMES_AGACHANDO = 2;
    const int FRAMES_ATAQUE_NORMAL = 2;
    const int FRAMES_ATAQUE_BOSS = 6;

    const int FRAME_DURATION_ATAQUE_BOSS = 18;
    const int FRAME_DURATION_BOSS = 9;
    const int FRAME_DURATION_INIMIGO = 10;
    const int FRAME_DURATION_ANDANDO = 7;
    const int FRAME_DURATION_PULO = 21;
    const int FRAME_DURATION_AGACHANDO = 15;
    const int FRAME_DURATION_ATAQUE = 7; 

    // Carregamento dos sprites
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

    // Inicializa variáveis do estado do personagem
    float pixeis_andados = dados->pixeis_andados;
    int pos_x_personagem = dados->pos_x;
    int pos_y_personagem = dados->pos_y;
    int frame_atual = dados->frame_atual;
    int direcao = dados->direcao;
    bool andando = dados->andando;
    float fundo_x = dados->fundo_x;

    int contador_frame_andando = 0;
    int contador_frame_pulando = dados->contador_frame_pulando;
    int contador_frame_agachando = 0;
    int contador_frame_ataque = 0;

    int largura_fundo = al_get_bitmap_width(background);
    int altura_fundo = al_get_bitmap_height(background);

    const float VELOCIDADE_FUNDO = 32.0f;
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
    Boss.vida = 500;
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

    // Loop principal do jogo
    while (jogando && !sair) {
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_TIMER) {

            // MOVIMENTO E FUNDO
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
                                // Inicia batalha do boss
                                batalha_boss = true;
                                Boss.ativo = true;
                                Boss.vida = 500;
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
                                } else {
                                    andando = false;
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
            if (agachando) {
                contador_frame_agachando++;
                if (contador_frame_agachando >= FRAME_DURATION_AGACHANDO) {
                    contador_frame_agachando = 0;
                    frame_atual = (frame_atual + 1) % FRAMES_AGACHANDO;
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
            } else if (agachando) {
                sprite_atual = sprites_agachando[frame_atual % FRAMES_AGACHANDO];
            } else if (atacando) {
                sprite_atual = sprites_ataque_normal[frame_atual % FRAMES_ATAQUE_NORMAL];
            } else {
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
                int largura_barra = 200;
                int altura_barra = 20;
                int x_barra = 20;
                int y_barra = 20;

                float proporcao = (float)vida/ 100.0f;
                if (proporcao < 0) proporcao = 0;

                al_draw_filled_rectangle(x_barra, y_barra, x_barra + largura_barra, y_barra + altura_barra, al_map_rgb(50, 50, 50));

                al_draw_filled_rectangle(x_barra, y_barra, x_barra + largura_barra * proporcao, y_barra + altura_barra, al_map_rgb(255, 0, 0));

                al_draw_rectangle(x_barra, y_barra, x_barra + largura_barra, y_barra + altura_barra, al_map_rgb(255, 255, 255), 2);

        float delta_time = 1.0 / 45.0; // Sua taxa de FPS
        int x_inicial[MAX_INIMIGOS];
        for(int i = 0; i<MAX_INIMIGOS; i++){
            x_inicial[i] = inimigos[i].x;
        }
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            float pos_tela = x_inicial[i] - (fundo_x + pixeis_andados) + pos_x_personagem;

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

        atualizar_inimigos(inimigos, fundo_x, FRAME_DURATION_INIMIGO, FRAMES_INIMIGO, x_inicial);

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
                    vida -= 10;
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
            if (Boss.contador_frame >= 5) {
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
                    al_get_bitmap_width(sprite_tiro) / 2.0f,
                    al_get_bitmap_height(sprite_tiro) / 2.0f,
                    tiros_boss[i].x, tiros_boss[i].y,
                    angulo,
                    0
                );
            }
        }
        for (int i = 0; i < num_tiros_boss; i++ ) {
                if (verificar_colisao_tiro_boss(&tiros_boss[i], pos_x_personagem, pos_y_personagem, largura_personagem, altura_personagem, agachando)) {
                    tiros_boss[i].ativo = false;
                    vida -= 10;
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
                        return ESTADO_GAME_OVER;
                    }
                }

                if (!tiros[i].ativo) {
                    for (int j = i; j < num_tiros - 1; j++) {
                        tiros[j] = tiros[j + 1];
                    }
                    num_tiros--;
                } 
            }
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