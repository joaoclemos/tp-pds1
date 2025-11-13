/* * setup.h
 *
 * Arquivo de cabeçalho para as funções de inicialização do jogo.
 * Declara as funções que serão usadas pelo main.c e outros arquivos
 * para construir o jogador, inimigos e cartas.
 */

// Guarda de cabeçalho, para evitar inclusões duplicadas
#ifndef SETUP_H
#define SETUP_H

// Inclui nossas definições de structs (Player, Enemy, Card...)
#include "game_structs.h"

/*
 * Declaração (ou "protótipo") das funções.
 * Isso diz ao compilador: "Ei, essas funções existem em algum lugar,
 * elas recebem estes parâmetros e retornam este tipo de dado."
 */

// Cria uma carta aleatória com base em um tipo e custo
Card create_random_card(TipoCarta tipo, int custo);

// Monta o baralho inicial de 20 cartas do jogador
PilhaCartas create_initial_deck();

// Inicializa o estado completo do jogador (HP, baralho, energia...)
Player setup_player();

// Gera um inimigo aleatório (fraco ou forte)
Enemy create_enemy();

// Embaralha uma pilha de cartas (vamos usar o algoritmo Fisher-Yates)
// Esta função está no utils.c do devkit? Se não, podemos criá-la.
// Vou assumir que vamos criá-la em setup.c por enquanto.
void shuffle_pilha(PilhaCartas *pilha);

#endif // Fim da guarda de cabeçalho