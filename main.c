/*
 * main.c
 *
 * Ponto de entrada principal do jogo.
 * Controla o loop de jogo, a máquina de estados (o que está
 * acontecendo agora, ex: turno do jogador) e o loop de eventos
 * (entrada do teclado, timer).
 */

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Necessário para o srand()

// --- NOSSOS ARQUIVOS DE CABEÇALHO ---
#include "constants.h"
#include "renderer.h"
#include "utils.h"
#include "game_structs.h" // Nossas structs (Player, Enemy...)
#include "setup.h"        // Nossas funções (setup_player, create_enemy...)

// --- MÁQUINA DE ESTADOS DO JOGO ---
// Usamos um 'enum' para definir todos os estados possíveis do jogo.
// Isso controla o fluxo principal do programa.
typedef enum {
    GAME_STATE_START,      // Estado inicial, configura o jogador
    GAME_STATE_NEW_COMBAT, // Configura um novo combate (cria inimigos, embaralha)
    GAME_STATE_PLAYER_TURN,  // Esperando o jogador agir (jogar cartas, etc.)
    GAME_STATE_ENEMY_TURN,   // Inimigos executam suas ações
    GAME_STATE_VICTORY,      // O jogador venceu os 10 combates
    GAME_STATE_GAME_OVER     // O HP do jogador chegou a 0
} GameState;

// --- FUNÇÕES DE LÓGICA DE TURNO (Protótipos) ---
// Para manter o main.c limpo, declaramos as funções aqui
// e podemos implementá-las mais abaixo ou em um novo arquivo (ex: combat.c).

// Configura o início de um novo combate
void setup_new_combat(Player* player, Enemy inimigos[]);

// Configura o início do turno do jogador (compra cartas, reseta energia)
void start_player_turn(Player* player);


