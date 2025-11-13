/*
 * setup.c
 *
 * Arquivo de implementação para as funções de setup.
 * Contém a lógica real para criar cartas, baralhos e inimigos.
 */

// Inclui o cabeçalho correspondente
#include "setup.h"

// Precisamos destas bibliotecas padrão para:
#include <stdlib.h> // rand() (gerar números aleatórios), NULL
#include <time.h>   // time() (para inicializar o rand())

/*
 * Função auxiliar para gerar um número aleatório dentro de um intervalo [min, max]
 * Ex: random_int_in_range(5, 10) pode retornar 5, 6, 7, 8, 9, ou 10.
 */
int random_int_in_range(int min, int max) {
    // rand() % N retorna um número entre 0 e N-1
    // (max - min + 1) é o total de números possíveis
    // + min ajusta o início do intervalo
    return (rand() % (max - min + 1)) + min;
}

/*
 * Implementação da função que cria uma carta aleatória.
 */
Card create_random_card(TipoCarta tipo, int custo) {
    Card nova_carta;
    nova_carta.tipo = tipo;
    nova_carta.custo_energia = custo;

    // Se for especial, o custo é 0 e o valor não importa
    if (tipo == ESPECIAL) {
        nova_carta.custo_energia = 0;
        nova_carta.efeito_valor = 0; // Não tem valor de efeito
        return nova_carta;
    }

    // Define o valor do efeito (dano/defesa) com base no custo
    switch (custo) {
        case 0:
            // Cartas de custo 0 têm efeito entre 1 e 5
            nova_carta.efeito_valor = random_int_in_range(1, 5);
            break;
        case 1:
            // Cartas de custo 1 têm efeito entre 5 e 10
            nova_carta.efeito_valor = random_int_in_range(5, 10);
            break;
        case 2:
            // Cartas de custo 2 têm efeito entre 10 e 15
            nova_carta.efeito_valor = random_int_in_range(10, 15);
            break;
        case 3:
            // Cartas de custo 3 têm efeito entre 15 e 30
            nova_carta.efeito_valor = random_int_in_range(15, 30);
            break;
    }
    return nova_carta;
}

/*
 * Implementação da função que embaralha uma pilha de cartas.
 * Usa o algoritmo Fisher-Yates (sugerido no seu utils.h/utils.c).
 */
void shuffle_pilha(PilhaCartas *pilha) {
    // Começa do fim da pilha e vai até o começo
    for (int i = pilha->num_cartas - 1; i > 0; i--) {
        // Sorteia um índice 'j' entre 0 e i (inclusive)
        int j = rand() % (i + 1);

        // Troca a carta da posição 'i' com a carta da posição 'j'
        Card temp = pilha->cartas[i];
        pilha->cartas[i] = pilha->cartas[j];
        pilha->cartas[j] = temp;
    }
}


/*
 * Implementação da função que monta o baralho inicial de 20 cartas.
 */
PilhaCartas create_initial_deck() {
    PilhaCartas baralho;
    baralho.num_cartas = 0; // Começa com 0 cartas

    int i = 0; // Usaremos 'i' como o índice para adicionar cartas

    // --- 1. Adicionar as 10 CARTAS DE ATAQUE ---

    // Pelo menos 1 de custo 0
    baralho.cartas[i++] = create_random_card(ATAQUE, 0);
    // Pelo menos 3 de custo 1
    baralho.cartas[i++] = create_random_card(ATAQUE, 1);
    baralho.cartas[i++] = create_random_card(ATAQUE, 1);
    baralho.cartas[i++] = create_random_card(ATAQUE, 1);
    // Pelo menos 1 de custo 2
    baralho.cartas[i++] = create_random_card(ATAQUE, 2);
    // Pelo menos 1 de custo 3
    baralho.cartas[i++] = create_random_card(ATAQUE, 3);
    
    // Agora temos 6 cartas de ataque. Precisamos de mais 4.
    // Vamos adicionar 4 cartas de ataque com custos aleatórios (0 a 3)
    for (int j = 0; j < 4; j++) {
        int custo_aleatorio = random_int_in_range(0, 3);
        baralho.cartas[i++] = create_random_card(ATAQUE, custo_aleatorio);
    }
    // Agora temos 10 cartas de ataque (i = 10)

    // --- 2. Adicionar as 8 CARTAS DE DEFESA ---
    
    // Pelo menos 1 de custo 0
    baralho.cartas[i++] = create_random_card(DEFESA, 0);
    // Pelo menos 3 de custo 1
    baralho.cartas[i++] = create_random_card(DEFESA, 1);
    baralho.cartas[i++] = create_random_card(DEFESA, 1);
    baralho.cartas[i++] = create_random_card(DEFESA, 1);
    // Pelo menos 1 de custo 2
    baralho.cartas[i++] = create_random_card(DEFESA, 2);
    // Exatamente 1 de custo 3
    baralho.cartas[i++] = create_random_card(DEFESA, 3);

    // Agora temos 6 cartas de defesa (índice 10 + 6 = 16). 
    // Precisamos de mais 2.
    for (int j = 0; j < 2; j++) {
        int custo_aleatorio = random_int_in_range(0, 2); // Custo 0, 1 ou 2
        baralho.cartas[i++] = create_random_card(DEFESA, custo_aleatorio);
    }
    // Agora temos 8 cartas de defesa (i = 18)

    // --- 3. Adicionar as 2 CARTAS ESPECIAIS ---
    baralho.cartas[i++] = create_random_card(ESPECIAL, 0);
    baralho.cartas[i++] = create_random_card(ESPECIAL, 0);
    // Total de 20 cartas (i = 20)

    // Define o número total de cartas no baralho
    baralho.num_cartas = 20;

    // --- 4. Embaralhar o baralho ---
    shuffle_pilha(&baralho);

    return baralho;
}


