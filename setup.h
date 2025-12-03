#ifndef SETUP_H
#define SETUP_H
#include "game_structs.h"

Card create_random_card(TipoCarta tipo, int custo);
PilhaCartas create_initial_deck();
Player setup_player();
Enemy create_enemy();
void shuffle_pilha(PilhaCartas *pilha);

#endif