int main() {
    // --- 1. INICIALIZAÇÃO DO ALLEGRO ---
    must_init(al_init(), "allegro");
    must_init(al_init_image_addon(), "allegro");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0); // 60 FPS
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    // --- 2. INICIALIZAÇÃO DO JOGO ---

    // Inicializa o gerador de números aleatórios (para setup.c)
    // Isso é feito APENAS UMA VEZ no início do programa.
    srand(time(NULL));
    // Variáveis de controle do Allegro
    unsigned char keyboard_keys[ALLEGRO_KEY_MAX];
    ClearKeyboardKeys(keyboard_keys);
    ALLEGRO_EVENT event;

    // Inicializa o display (definido em renderer.c)
    Renderer renderer;
    FillRenderer(&renderer);

    // Registra as fontes de eventos (teclado, timer, fechar janela)
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(renderer.display));

    // --- 3. VARIÁVEIS DE ESTADO DO JOGO ---

    // Define o estado inicial do jogo
    GameState current_state = GAME_STATE_START;
    
    // Variáveis que vão guardar todo o estado do jogo
    Player player;
    Enemy inimigos[2]; // O jogo sempre tem 2 inimigos por combate
    int combates_vencidos = 0;

    // Variáveis de controle do loop principal
    bool running = true;
    bool redraw = true; // 'true' no início para desenhar o primeiro frame

    // --- 4. O LOOP PRINCIPAL DO JOGO ---
    al_start_timer(timer);

    while (running) {
        
        // --- 4A. PROCESSAMENTO DE EVENTOS (Input) ---
        // Espera por um evento (tecla, timer, fechar janela)
        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                // --- Lógica de Input do Teclado ---
                // (Esta lógica roda a 60 FPS)

                if (current_state == GAME_STATE_PLAYER_TURN) {
                    
                    // TODO: Colocar aqui a lógica de input do jogador
                    // Exemplo:
                    // if (keyboard_keys[ALLEGRO_KEY_RIGHT]) {
                    //     selecionar_proxima_carta();
                    // }
                    // if (keyboard_keys[ALLEGRO_KEY_ENTER]) {
                    //     jogar_carta_selecionada();
                    // }

                    // Tecla ESC para encerrar o turno
                    if (keyboard_keys[ALLEGRO_KEY_ESCAPE]) {
                        printf("Turno do jogador encerrado!\n");
                        current_state = GAME_STATE_ENEMY_TURN;
                    }
                }
                
                // Teclas de Debug (conforme especificação)
                if (keyboard_keys[ALLEGRO_KEY_X]) {
                    printf("DEBUG: Vida do jogador reduzida para 1.\n");
                    player.stats.hp_atual = 1;
                }
                if (keyboard_keys[ALLEGRO_KEY_SPACE]) {
                    printf("DEBUG: Inimigos derrotados.\n");
                    inimigos[0].stats.hp_atual = 0;
                    inimigos[1].stats.hp_atual = 0;
                }
                // Tecla Q para sair do jogo
                if (keyboard_keys[ALLEGRO_KEY_Q]) {
                    running = false;
                }

                // Limpa o estado "visto" das teclas
                for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
                    keyboard_keys[i] &= ~GAME_KEY_SEEN;
                }

                redraw = true; // Marca que a tela precisa ser redesenhada
                break;

            // Atualiza o estado das teclas no array
            case ALLEGRO_EVENT_KEY_DOWN:
                keyboard_keys[event.keyboard.keycode] = GAME_KEY_SEEN | GAME_KEY_DOWN;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                keyboard_keys[event.keyboard.keycode] &= ~GAME_KEY_DOWN;
                break;

            // Evento de fechar a janela (clicar no "X")
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
        }

        // Se 'running' for false (Q pressionado ou janela fechada), sai do loop
        if (!running) {
            break;
        }

        // --- 4B. PROCESSAMENTO DA LÓGICA DO JOGO (Máquina de Estados) ---
        // Esta parte SÓ roda quando a lógica do jogo precisa avançar,
        // (ex: mudar de turno), e NÃO a 60 FPS.
        
        switch (current_state) {
            
            case GAME_STATE_START:
                printf("Carregando jogo... Criando jogador...\n");
                player = setup_player(); // Cria o jogador (de setup.c)
                combates_vencidos = 0;
                current_state = GAME_STATE_NEW_COMBAT; // Avança para o próximo estado
                break;

            case GAME_STATE_NEW_COMBAT:
                printf("Iniciando combate %d...\n", combates_vencidos + 1);
                
                // TODO: Implementar a função setup_new_combat
                // setup_new_combat(&player, inimigos);
                
                // Por enquanto, vamos só criar os inimigos:
                inimigos[0] = create_enemy();
                inimigos[1] = create_enemy();
                // TODO: Adicionar lógica para (max 1 inimigo forte)

                printf("Inimigo 1 (Tipo %d) criado com %d HP.\n", inimigos[0].tipo, inimigos[0].stats.hp_atual);
                printf("Inimigo 2 (Tipo %d) criado com %d HP.\n", inimigos[1].tipo, inimigos[1].stats.hp_atual);

                // TODO: Implementar a função start_player_turn
                // start_player_turn(&player);
                // (Esta função deve resetar energia, limpar mão, comprar 5 cartas)

                current_state = GAME_STATE_PLAYER_TURN; // Passa o turno ao jogador
                break;

            case GAME_STATE_PLAYER_TURN:
                // A maior parte da lógica deste estado está na seção de
                // INPUT (ALLEGRO_EVENT_TIMER), pois estamos esperando
                // o jogador apertar teclas.

                // TODO: Checar se todos os inimigos estão mortos
                // bool inimigos_mortos = (inimigos[0].stats.hp_atual <= 0 &&
                //                         inimigos[1].stats.hp_atual <= 0);
                //
                // if (inimigos_mortos) {
                //     combates_vencidos++;
                //     printf("Combate vencido! Total de vitorias: %d\n", combates_vencidos);
                //     if (combates_vencidos >= 10) {
                //         current_state = GAME_STATE_VICTORY;
                //     } else {
                //         current_state = GAME_STATE_NEW_COMBAT;
                //     }
                // }
                break;

            case GAME_STATE_ENEMY_TURN:
                printf("Turno dos inimigos...\n");
                
                // TODO: Implementar a lógica de IA dos inimigos
                // (Percorrer o array inimigos[], fazer eles atacarem/defenderem)
                // ex: enemy_perform_action(&inimigos[0], &player);
                //     enemy_perform_action(&inimigos[1], &player);

                // TODO: Checar se o jogador morreu
                // if (player.stats.hp_atual <= 0) {
                //     current_state = GAME_STATE_GAME_OVER;
                // } else {
                //     // Se não morreu, começa um novo turno do jogador
                //     start_player_turn(&player); // Compra cartas, reseta energia
                //     current_state = GAME_STATE_PLAYER_TURN;
                // }
                
                // Apenas como placeholder, vamos pular de volta ao turno do jogador
                printf("Inimigos agiram. Voltando ao turno do jogador.\n");
                current_state = GAME_STATE_PLAYER_TURN;
                break;

            case GAME_STATE_VICTORY:
                // O jogo para aqui. O loop de evento (input)
                // ainda está rodando, então o jogador pode apertar 'Q'
                // ou fechar a janela para sair.
                // O renderer.c vai desenhar a tela de vitória.
                break;

            case GAME_STATE_GAME_OVER:
                // Mesma coisa, o jogo para aqui.
                // O renderer.c vai desenhar a tela de Game Over.
                break;
        }


        // --- 4C. RENDERIZAÇÃO ---
        // Se a flag 'redraw' for verdadeira (setada pelo timer)
        // e a fila de eventos estiver vazia (processamos toda a lógica)
        if (redraw && al_is_event_queue_empty(queue)) {
            
            // TODO: Você precisará modificar a função Render em
            // renderer.c e renderer.h para que ela aceite
            // o estado do jogo e saiba o que desenhar.
            
            // Exemplo da nova chamada (após modificar renderer.c):
            // Render(&renderer, current_state, player, inimigos);
            
            // Por enquanto, chamamos a original do devkit
            Render(&renderer);
            
            redraw = false;
        }
    }

    // --- 5. LIMPEZA E FINALIZAÇÃO ---
    // Destrói todos os componentes do Allegro
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    ClearRenderer(&renderer); // (de renderer.c)
    
    return 0;
}