/*
 * Implementação da função que inicializa o jogador.
 */
Player setup_player() {
    Player jogador;

    // Define os stats iniciais
    jogador.stats.hp_atual = 100;
    jogador.stats.hp_max = 100;
    jogador.stats.escudo = 0;
    jogador.energia_atual = 3; // Energia é redefinida a cada turno
    jogador.energia_max = 3;

    // Cria o baralho principal
    jogador.baralho_completo = create_initial_deck();

    // As outras pilhas (compra, mão, descarte) começam vazias
    // Elas serão preenchidas no início de cada combate
    jogador.pilha_compra.num_cartas = 0;
    jogador.mao.num_cartas = 0;
    jogador.pilha_descarte.num_cartas = 0;

    return jogador;
}


/*
 * Implementação da função que cria um inimigo aleatório.
 */
Enemy create_enemy() {
    Enemy inimigo;

    // Sorteia a chance de ser forte (5%)
    // (float)rand() / RAND_MAX gera um número float entre 0.0 e 1.0
    float chance = (float)rand() / RAND_MAX;

    if (chance <= 0.05) {
        // --- INIMIGO FORTE (5% de chance) ---
        inimigo.tipo = FORTE;
        // HP entre 40 e 100
        inimigo.stats.hp_max = random_int_in_range(40, 100);
        // IA com 2 ou 3 ações
        inimigo.num_acoes_ia = random_int_in_range(2, 3);
        
        // Regra: "no máximo uma delas de nível 1"
        int custo1_usado = 0;
        for (int i = 0; i < inimigo.num_acoes_ia; i++) {
            int custo_acao;
            if (custo1_usado == 0) {
                // Pode gerar custo 1, 2 ou 3
                custo_acao = random_int_in_range(1, 3);
                if (custo_acao == 1) {
                    custo1_usado = 1; // Marca que já usamos o custo 1
                }
            } else {
                // Se já usou custo 1, só pode gerar 2 ou 3
                custo_acao = random_int_in_range(2, 3);
            }
            
            // Sorteia se a ação é Ataque ou Defesa (50/50)
            TipoCarta tipo_acao = (rand() % 2 == 0) ? ATAQUE : DEFESA;
            // Cria a ação (usando a mesma lógica de criar cartas)
            Card acao_card = create_random_card(tipo_acao, custo_acao);
            
            inimigo.ia_ciclo[i].tipo_acao = acao_card.tipo;
            inimigo.ia_ciclo[i].valor_efeito = acao_card.efeito_valor;
        }

    } else {
        // --- INIMIGO FRACO (95% de chance) ---
        inimigo.tipo = FRACO;
        // HP entre 10 e 30
        inimigo.stats.hp_max = random_int_in_range(10, 30);
        // IA com 1 ou 2 ações
        inimigo.num_acoes_ia = random_int_in_range(1, 2);

        for (int i = 0; i < inimigo.num_acoes_ia; i++) {
            // Ações seguem restrições de custo 0 ou 1
            int custo_acao = random_int_in_range(0, 1);
            // Sorteia se a ação é Ataque ou Defesa (50/50)
            TipoCarta tipo_acao = (rand() % 2 == 0) ? ATAQUE : DEFESA;
            
            Card acao_card = create_random_card(tipo_acao, custo_acao);
            
            inimigo.ia_ciclo[i].tipo_acao = acao_card.tipo;
            inimigo.ia_ciclo[i].valor_efeito = acao_card.efeito_valor;
        }
    }

    // Define os stats de combate iniciais
    inimigo.stats.hp_atual = inimigo.stats.hp_max;
    inimigo.stats.escudo = 0;
    inimigo.acao_ia_atual = 0; // Começa na primeira ação do ciclo

    return inimigo;
}