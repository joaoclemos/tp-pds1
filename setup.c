#include "setup.h"
#include <stdlib.h> 
#include <time.h>   
#include "constants.h"

int random_int_in_range(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

// Cria uma carta baseada no tipo e custo (SEGUINDO AS REGRAS DO ENUNCIADO)
Card create_random_card(TipoCarta tipo, int custo) {
    Card nova_carta;
    nova_carta.tipo = tipo;
    nova_carta.custo_energia = custo;
    nova_carta.magnitude = 0;
    nova_carta.is_vampiric = 0;

    if (tipo == ESPECIAL) {
        nova_carta.custo_energia = 0;
        nova_carta.efeito_valor = 0;
        return nova_carta;
    }

    if (tipo == ATAQUE || tipo == DEFESA) {
        switch (custo) {
            case 0: nova_carta.efeito_valor = random_int_in_range(1, 5); break;
            case 1: nova_carta.efeito_valor = random_int_in_range(5, 10); break;
            case 2: nova_carta.efeito_valor = random_int_in_range(10, 15); break;
            case 3: nova_carta.efeito_valor = random_int_in_range(15, 30); break;
            default: nova_carta.efeito_valor = 1; break;
        }
    }
    else if (tipo == BUFF) {
        int sorteio = rand() % 4; 
        if (sorteio == 0) { // Força
            nova_carta.efeito_valor = ID_FORCA; 
            nova_carta.magnitude = 1; 
        } 
        else if (sorteio == 1) { // Destreza
            nova_carta.efeito_valor = ID_DESTREZA; 
            nova_carta.magnitude = 1; 
        }
        else if (sorteio == 2) { // Cura Instant
            nova_carta.efeito_valor = ID_CURA_INSTANT; 
            nova_carta.magnitude = (custo == 0 ? 1 : custo) * 5; 
        }
        else { // Regen
            nova_carta.efeito_valor = ID_REGEN_RODADAS;
            if (rand() % 2 == 0) { nova_carta.custo_energia = CUSTO_X; nova_carta.magnitude = 0; } 
            else { nova_carta.magnitude = (custo == 0 ? 2 : custo + 1); }
        }
    }
    else if (tipo == DEBUFF) {
        int sorteio = rand() % 4;
        if (sorteio == 0) { // Veneno
            nova_carta.efeito_valor = ID_VENENO; 
            nova_carta.magnitude = random_int_in_range(3, 5); 
        } 
        else if (sorteio == 1) { // Vulnerável
            nova_carta.efeito_valor = ID_VULNERAVEL; 
            nova_carta.magnitude = 1; // Dura 1 rodada
        } 
        else if (sorteio == 2) { // Fraqueza
            nova_carta.efeito_valor = ID_FRAQUEZA; 
            nova_carta.magnitude = 1; // Dura 1 rodada
        }
        else { // Sono
            nova_carta.efeito_valor = ID_SONO; 
            nova_carta.magnitude = 1; 
        }
    }

    return nova_carta;
}

void shuffle_pilha(PilhaCartas *pilha) {
    for (int i = pilha->num_cartas - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = pilha->cartas[i];
        pilha->cartas[i] = pilha->cartas[j];
        pilha->cartas[j] = temp;
    }
}

// --- MODIFICADO: Inclui Golpe Vampírico ---
PilhaCartas create_initial_deck() {
    PilhaCartas baralho;
    baralho.num_cartas = 0;
    int i = 0;

    // --- 7 ATAQUES COMUNS ---
    // (Reduzido de 8 para 7 para dar espaço ao Vampiro)
    baralho.cartas[i++] = create_random_card(ATAQUE, 0);
    baralho.cartas[i++] = create_random_card(ATAQUE, 1);
    baralho.cartas[i++] = create_random_card(ATAQUE, 2);
    baralho.cartas[i++] = create_random_card(ATAQUE, 3);
    for(int k=0; k<3; k++) baralho.cartas[i++] = create_random_card(ATAQUE, random_int_in_range(0, 2));
    
    // --- 6 DEFESAS ---
    baralho.cartas[i++] = create_random_card(DEFESA, 0);
    baralho.cartas[i++] = create_random_card(DEFESA, 1);
    baralho.cartas[i++] = create_random_card(DEFESA, 2);
    baralho.cartas[i++] = create_random_card(DEFESA, 3);
    for(int k=0; k<2; k++) baralho.cartas[i++] = create_random_card(DEFESA, random_int_in_range(0, 2));

    // --- 2 ESPECIAIS ---
    baralho.cartas[i++] = create_random_card(ESPECIAL, 0);
    baralho.cartas[i++] = create_random_card(ESPECIAL, 0);

    // --- 4 BUFFS/DEBUFFS ---
    baralho.cartas[i++] = create_random_card(BUFF, 1);
    baralho.cartas[i++] = create_random_card(BUFF, 1);
    baralho.cartas[i++] = create_random_card(DEBUFF, 1);
    baralho.cartas[i++] = create_random_card(DEBUFF, 1);

    // --- 1 GOLPE VAMPÍRICO (Ataque Especial) ---
    // Conta como o 8º ataque, mas com efeito extra de roubo de vida
    baralho.cartas[i].tipo = ATAQUE;
    baralho.cartas[i].custo_energia = 2;
    baralho.cartas[i].efeito_valor = 8;
    baralho.cartas[i].magnitude = 0;
    baralho.cartas[i].is_vampiric = 1; // Ativa o efeito no logic.c
    i++;

    // Total = 20 Cartas
    baralho.num_cartas = 20;
    shuffle_pilha(&baralho);
    return baralho;
}

Player setup_player() {
    Player jogador;
    jogador.stats.hp_atual = 100;
    jogador.stats.hp_max = 100;
    jogador.stats.escudo = 0;
    
    jogador.stats.forca = 0;
    jogador.stats.destreza = 0;
    jogador.stats.veneno = 0;
    jogador.stats.vulneravel = 0;
    jogador.stats.fraco = 0;
    jogador.stats.regeneracao = 0;
    jogador.stats.dormindo = 0;

    jogador.energia_atual = 3;
    jogador.energia_max = 3;
    
    jogador.baralho_completo = create_initial_deck();
    jogador.pilha_compra.num_cartas = 0;
    jogador.mao.num_cartas = 0;
    jogador.pilha_descarte.num_cartas = 0;
    
    return jogador;
}

Enemy create_enemy() {
    Enemy inimigo;
    float chance = (float)rand() / RAND_MAX;
    int tem_ataque = 0;

    inimigo.stats.forca = 0;
    inimigo.stats.destreza = 0;
    inimigo.stats.veneno = 0;
    inimigo.stats.vulneravel = 0;
    inimigo.stats.fraco = 0;
    inimigo.stats.regeneracao = 0;
    inimigo.stats.dormindo = 0;

    if (chance <= 0.05) {
        inimigo.tipo = FORTE;
        inimigo.stats.hp_max = random_int_in_range(50, 100);
        inimigo.num_acoes_ia = 3; 
    } else {
        inimigo.tipo = FRACO;
        inimigo.stats.hp_max = random_int_in_range(15, 35);
        inimigo.num_acoes_ia = 2; 
    }

    for (int i = 0; i < inimigo.num_acoes_ia; i++) {
        TipoCarta tipo = (rand() % 2 == 0) ? ATAQUE : DEFESA;
        // Inimigos usam custo 0, 1 ou 2
        Card acao = create_random_card(tipo, random_int_in_range(0, 2));
        inimigo.ia_ciclo[i].tipo_acao = acao.tipo;
        inimigo.ia_ciclo[i].valor_efeito = acao.efeito_valor;
        if (tipo == ATAQUE) tem_ataque = 1;
    }

    if (!tem_ataque) {
        inimigo.ia_ciclo[0].tipo_acao = ATAQUE;
    }

    inimigo.stats.hp_atual = inimigo.stats.hp_max;
    inimigo.stats.escudo = 0;
    inimigo.acao_ia_atual = 0; 

    return inimigo;
}

Enemy create_boss() {
    Enemy boss;
    boss.tipo = BOSS;
    boss.stats.hp_max = 300; 
    boss.stats.hp_atual = 300;
    boss.stats.escudo = 0;
    boss.stats.forca = 2; 
    boss.stats.destreza = 0;
    boss.stats.veneno = 0;
    boss.stats.vulneravel = 0;
    boss.stats.fraco = 0;
    boss.stats.regeneracao = 0;
    boss.stats.dormindo = 0;

    boss.num_acoes_ia = 3;
    boss.ia_ciclo[0].tipo_acao = ATAQUE; boss.ia_ciclo[0].valor_efeito = 20; 
    boss.ia_ciclo[1].tipo_acao = DEFESA; boss.ia_ciclo[1].valor_efeito = 30; 
    boss.ia_ciclo[2].tipo_acao = ATAQUE; boss.ia_ciclo[2].valor_efeito = 25; 
    
    boss.acao_ia_atual = 0;
    return boss;
}