/*
// -----------------------------------------------------------------
// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE LÓGICA (Exemplos) ---
// (Você pode mover isso para um novo arquivo, ex: combat.c)
// -----------------------------------------------------------------

void setup_new_combat(Player* player, Enemy inimigos[]) {
    // 1. Criar inimigos (já feito no main por enquanto)
    inimigos[0] = create_enemy();
    inimigos[1] = create_enemy();
    // TODO: Adicionar lógica para (max 1 inimigo forte)

    // 2. Resetar o baralho
    // Copia o baralho principal para a pilha de compra
    player->pilha_compra = player->baralho_completo;
    // Embaralha a pilha de compra
    shuffle_pilha(&(player->pilha_compra));

    // 3. Limpar a mão e o descarte
    player->mao.num_cartas = 0;
    player->pilha_descarte.num_cartas = 0;
}

void start_player_turn(Player* player) {
    // 1. Resetar energia
    player->energia_atual = player->energia_max;

    // 2. Limpar escudo
    player->stats.escudo = 0;

    // 3. Descartar a mão atual
    // TODO: Mover cartas da 'mao' para 'pilha_descarte'

    // 4. Comprar 5 novas cartas
    // TODO: Implementar a função draw_cards(player, 5)
    // (Lembrando de reembaralhar o descarte se a pilha de compra acabar)
}
*/