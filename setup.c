#include "setup.h"
#include <stdlib.h> 
#include <time.h>   
#include "constants.h"

int random_int_in_range(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

// Cria uma carta baseada no tipo
Card create_random_card(TipoCarta tipo, int custo) {
    Card nova_carta;
    nova_carta.tipo = tipo;
    nova_carta.custo_energia = custo;
    nova_carta.magnitude = 0;

    if (tipo == ESPECIAL) {
        nova_carta.custo_energia = 0;
        nova_carta.efeito_valor = 0;
        return nova_carta;
    }

    if (tipo == ATAQUE || tipo == DEFESA) {
        switch (custo) {
            case 0: nova_carta.efeito_valor = random_int_in_range(3, 5); break;
            case 1: nova_carta.efeito_valor = random_int_in_range(6, 9); break;
            case 2: nova_carta.efeito_valor = random_int_in_range(10, 14); break;
            case 3: nova_carta.efeito_valor = random_int_in_range(15, 25); break;
        }
    }
    // BUFFS ALEATÓRIOS
    else if (tipo == BUFF) {
        int sorteio = rand() % 4; 
        
        if (sorteio == 0) { // Força
            nova_carta.efeito_valor = ID_FORCA;
            nova_carta.magnitude = random_int_in_range(1, 2);
        } 
        else if (sorteio == 1) { // Destreza
            nova_carta.efeito_valor = ID_DESTREZA;
            nova_carta.magnitude = random_int_in_range(1, 2);
        }
        else if (sorteio == 2) { // Cura Instant
            nova_carta.efeito_valor = ID_CURA_INSTANT;
            nova_carta.magnitude = (custo == 0 ? 1 : custo) * 5; 
        }
        else { // Regeneração (Pode ser Custo X)
            nova_carta.efeito_valor = ID_REGEN_RODADAS;
            // 50% de chance de ser Custo X, 50% de ser normal
            if (rand() % 2 == 0) {
                nova_carta.custo_energia = CUSTO_X;
                nova_carta.magnitude = 0;
            } else {
                nova_carta.magnitude = (custo == 0 ? 2 : custo + 1); 
            }
        }
    }
    // DEBUFFS ALEATÓRIOS
    else if (tipo == DEBUFF) {
        int sorteio = rand() % 4;
        
        if (sorteio == 0) { // Veneno
            nova_carta.efeito_valor = ID_VENENO;
            nova_carta.magnitude = random_int_in_range(3, 5);
        } 
        else if (sorteio == 1) { // Vulnerável
            nova_carta.efeito_valor = ID_VULNERAVEL;
            nova_carta.magnitude = 2;
        } 
        else if (sorteio == 2) { // Fraqueza
            nova_carta.efeito_valor = ID_FRAQUEZA;
            nova_carta.magnitude = 2;
        }
        else { // Sono
            nova_carta.efeito_valor = ID_SONO;
            nova_carta.magnitude = 1; 
            if (custo >= 2) nova_carta.magnitude = 2;
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

PilhaCartas create_initial_deck() {
    PilhaCartas baralho;
    baralho.num_cartas = 0;
    int i = 0;

    // 8 Ataques
    for(int k=0; k<8; k++) baralho.cartas[i++] = create_random_card(ATAQUE, random_int_in_range(0, 2));
    
    // 6 Defesas
    for(int k=0; k<6; k++) baralho.cartas[i++] = create_random_card(DEFESA, random_int_in_range(0, 2));
    
    // 2 Especiais
    baralho.cartas[i++] = create_random_card(ESPECIAL, 0);
    baralho.cartas[i++] = create_random_card(ESPECIAL, 0);

    // 2 BUFFS (Sorteados)
    baralho.cartas[i++] = create_random_card(BUFF, random_int_in_range(1, 2));
    baralho.cartas[i++] = create_random_card(BUFF, random_int_in_range(1, 2));

    // 2 DEBUFFS (Sorteados)
    baralho.cartas[i++] = create_random_card(DEBUFF, random_int_in_range(1, 2));
    baralho.cartas[i++] = create_random_card(DEBUFF, random_int_in_range(1, 2));

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